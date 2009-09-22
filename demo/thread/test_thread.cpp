#include "resource.h"
#include <windows.h>
#include <iostream>

#include "uthread.h"

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

    return 0;
}
