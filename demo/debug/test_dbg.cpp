#include <windows.h>

#include <iostream>

using std::cout;
using std::cin;
using std::endl;

#include "udebug.h"

#if 0
#ifdef HU_DEBUG
    #define LOG_STRING(format, ...) UDebugger::odprintf(format, ##__VA_ARGS__)
    //#define LOG_STRING(format, args...) UDebugger::odprintf(format, ##args)
#else
    #define LOG_STRING(...)
#endif
#endif

int main()
{
    OutputDebugString("Hello Debuger!");

    UDebugger::odprintf("Hi, I am here, %d", 333);

    LOG_STRING("Now end debugging: 1");

    cin.ignore();

    return 0;
}

