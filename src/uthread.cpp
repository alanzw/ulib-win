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

bool UThread::create()
{
    m_hThread = ::CreateThread(
                    NULL,          // default security attributes
                    0,             // use default stack size
                    m_lpStartAddr, // thread function name
                    m_lpData,       // argument to thread function
                    0,             // use default creation flags
                    &m_dwThreadId);   // returns the thread identifier
    if(!m_hThread)
    {
        return false;
    }

    return true;
}

bool UThread::wait()
{
    ::WaitForSingleObject(m_hThread, INFINITE);
}

bool UThread::suspend()
{

}

bool UThread::resume()
{

}

bool UThread::stop()
{

}

DWORD UThread::getExitCode()
{
    if (m_hThread == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    else
    {
        ::GetExitCodeThread(m_hThread, &m_dwExitCode);
        return m_dwExitCode;
    }
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
