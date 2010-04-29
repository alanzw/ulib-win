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
        ::MessageBox(NULL, buf, "Loader", MB_OK);
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(injected_dllname), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, (LPVOID)RemoteString, dllname, strlen(dllname), NULL);
    CreateRemoteThread(Proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, 0, NULL);

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
      MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", MB_OK);
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

//
//    InjectThread
//
//    Arguments:
//        ppi    - pointer to PROCESS_INFORMATION structure (i.e. as returned
//              by CreateProcess. Only need to know the proc/thread HANDLES, not IDs
//
//        pCode     - pointer to routine to inject
//        nCodeLen - size (in bytes) of this function
//        pData     - pointer to data to inject
//        nDataLen - size (in bytes) of data
//
//
//    [ ???? ???? ???? ???? ???? ]      <--- Orig. ESP
//  [ EFlags                   ]
//  [ SegCs                       ]
//  [ Orig. EIP                   ]
//  [ Eax/Ebx/Ecx..               ]
//  [ ...                       ]
//  [ POP ESP / POPAD / IRETD  ] <--\  <--- User func returns here (0xCF6158)
//                                  |
//  [                           ]    |
//  [        USER DATA          ]    |
//    [                           ]<-\ |
//                                  | |
//    [       -- padding --       ]  | |
//                                | |
//  [                           ]  | |
//  [        USER CODE          ]  | |
//    [                           ]  | |  <--- EIP points here
//  [ ESP (points to popad regs]  | |
//  [ Argument (ptr to data)   ] -/ |
//  [ Return address (cleanup) ]----/  <--- ESP points here
//
//
//    Once ESP and EIP have been properly setup, the thread
//  is resumed and execution begins at the start of the user-code
//  (on the thread's stack). Because the function is __stdcall,
//  when the function returns, the return address and function
//  argument are popped off the stack (using a RETN 4 instruction which
//  must be the last instruction in the user-supplied code) and
//  execution shifts to the first DWORD of the cleanup[] array, which contains
//  three (3) op-codes: POP ESP, POPAD, IRETD.
//
//  The first of these (POP ESP) takes the current DWORD stored at
//  the current location of the stack pointer (we carefully placed it there).
//  After the POP ESP executes, ESP now jumps up the stack to the second DWORD in
//  the cleanup[] array, which contains all the registers (in order) required
//  by the POPAD instruction (which executes next)
//
//  The last instruction to execute is IRETD - this pops it's return address
//  off the stack (the original value of EIP before we injected), and also
//  pops (and restores) the CS register and Flags register - once this is complete,
//  all registers are back in their initial state, ESP points back to the original
//  stack location, and execution resumes where it left off. Sweet :-)
//


BOOL injectThread(PPROCESS_INFORMATION ppi, PINJCODE pCode, ULONG nCodeLen, PVOID pData, ULONG nDataLen)
{
    CONTEXT    context = { CONTEXT_FULL };

    ULONG    cleanup[12];
    ULONG    callenv[3];
    ULONG   stackptr;

    if(-1 == SuspendThread(ppi->hThread))
        return FALSE;

    GetThreadContext(ppi->hThread, &context);

    // construct cleanup frame
    cleanup[11] = context.EFlags;
    cleanup[10] = context.SegCs;
    cleanup[9]  = context.Eip;        // return address for IRETD
    cleanup[8]  = context.Eax;
    cleanup[7]  = context.Ecx;
    cleanup[6]  = context.Edx;
    cleanup[5]  = context.Ebx;
    cleanup[4]  = 0xffffffff;        // esp, ignored by POPAD
    cleanup[3]  = context.Ebp;
    cleanup[2]  = context.Esi;
    cleanup[1]  = context.Edi;
    cleanup[0]  = 0x00CF615C;        // POP ESP / POPAD / IRETD

    stackptr = context.Esp - sizeof(cleanup);
    WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, cleanup, sizeof(cleanup), 0);

    // push user-supplied data
    stackptr -= nDataLen;
    WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, pData, nDataLen, 0);

    // push user-supplied code
    stackptr= (stackptr - nCodeLen) & ~0x1f;
    WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, (PVOID)pCode, nCodeLen, 0);

    // push code environment
    callenv[2] = context.Esp - sizeof(cleanup) + 4;            // ESP for executing cleanup
    callenv[1] = context.Esp - sizeof(cleanup) - nDataLen;    // Argument - pointer to user data
    callenv[0] = context.Esp - sizeof(cleanup);                // Return address - to cleanup code

    stackptr -= sizeof(callenv);
    WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, callenv, sizeof(callenv), 0);

    // resume execution on thread's stack
    context.Esp = stackptr;
    context.Eip = stackptr + sizeof(callenv);

    FlushInstructionCache(ppi->hProcess, (PVOID)context.Eip, nCodeLen);
    SetThreadContext(ppi->hThread, &context);

    ResumeThread(ppi->hThread);

    return TRUE;
}

}; // namespace DLLInject

}; // namespace huys
