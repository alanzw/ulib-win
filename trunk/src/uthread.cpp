#include <windows.h>
#include <tchar.h>
#include <cassert>

#include "uthread.h"
#include "uprocessman.h"

UThread::UThread(LPTHREAD_START_ROUTINE start_routine, LPVOID lpData)
: m_lpStartAddr(start_routine), m_lpData(lpData), m_dwExitCode(-1)
{}

UThread::~UThread()
{}

bool UThread::create(DWORD dwCreationFlags /* = 0 */)
{
    m_hObj = ::CreateThread(
                   NULL,            // default security attributes
                   0,               // use default stack size
                   m_lpStartAddr,   // thread function name
                   m_lpData,        // argument to thread function
                   dwCreationFlags,               // use default creation flags
                   &m_dwThreadId);  // returns the thread identifier
    if(!m_hObj)
    {
        return false;
    }

    return true;
}

bool UThread::wait()
{
    ::WaitForSingleObject(m_hObj, INFINITE);
    return false;
}

bool UThread::suspend()
{
    return ((unsigned int)-1 != ::SuspendThread(m_hObj));
}

bool UThread::resume()
{
    return ((unsigned int)-1 != ::ResumeThread(m_hObj));
}

bool UThread::stop()
{
    return false;
}

DWORD UThread::getExitCode()
{
    if (m_hObj == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    else
    {
        ::GetExitCodeThread(m_hObj, &m_dwExitCode);
        return m_dwExitCode;
    }
}

bool UThread::setPriority( Priority nPriority )
{
    return (TRUE == ::SetThreadPriority(m_hObj, nPriority));
}


//
static DWORD WINAPI SNEThreadStartRoutine(LPVOID lpParam)
{
    UThreadParam *lpThreadParam = (UThreadParam *)lpParam;

    startProgram(lpThreadParam->sCmdline, lpThreadParam->sWDir, TRUE);

    if (NULL != lpThreadParam->lpExitFunc)
    {
        lpThreadParam->lpExitFunc(lpThreadParam->lpExitFuncParam);
    }

    ::PostMessage( lpThreadParam->hWnd,
                   lpThreadParam->nMessage,
                   (WPARAM)lpThreadParam->nExitCode,
                   (LPARAM)0);

    return 0;
}

//
bool spawnNewThreadAndExec( UThreadParam *utParam )
{
    HANDLE hThread = ::CreateThread(
                            NULL,
                            0,
                            &SNEThreadStartRoutine,
                            utParam,
                            0,
                            &utParam->dwThreadID);
    return (NULL != hThread);
}
