#ifndef U_THREAD_H
#define U_THREAD_H

#include "uobject.h"

class ULIB_API UThread : public UKernelObject
{
public:
    UThread(LPTHREAD_START_ROUTINE start_routine, LPVOID lpData);
    ~UThread();
    bool create();
    bool wait();
    bool suspend();
    bool resume();
    bool stop();
    DWORD getExitCode();

	enum Priority {
		TP_LOWEST       = THREAD_PRIORITY_LOWEST,
		TP_BELOW_NORMAL = THREAD_PRIORITY_BELOW_NORMAL,
		TP_NORMAL       = THREAD_PRIORITY_NORMAL,
		TP_ABOVE_NORMAL = THREAD_PRIORITY_ABOVE_NORMAL,
		TP_HIGHEST      = THREAD_PRIORITY_HIGHEST
	};

	bool setPriority(Priority nPriority);

protected:

private:
    //HANDLE m_hThread;
    DWORD m_dwThreadId;
    LPTHREAD_START_ROUTINE m_lpStartAddr;
    LPVOID m_lpData;

    DWORD m_dwExitCode;
};

/*! thread exit callback function */
//typedef DWORD WINAPI (*U_LPTHREAD_EXIT_ROUTINE)(LPVOID lpParam);
#define U_LPTHREAD_EXIT_ROUTINE LPTHREAD_START_ROUTINE


struct UThreadParam {
    /*! Message receiver window handle */
    HWND hWnd;
    /*! New Thread ID */
    DWORD dwThreadID;
    /*! Thread exit code */
    int nExitCode;
    /*! The wait time for the child process */
    DWORD dwMilliseconds;
    /*! The message sent to receiver window */
    UINT nMessage;
    /*! The cmdline string passed to CreateProcess */
    const TCHAR *sCmdline;
    /*! The working directory of the child process */
    const TCHAR *sWDir;
    /*! The function called when quit the thread */
    U_LPTHREAD_EXIT_ROUTINE lpExitFunc;
    /*! The parameter passed to exit function */
    LPVOID lpExitFuncParam;
};

//! spawn and execute
/*! spawn a new thread, create and wait for new child process. */
ULIB_API bool spawnNewThreadAndExec(UThreadParam *utParam);

#endif // U_THREAD_H
