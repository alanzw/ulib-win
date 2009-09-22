#include <windows.h>
#include <tchar.h>

#include "uhook.h"

#ifdef _MSC_VER
    #pragma data_seg(push, ".HUYS")
    HHOOK g_hHook = NULL;
    HMODULE g_hModule = NULL;
    int aa = 0;
    #pragma data_seg(pop)
    #pragma comment(linker, "/section:.HUYS,RWS")
#endif

#ifdef __MINGW_H
HHOOK g_hHook __attribute__((section ("Shared"), shared)) = NULL;
HMODULE g_hModule __attribute__((section ("Shared"), shared)) = NULL;
int aa __attribute__((section ("Shared"), shared)) = 0;
#endif

HWND s_hWnd = NULL;

extern "C"
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        LOG_STRING("UHook DLL_PROCESS_ATTACH!");
        break;
    case DLL_THREAD_ATTACH:
        g_hModule = hModule;
        LOG_STRING("UHook DLL_THREAD_ATTACH!");
        break;
    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        LOG_STRING("UHook DLL_THREAD_DETACH!");
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        LOG_STRING("UHook DLL_PROCESS_DETACH!");
        break;
    }
    return TRUE;

    UNREFERENCED_PARAMETER(lpReserved);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (nCode < 0)
    {
        return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    if (HC_ACTION == nCode)
    {
        //::MessageBox(NULL, "xx", "xx", MB_OK);
        ::PostMessage(s_hWnd, UWM_MOUSEMOVE, 0, 0);
    }

    return 1;
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (WM_RBUTTONDBLCLK == wParam)
        {
            ::PostMessage(s_hWnd, UWM_MOUSEMOVE, 0, 0);
            //::SetWindowText(s_hWnd, "GGGGGG");
        }
    }
    return ::CallNextHookEx(g_hHook, nCode, wParam,lParam);
}

BOOL setMyHook(HWND hWnd)
{
    //LOG_STRING("SetHook!");

    s_hWnd = hWnd;

    if (NULL == g_hHook)
    {
        //g_hHook = ::SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hModule, 0);
        g_hHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, g_hModule, 0);
    }

    return NULL != g_hHook;
}

BOOL clearMyHook()
{
    BOOL bRet = FALSE;
    if (g_hHook)
    {
        bRet = ::UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }

    return bRet;
}

int getValue()
{
    return aa;
}

void setValue(int val)
{
    aa = val;
}

UHook::UHook()
{

}

UHook::~UHook()
{

}
