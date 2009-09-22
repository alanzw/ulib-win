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

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.

    _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );

    CloseHandle( hProcess );
}

DWORD getWindowPid(HWND hWnd, LPDWORD lpdwProcessId)
{
    return ::GetWindowThreadProcessId(hWnd, lpdwProcessId);
}

