#include "resource.h"
#include <windows.h>
#include <iostream>

#include "uthread.h"

#include <process.h>

unsigned Counter;
unsigned __stdcall SecondThreadFunc( void* pArguments )
{
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

    // Create the second thread.
    hThread = (HANDLE)_beginthreadex( NULL, 0, &SecondThreadFunc, NULL, 0, &threadID );

    // Wait until second thread terminates. If you comment out the line
    // below, Counter will not be correct because the thread has not
    // terminated, and Counter most likely has not been incremented to
    // 1000000 yet.
    WaitForSingleObject( hThread, INFINITE );
    printf( "Counter should be 1000000; it is-> %d\n", Counter );
    // Destroy the thread object.
    CloseHandle( hThread );
}

DWORD WINAPI myThreadFunc(LPVOID lpParam)
{
    int *pNum = (int *)lpParam;
    std::cout << "child thread: " << *pNum << std::endl;
    (*pNum)++;

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
