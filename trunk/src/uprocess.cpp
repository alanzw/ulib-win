#include <windows.h>
#include <tchar.h>

#include <cassert>

#include "uprocess.h"
#include "umsg.h"

UProcess::UProcess()
{
    ::ZeroMemory( &m_si, sizeof(STARTUPINFO) );
    m_si.cb = sizeof(STARTUPINFO);
    m_si.dwFlags = STARTF_USESHOWWINDOW;
    m_si.wShowWindow = SW_MAXIMIZE;

    ::ZeroMemory( &m_pi, sizeof(PROCESS_INFORMATION) );
}

UProcess::~UProcess()
{}

BOOL UProcess::create(LPCTSTR sCmdLine, DWORD dwCreateFlags)
{
    return ::CreateProcess(
        NULL,
        (LPTSTR)sCmdLine,
        NULL,
        NULL,
        TRUE,
        dwCreateFlags,
        NULL,
        NULL,
        &m_si,
        &m_pi);
}

BOOL UProcess::resume()
{
    return ((unsigned int)-1 != ::ResumeThread(m_pi.hThread));
}

BOOL UProcess::resumeAndWait()
{
    return this->resume() && ::WaitForSingleObject(m_pi.hProcess, INFINITE);
}

DWORD UProcess::getExitCode()
{
    DWORD dwExitCode;
    if (0 != ::GetExitCodeProcess(m_pi.hProcess, &dwExitCode))
    {
        return dwExitCode;
    }
    else
    {
        showError("Process::");
        return -1;
    }
}

/*
typedef struct _STARTUPINFO {
DWORD  cb;
LPTSTR lpReserved;
LPTSTR lpDesktop;
LPTSTR lpTitle;
DWORD  dwX;
DWORD  dwY;
DWORD  dwXSize;
DWORD  dwYSize;
DWORD  dwXCountChars;
DWORD  dwYCountChars;
DWORD  dwFillAttribute;
DWORD  dwFlags;
WORD   wShowWindow;
WORD   cbReserved2;
LPBYTE lpReserved2;
HANDLE hStdInput;
HANDLE hStdOutput;
HANDLE hStdError;
}STARTUPINFO, *LPSTARTUPINFO;
*/

BOOL UProcess::runProgram(char *programName /* = "notepad.exe" */, DWORD dwTimeout /* = 5000 */ )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    BOOL bResult;

    ::ZeroMemory( &si, sizeof(STARTUPINFO) );
    si.cb = sizeof(si);
    si.dwFlags=STARTF_USESHOWWINDOW;
    si.wShowWindow=SW_MAXIMIZE;

    ::ZeroMemory( &pi, sizeof(pi) );

    bResult = ::CreateProcess(NULL,
        programName,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE| NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        &si,
        &pi);

    if (!bResult) return FALSE;

    ::WaitForSingleObject(pi.hProcess, dwTimeout);

    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);

    return TRUE;
}

LPTSTR UProcess::getCommadLine()
{
    return ::GetCommandLine();
}


TCHAR *UPipedProcess::spContent = NULL;

UPipedProcess::UPipedProcess(char *cmd, HWND hwnd, UINT id)
{
    assert(hwnd != NULL);
    strcpy(command, cmd);
    target = hwnd;
    pid = id;
}

