#include <windows.h>
#include <tchar.h>

#include "uhook.h"

#ifdef _MSC_VER
    #pragma data_seg(push, ".HUYS")
    HHOOK g_hHook = NULL;
    HMODULE g_hModule = NULL;
    int aa = 0;
    BOOL g_bSubclassed = FALSE;
    WNDPROC s_OldWndProc = NULL;
    WNDPROC s_NewWndProc = NULL;
    HWND g_hMsgWnd = NULL;
    HWND g_hWnd = NULL;
    #pragma data_seg(pop)
    #pragma comment(linker, "/section:.HUYS,RWS")
#endif

#ifdef __GNUC__
HHOOK g_hHook __attribute__((section ("Shared"), shared)) = NULL;
HMODULE g_hModule __attribute__((section ("Shared"), shared)) = NULL;
int aa __attribute__((section ("Shared"), shared)) = 0;
BOOL g_bSubclassed __attribute__((section ("Shared"), shared)) = FALSE;
WNDPROC s_OldWndProc __attribute__((section ("Shared"), shared))  = NULL;
WNDPROC s_NewWndProc __attribute__((section ("Shared"), shared)) = NULL;
HWND g_hMsgWnd __attribute__((section ("Shared"), shared)) = NULL;
HWND g_hWnd __attribute__((section ("Shared"), shared)) = NULL;
#endif


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
        ::PostMessage(g_hWnd, UWM_MOUSEMOVE, 0, 0);
    }

    return 1;
}

LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (WM_RBUTTONDBLCLK == wParam)
        {
            ::PostMessage(g_hWnd, UWM_MOUSEMOVE, 0, 0);
            //::SetWindowText(g_hWnd, "GGGGGG");
        }
    }
    return ::CallNextHookEx(g_hHook, nCode, wParam,lParam);
}

//The CBT hook Procedure (Computer Based Training Hook)
LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode==HCBT_ACTIVATE || nCode == HCBT_MINMAX || nCode == HCBT_MOVESIZE || nCode == HCBT_SETFOCUS)  //Called when the application window is activated
    {
        //::PostMessage(g_hMsgWnd, 51111, 111, 111);
        if((HWND)(wParam) == g_hWnd)  //check if the window activated is Our Targer App
        {
            if (g_bSubclassed == FALSE)
            {
                    s_OldWndProc=(WNDPROC)::SetWindowLong(g_hWnd, GWL_WNDPROC, (LONG)s_NewWndProc);
                    g_bSubclassed = TRUE;    // Set state as subclassed
                    ::PostMessage(g_hMsgWnd, 51111, 111, 111);
            }
        }
        return 0;
    }

    if (nCode==HCBT_DESTROYWND) //Called when the application window is destroyed
    {

        if((HWND)wParam == g_hWnd)
        {
            //SendNotifyMessage(hApp,WM_APP +1024,(WPARAM)wParam,(LPARAM)lParam);// Send the message  to the vb app
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}//End of the hook procedure

BOOL setMyHook(HWND hWnd)
{
    //LOG_STRING("SetHook!");

    g_hWnd = hWnd;

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

LRESULT newWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //LOG_STRING("xxxx_WM_COMMAND");
    if (uMsg == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case 11112:
            {
                ::MessageBox(hwnd, "hww1", "info", MB_OK|MB_ICONINFORMATION);
                break;
                //return FALSE;
            }
        case 11113:
            {
                ::MessageBox(hwnd, "hww2", "info", MB_OK|MB_ICONINFORMATION);
                break;
                //return FALSE;
            }
        default:
            ;
        }

    }
    return CallWindowProc((WNDPROC)s_OldWndProc, hwnd, uMsg, wParam, lParam);
}

ULIB_API BOOL setCBTHook( HWND hWnd, HWND hMsgWnd)
{
    g_hWnd = hWnd;
    //s_NewWndProc = (WNDPROC)&newWndProc;
    s_NewWndProc = (WNDPROC)newWndProc;
    g_hMsgWnd = hMsgWnd;

    if (NULL == g_hHook)
    {
        //g_hHook = ::SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hModule, 0);
        g_hHook = ::SetWindowsHookEx(WH_CBT, (HOOKPROC)CBTProc, g_hModule, 0);
    }

    //::PostMessage(g_hMsgWnd, 51111, 111, 111);

    return NULL != g_hHook;
}



WNDPROC getOldProc()
{
    return s_OldWndProc;
}

UHook::UHook()
{

}

UHook::~UHook()
{

}
