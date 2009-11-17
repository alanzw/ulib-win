#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <shlwapi.h>
//#include <cassert>
#include <stdio.h>

#include "udllman.h"

UDllMan::UDllMan()
{}

UDllMan::~UDllMan()
{
    this->free();
}

UDllMan::UDllMan(const TCHAR *filepath)
{
    this->load(filepath);
}

int UDllMan::load(const TCHAR *filepath)
{
    m_hDll = ::LoadLibrary(filepath);

    if (m_hDll == NULL)
    {
        throw new UDllException(UDllMan::UDllException::UDLL_ERROR_LOAD_LIBRARY);
        return 1;
    }

    //assert(m_hDll);

    return 0;
}

FARPROC WINAPI UDllMan::find(const TCHAR *funcname)
{
    FARPROC proc = ::GetProcAddress(m_hDll, funcname);

    if (proc == NULL)
    {
        throw new UDllException(UDllMan::UDllException::UDLL_ERROR_GET_PROC_ADDRESS);
        return NULL;
    }

    return proc;
}

int UDllMan::free()
{
    if (NULL != m_hDll)
    {
        ::FreeLibrary(m_hDll);
    }
	return 0;
}

namespace huys
{

namespace DLLInject
{
// I could just use PROCESS_ALL_ACCESS
// but it's always best to use the absolute
// bare minimum of priveleges, so that your code works in as
// many circumstances as possible.
#define CREATE_THREAD_ACCESS      \
    ( PROCESS_CREATE_THREAD |     \
      PROCESS_QUERY_INFORMATION | \
      PROCESS_VM_OPERATION |      \
      PROCESS_VM_WRITE |          \
      PROCESS_VM_READ )

BOOL injectDll(DWORD dwPid, char *dllname, char *injected_dllname)
{
    HANDLE Proc;
    char buf[50]={0};
    LPVOID RemoteString, LoadLibAddy;

    if(!dwPid)
        return false;

    Proc = ::OpenProcess(CREATE_THREAD_ACCESS, FALSE, dwPid);

    if(!Proc)
    {
        sprintf(buf, "OpenProcess() failed: %d", GetLastError());
        ::MessageBox(NULL, buf, "Loader", NULL);
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(injected_dllname), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, (LPVOID)RemoteString, dllname, strlen(dllname), NULL);
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

    CloseHandle(Proc);

    return TRUE;
}

unsigned long GetTargetProcessIdFromProcname(char *procName)
{
   PROCESSENTRY32 pe;
   HANDLE thSnapshot;
   BOOL retval, ProcFound = false;

   thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   if(thSnapshot == INVALID_HANDLE_VALUE)
   {
      MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", NULL);
      return false;
   }

   pe.dwSize = sizeof(PROCESSENTRY32);

   retval = Process32First(thSnapshot, &pe);

   while(retval)
   {
      if(StrStrI(pe.szExeFile, procName) )
      {
         ProcFound = true;
         break;
      }

      retval    = Process32Next(thSnapshot,&pe);
      pe.dwSize = sizeof(PROCESSENTRY32);
   }

   LOG_STRING("Find Process %s", pe.szExeFile);

   return pe.th32ProcessID;
  }

unsigned long GetTargetProcessIdFromWindow(char *className, char *windowName)
{
    unsigned long pid;
    HWND targetWnd;

    targetWnd = FindWindow(className, windowName);
    GetWindowThreadProcessId(targetWnd, &pid);

    return pid;
}

unsigned long GetTargetThreadIdFromWindow(char *className, char *windowName)
{
    HWND targetWnd;
    HANDLE hProcess;
    unsigned long pid, pTID, threadID;

    targetWnd = FindWindow(className, windowName);
    GetWindowThreadProcessId(targetWnd, &pid);

#ifdef _MSC_VER
    _asm {
        mov eax, fs:[0x18]
        add eax, 36
        mov [pTID], eax
    }
#endif // _MSC_VER

#ifdef __GNUC__

    asm ("movl %%fs:0x18, %%eax\n\t"
        "addl $36, %%eax\n\t"
        "movl %%eax, %0\n\t"
        :"=r"(pTID)
        :
        :);

#endif


    hProcess = OpenProcess(PROCESS_VM_READ, false, pid);
    ReadProcessMemory(hProcess, (const void *)pTID, &threadID, 4, NULL);
    CloseHandle(hProcess);

    return threadID;
}

BOOL injectLibW(DWORD dwPid, LPCWSTR sLibFile)
{
    BOOL fOk = FALSE;
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    LPWSTR lpLibFileRemote = NULL;

    //
    hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | // For CreateRemoteThread
        PROCESS_VM_OPERATION  | // For VirtualAllocEx/VirtualFreeEx
        PROCESS_VM_WRITE,
        FALSE,
        dwPid);
    if (NULL == hProcess)
    {
        return FALSE;
    }


    //
    int cch = 1 + lstrlenW(sLibFile);
    int cb = cch * sizeof(WCHAR);

    //
    lpLibFileRemote = (LPWSTR)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
    if (NULL == lpLibFileRemote)
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    //
    if (!WriteProcessMemory(hProcess, lpLibFileRemote, (PVOID)sLibFile, cb, NULL))
    {
        CloseHandle(hProcess);
        return FALSE;
    }

    LPTHREAD_START_ROUTINE pfnThreadRtn =
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
    if (NULL == pfnThreadRtn)
    {
        return FALSE;
    }

    //
    hThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        pfnThreadRtn,
        lpLibFileRemote,
        0,
        NULL);

    if (NULL == hThread)
    {
        return FALSE;
    }

    //
    WaitForSingleObject(hThread, INFINITE);

    fOk = TRUE;

    // finally
    if (NULL != lpLibFileRemote)
    {
        VirtualFreeEx(hProcess, lpLibFileRemote, 0, MEM_RELEASE);
    }

    if (NULL != hThread)
    {
        CloseHandle(hThread);
    }

    if (NULL != hProcess)
    {
        CloseHandle(hProcess);
    }

    return fOk;
}

BOOL injectLibA(DWORD dwPid, LPCSTR sLibFile)
{
    LPWSTR sLibFileW = new WCHAR[lstrlenA(sLibFile)+1];
    //
    wsprintfW(sLibFileW, L"%s", sLibFile);
    //
    return injectLibW(dwPid, sLibFileW);
}

BOOL ejectLibW(DWORD dwPid, LPCWSTR sLibFile)
{
    BOOL fOk = FALSE;
    HANDLE hthSnapshot = NULL;
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;

    //
    hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
    if (NULL == hthSnapshot)
    {
        return FALSE;
    }

    //
    MODULEENTRY32W me = {sizeof(me)};
    BOOL fFound = FALSE;
    BOOL fMoreMods = Module32FirstW(hthSnapshot, &me);
    for (; fMoreMods; fMoreMods = Module32NextW(hthSnapshot, &me))
    {
        fFound = (lstrcmpiW(me.szModule, sLibFile) == 0) ||
            (lstrcmpiW(me.szExePath, sLibFile) == 0);
        if (fFound)
        {
            break;
        }
    }

    return fOk;
}


}; // namespace DLLInject

}; // namespace huys
