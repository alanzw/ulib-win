#include <windows.h>

#include "ulib.h"

#ifdef BUILD_DLL
extern "C"
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LOG_STRING("Ulib-win DLL_PROCESS_ATTACH!");
    case DLL_THREAD_ATTACH:
        LOG_STRING("Ulib-win DLL_THREAD_ATTACH!");
    case DLL_THREAD_DETACH:
        LOG_STRING("Ulib-win DLL_THREAD_DETACH!");
    case DLL_PROCESS_DETACH:
        LOG_STRING("Ulib-win DLL_PROCESS_DETACH!");
        break;
    }
    return TRUE;
}
#endif // BUILD_DLL

