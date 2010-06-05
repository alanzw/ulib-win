#include "resource.h"

#include <windows.h>
#include <process.h>
#include <tchar.h>

#include <iostream>

using std::cout;

#include <stdio.h>

#include "uthread.h"

#define MY_MESSAGE WM_USER+100
const int MAX_INFO_SIZE = 20;

HANDLE hStartEvent; // thread start event

unsigned Counter;

unsigned __stdcall SecondThreadFunc( void* pArguments )
{
    cout<< "thread function start\n";

    MSG msg;
    // force the system to create the message queue
    ::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    if(!SetEvent(hStartEvent)) //set thread start event 
    {
        printf("set start event failed,errno:%d\n",::GetLastError());
        return 1;
    }
    
    while(true)
    {
        if(GetMessage(&msg,0,0,0)) // get message from the message queue
        {
            switch(msg.message)
            {
            case MY_MESSAGE:
                char * pInfo = (char *)msg.wParam;
                printf("recv %s\n",pInfo);
                delete[] pInfo;
                break;
            }
        }
    };

    printf( "In second thread...\n" );

    while ( Counter < 1000000 )
        Counter++;

    _endthreadex( 0 );
    return 0;
}

void test()
{
    HANDLE hThread;
    unsigned threadID;

    printf( "Creating second thread...\n" );
    
    hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
    if(hStartEvent == 0)
    {
        printf("create start event failed,errno:%d\n",::GetLastError());
        return;
    }


    // Create the second thread.
    hThread = (HANDLE)_beginthreadex( NULL, 0, &SecondThreadFunc, NULL, 0, &threadID );
    
    //wait thread start event to avoid PostThreadMessage return errno:1444
    ::WaitForSingleObject(hStartEvent,INFINITE);
    ::CloseHandle(hStartEvent);


    // Wait until second thread terminates. If you comment out the line
    // below, Counter will not be correct because the thread has not
    // terminated, and Counter most likely has not been incremented to
    // 1000000 yet.
    //WaitForSingleObject( hThread, INFINITE );
    //printf( "Counter should be 1000000: it is-> %d\n", Counter );
    
    
    int count = 0;
    while(20 > count)
    {
        char* pInfo = new char[MAX_INFO_SIZE]; //create dynamic msg
        sprintf(pInfo,"msg_%d",++count);
        
        if(!PostThreadMessage(threadID,MY_MESSAGE,(WPARAM)pInfo,0))//post thread msg
        {
            printf("post message failed,errno:%d\n",::GetLastError());
            delete[] pInfo;
        }
        ::Sleep(1000);
    }

    
    // Destroy the thread object.
    CloseHandle( hThread );
}

DWORD WINAPI myThreadFunc(LPVOID lpParam)
{
    int *pNum = (int *)lpParam;
    std::cout << "child thread: " << *pNum << std::endl;
    (*pNum)++;

    _endthreadex(0);
    return 0;
}

int main(int argc, char *argv[])
{
    int aNum = 0;
    std::cout << "main  thread: " << aNum << std::endl;

    UThread aThread(myThreadFunc, &aNum);
    aThread.create();
    aNum++;
    aThread.wait();

    std::cout << "exit  code  : " << aThread.getExitCode() << std::endl;
    std::cout << "main  thread: " << aNum << std::endl;

    std::cin.ignore();

    test();

    return 0;
}
