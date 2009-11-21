#include "hwApp.h"

#ifdef BUILD_DLL

extern "C"
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //LOG_STRING("Ulib-win DLL_PROCESS_ATTACH!");
        break;
    case DLL_THREAD_ATTACH:
        //LOG_STRING("Ulib-win DLL_THREAD_ATTACH!");
        break;
    case DLL_THREAD_DETACH:
        //LOG_STRING("Ulib-win DLL_THREAD_DETACH!");
        break;
    case DLL_PROCESS_DETACH:
        //LOG_STRING("Ulib-win DLL_PROCESS_DETACH!");
        break;
    }
    return TRUE;
}

#elif defined(HW_CONSOLE)

extern hwAppConsole * hwCreateApp();

int hwMain(int argc, hwCharPtrPtr argv)
{
    hwAppConsole *app = hwCreateApp();

    if (!app->init(argc, argv))
    {
        return -1;
    }

    if (!app->onRun())
    {
        return -2;
    }

    if (!app->onCleanup())
    {
        return -3;
    }

    if (!app->onExit())
    {
        return -4;
    }

    return 0;
}

extern "C" int _tmain(int argc, TCHAR **argv)
{
    return hwMain(argc, argv);
}

#elif defined(HW_GUI)

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
    return hwWinMain(hInstance, lpCmdLine, nCmdShow);
}

#else // Static Lib


#endif // HW_CONSOLE
