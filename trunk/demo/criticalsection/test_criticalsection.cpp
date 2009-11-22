#include <windows.h>
#include <iostream>

using std::cout;
using std::endl;

#include "ucritical.h"

int g_tickets = 10;

UCriticalSection g_cs;

DWORD WINAPI thread_proc1(LPVOID lpParam)
{
    while (true)
    {
        UCriticalSection::scoped_lock lock(g_cs);

        if (g_tickets <= 0) break;

        Sleep(1);
        cout << "thread1: " << g_tickets-- << endl;
    }
    return 0;
}

DWORD WINAPI thread_proc2(LPVOID lpParam)
{
    while (true)
    {
        UCriticalSection::scoped_lock lock(g_cs);
        if (g_tickets<=0) break;

        cout << "thread2: " << g_tickets-- << endl;
    }
    return 0;
}

void thread_CriticalSection()
{
    HANDLE handle1 = CreateThread(NULL, 0, thread_proc1, NULL, 0, NULL);
    HANDLE handle2 = CreateThread(NULL, 0, thread_proc2, NULL, 0, NULL);
    CloseHandle(handle1);
    CloseHandle(handle2);
    Sleep(200);   //等待线程执行完闭
}

int main()
{
    cout << "++Critical Section++" << endl;
    thread_CriticalSection();
    cout << "++Critical Section++" << endl;
    return 0;
}
