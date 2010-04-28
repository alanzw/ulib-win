#include "hwApp.h"

int hwWinMain(HINSTANCE hInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    int nArgc = 0;
    LPTSTR * lpArgv = hwCommandLineToArgv(lpCmdLine, &nArgc);
    return hwMain(nArgc, lpArgv);
}

LPTSTR * hwCommandLineToArgv(LPCTSTR lpCmdLine, int *lpArgc)
{
    return NULL;
}


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
