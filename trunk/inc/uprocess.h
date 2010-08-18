#ifndef U_PROCESS_H
#define U_PROCESS_H

#include "uobject.h"

class ULIB_API UProcess : public UKernelObject
{
public:
    UProcess();
    ~UProcess();

    BOOL create( LPCTSTR sCmdLine,
                 DWORD dwCreateFlags = CREATE_SUSPENDED);

    BOOL resume();
    BOOL resumeAndWait();
    DWORD getExitCode();
private:
    STARTUPINFO m_si;
    PROCESS_INFORMATION m_pi;
protected:

public:
    static BOOL runProgram(char *programName = "notepad.exe", DWORD dwTimeout = 5000);
    static LPTSTR getCommadLine();
    static DWORD getCurrentProcessId() {return ::GetCurrentProcessId();}
    static HANDLE getCurrentProcess() {return ::GetCurrentProcess();}
    static void getStartupInfo(STARTUPINFO *pSI)
    {
        ::GetStartupInfo(pSI);
    };
};



class ULIB_API UProgressList
{
public:
    UProgressList();
    ~UProgressList();
protected:
private:
};

#define MAX_LINE_LENGTH 1024

class ULIB_API UPipedProcess
{
public:
    UPipedProcess() { }
    UPipedProcess(char *cmd, HWND hwnd, UINT id = 0);
    BOOL run();
    static DWORD WINAPI listener(LPVOID me);
    ~UPipedProcess()
    {
        //if(command)
        //    delete[] command;
        //if (pContent)
        //    delete[] pContent;
    }
    static void deleteContent()
    {
        if (spContent)
            delete[] spContent;
    }
protected:
    void listen();
    TCHAR command[MAX_LINE_LENGTH];
    HWND target;
    HANDLE hreadFromChild;
    UINT pid;   // process ID assigned by caller
    TCHAR buffer[MAX_LINE_LENGTH+1];
    static TCHAR *spContent;
};

#define UPM_FINISHED_MSG _T("UPM_FINISHED-{D741F7E0-9F58-11d5-A043-006067718D04}")
#define UPM_LINE_MSG _T("UPM_LINE-{D741F7E1-9F58-11d5-A043-006067718D04}")
#define UPM_PROCESS_HANDLE_MSG _T("UPM_PROCESS_HANDLE-{D741F7E2-9F58-11d5-A043-006067718D04}")

//
#ifndef IMPLEMENT_MSG
#define IMPLEMENT_MSG(x) static UINT x = ::RegisterWindowMessage(x##_MSG);
#endif

//
IMPLEMENT_MSG(UPM_FINISHED)
IMPLEMENT_MSG(UPM_LINE)
IMPLEMENT_MSG(UPM_PROCESS_HANDLE)

#endif // U_PROCESS_H
