#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
  #include "fix/psapi.h"
#else
  #include <psapi.h>
#endif

#include "uprocessman.h"

BOOL isProgramUp(const TCHAR *name)
{
    char szProcessName[MAX_PATH] = {0};

    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i = 0;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return FALSE;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {

            //
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,
                FALSE, aProcesses[i]);

            //
            if ( NULL != hProcess )
            {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName));
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }

            if (lstrcmpi(name, szProcessName) == 0)
            {
                ::CloseHandle(hProcess);
                return TRUE;
            }
        }

    }

    return FALSE;
}

BOOL startProgram( const TCHAR *cmdline,
                   const TCHAR *wdir,
                   BOOL bWait /* = FALSE */ )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL bResult;

    ::ZeroMemory( &si, sizeof(STARTUPINFO) );
    si.cb = sizeof(si);
    si.dwFlags=STARTF_USESHOWWINDOW;
    si.wShowWindow=SW_NORMAL;

    ::ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );

    bResult = ::CreateProcess(NULL,
        (TCHAR *)cmdline,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE| NORMAL_PRIORITY_CLASS,
        NULL,
        wdir,
        &si,
        &pi);

    if (!bResult) return FALSE;

    if (bWait)
    {
        ::WaitForSingleObject(pi.hProcess, INFINITE);
    }

    //g_hChildProcess = pi.hProcess;

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return FALSE;
}

BOOL stopProgram(const TCHAR *name)
{
    char szProcessName[MAX_PATH] = "unknown";

    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i = 0;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return FALSE;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {

            //
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE,
                FALSE, aProcesses[i]);

            //
            if ( NULL != hProcess )
            {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName));
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }

            if (lstrcmpi(name, szProcessName) == 0)
            {
                ::TerminateProcess(hProcess,0);
            }
            else
            {
                ::CloseHandle(hProcess);
            }
        }

    }
    return FALSE;
}

VOID printProcessName(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE,
                                   processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.

    _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, (UINT)processID );

    CloseHandle( hProcess );
}

DWORD getWindowPid(HWND hWnd, LPDWORD lpdwProcessId)
{
    return ::GetWindowThreadProcessId(hWnd, lpdwProcessId);
}

DWORD terminatePid(DWORD dwPID, DWORD dwTimeout)
{
    HANDLE hProcess;
    DWORD dwRet;

    //
    hProcess = ::OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwPID);

    if(NULL == hProcess)
    {
        return -1;
    }

    //::EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM)dwPID);

    //
    if(WAIT_OBJECT_0 != ::WaitForSingleObject(hProcess, dwTimeout))
        dwRet = (::TerminateProcess(hProcess,0)? 1:-1);
    else
        dwRet = 1;

    ::CloseHandle(hProcess);

     return dwRet;
}