BOOL UPipedProcess::run()
{
    hReadFromChild = NULL;
    HANDLE hwriteToParent = NULL;
    HANDLE hwriteToParent2 = NULL;

    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE }; // inheritable handle

    if(!::CreatePipe(&hReadFromChild, &hwriteToParent, &sa, 0))
    { /* pipe failed */
        // ::GetLastError() will reveal the cause
        delete this;
        return FALSE;
    } /* pipe failed */

    if(!::DuplicateHandle(GetCurrentProcess(),     // duplicate from this process
        hwriteToParent,        // this handle
        GetCurrentProcess(),     // into this process
        &hwriteToParent2,        // as this handle
        0,                // no access flags (subsumed by DUPLICATE_SAME_ACCESS)
        TRUE,                // create inheritable
        DUPLICATE_SAME_ACCESS))  // create duplicate access
    { /* duplicate failed */
        DWORD err = ::GetLastError();
        ::CloseHandle(hReadFromChild);
        ::CloseHandle(hwriteToParent);
        ::SetLastError(err);
        delete this;
        return FALSE;
    } /* duplicate failed */

    STARTUPINFO startup;
    PROCESS_INFORMATION procinfo;

    ::ZeroMemory(&startup, sizeof(startup));

    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    startup.wShowWindow = SW_HIDE; // hidden console window
    startup.hStdInput = NULL; // not used
    startup.hStdOutput = hwriteToParent;
    startup.hStdError = hwriteToParent2;

    // We want a non-inherited read handle. DuplicateHandle with a
    // NULL target fixes the read side to be non-inheritable
    ::DuplicateHandle(::GetCurrentProcess(),    // in this process
        hReadFromChild,           // child read handle
        ::GetCurrentProcess(),    // to this process
        NULL,                     // modify existing handle
        0,                        // flags
        FALSE,                    // not inheritable
        DUPLICATE_SAME_ACCESS);   // same handle access

    // We need a writeable buffer for the command (silly Windows restriction)
    //LPTSTR cmd = new char[strlen(command)+1];

    BOOL started = ::CreateProcess(NULL,        // command is part of input string
        command,         // (writeable) command string
        NULL,        // process security
        NULL,        // thread security
        TRUE,        // inherit handles flag
        0,           // flags
        NULL,        // inherit environment
        NULL,        // inherit directory
        &startup,    // STARTUPINFO
        &procinfo);  // PROCESS_INFORMATION
    //delete[] cmd;

    //assert(started);

    if(!started)
    { /* failed to start */
        DWORD err = ::GetLastError(); // preserve across CloseHandle calls
        showErrorByNum(err);
        ::CloseHandle(hReadFromChild);
        ::CloseHandle(hwriteToParent);
        ::CloseHandle(hwriteToParent2);
        ::SetLastError(err);
        ::PostMessage(target, UPM_FINISHED, (WPARAM)err, (LPARAM)pid);
        delete this;
        return FALSE;
    } /* failed to start */

    ::PostMessage(target, UPM_PROCESS_HANDLE, (WPARAM)procinfo.hProcess, (LPARAM)pid);

    // Now close the output pipes so we get true EOF/broken pipe
    ::CloseHandle(hwriteToParent);
    ::CloseHandle(hwriteToParent2);

    // We have to create a listener thread. We create a worker
    // thread that handles this
    DWORD dwThreadId;
    HANDLE hthread = ::CreateThread(
        NULL,
        0,
        listener,
        (LPVOID)this,
        0,
        &dwThreadId);

    //assert(hthread!=NULL);

    if(hthread == NULL)
    { /* failed */
        DWORD err = ::GetLastError();
        ::PostMessage(target, UPM_FINISHED, (WPARAM)err, (LPARAM)pid);
        ::CloseHandle(hReadFromChild);
        ::CloseHandle(hwriteToParent);
        ::CloseHandle(hwriteToParent2);
        delete this;
        return FALSE;
    } /* failed */

    ::WaitForSingleObject(hthread, INFINITE);
    CloseHandle(hthread);
    return TRUE;
} // Process::run

DWORD WINAPI UPipedProcess::listener(LPVOID me)
{
    ((UPipedProcess *)me)->listen();
    return 0;
} // Process::listener



void UPipedProcess::listen()
{
    //::WaitForSingleObject(hChildThread, INFINITE);
    //Sleep(2000);

    spContent = new TCHAR[MAX_LINE_LENGTH*10];
    ::ZeroMemory(spContent, sizeof(TCHAR)*MAX_LINE_LENGTH*10);

    //TCHAR *pLine = new TCHAR[MAX_LINE_LENGTH+1];
    //::ZeroMemory(pLine, sizeof(TCHAR)*(MAX_LINE_LENGTH+1));

    DWORD bytesRead;

    while(::ReadFile(hReadFromChild, buffer, sizeof(buffer) - sizeof(TCHAR), &bytesRead, NULL))
    { /* got data */
        if(bytesRead == 0)
            break; // EOF condition

        //assert(bytesRead!=0);

        buffer[bytesRead] = _T('\0');
        //wsprintf(pLine, "%s", buffer);
        strcat(spContent, buffer);
        // Convert to lines
        //LPTSTR b = buffer;
        //while(TRUE)
        //{
        /* convert and send */
        //LPTSTR p = _tcschr(b, _T('\n'));
        //if(p == NULL)
        //{
        /* incomplete line */
        //    _tcscat(line, b);
        //    break; // leave assembly loop
        // } /* incomplete line */
        // else
        //{
        /* complete line */
        //    int offset = 0;
        //    if(p - b > 0)
        //     { /* get rid of \r */
        //        if(p[-1] == _T('\r'))
        //            offset = 1;
        //     } /* get rid of \r */
        //     _tcscpy(line+_tcslen(line), b);
        //     b = p + 1;
        //::ZeroMemory(pLine, sizeof(TCHAR)*(MAX_LINE_LENGTH+1));
        //   } /* complete line */
        //} /* convert and send */
    } /* got data */
    //assert(bytesRead!=0);
    DWORD err = ::GetLastError();

    ::CloseHandle(hReadFromChild);


    if(strlen(spContent) > 0)
        PostMessage(target, UPM_LINE, (WPARAM)spContent, (LPARAM)pid);

    //delete[] pLine;
    //else
    //{
    //    PostMessage(target, UPM_LINE, (WPARAM)"haxxxx", (LPARAM)pid);
    //    delete line;
    //}
    DWORD status = 0;
    if(err != ERROR_BROKEN_PIPE)
        status = err;

    PostMessage(target, UPM_FINISHED, status, (LPARAM)pid);

    delete this;
} // Process::listener
