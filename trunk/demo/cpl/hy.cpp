// Control Panel Library
#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <cpl.h>

#include "ulib.h"

HINSTANCE g_hModule = 0;

extern "C"
BOOL APIENTRY DllMain( HINSTANCE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        LOG_STRING("hy.cpl DLL_PROCESS_ATTACH!");
        break;
    case DLL_THREAD_ATTACH:
        LOG_STRING("hy.cpl DLL_THREAD_ATTACH!");
        break;
    case DLL_THREAD_DETACH:
        LOG_STRING("hy.cpl DLL_THREAD_DETACH!");
        break;
    case DLL_PROCESS_DETACH:
        LOG_STRING("hy.cpl DLL_PROCESS_DETACH!");
        break;
    }
    return TRUE;
}


LONG OnNewInquire(UINT uAppNum, NEWCPLINFO* pInfo)
{
    // Fill in the data
    pInfo->dwSize		  = sizeof(NEWCPLINFO);
    pInfo->dwFlags		  = 0;
    pInfo->dwHelpContext  = 0;
    pInfo->lData		  = 0;
    pInfo->hIcon		  = ::LoadIcon(g_hModule, MAKEINTRESOURCE(IDI_APP));

    lstrcpy(pInfo->szName, "hy");
    lstrcpy(pInfo->szInfo, "HY CPL");
    lstrcpy(pInfo->szHelpFile, _T(""));

    return 0;
}

LONG OnInquire(UINT uAppNum, CPLINFO* pInfo)
{
    pInfo->idIcon	= IDI_APP;
    pInfo->idName	= IDS_CPL_NAME;
    pInfo->idInfo	= IDS_CPL_INFO;
    pInfo->lData	= 0;

    return 0;
}

LONG OnDblclk(HWND hWnd, UINT uAppNum, LONG lData)
{
 // Insert your Control Panel Applet dialog (property 
 // sheet) code here
    ::MessageBox(NULL, "hy", "CPL", MB_OK);
    return 0;
}

LONG OnExit()
{
 return 0; // OK
}

LONG OnGetCount()
{
 return 1; // only one applet
}

LONG OnInit()
{
 return 1; // OK
}

LONG OnStop(UINT uAppNum, LONG lData)
{
    return 1; // not handled
}


extern "C" __declspec(dllexport)
LONG APIENTRY CPlApplet(HWND hWnd, UINT uMsg, LPARAM lp1, LPARAM lp2)
{
    switch (uMsg)
    {
    case CPL_DBLCLK:
        return OnDblclk(hWnd, lp1, lp2);
    case CPL_EXIT:
        return OnExit();
    case CPL_GETCOUNT:
        return OnGetCount();
    case CPL_INIT:
        return OnInit();
    case CPL_INQUIRE:
        return OnInquire(lp1, (CPLINFO*)lp2);
    case CPL_NEWINQUIRE:
        return OnNewInquire(lp1, (NEWCPLINFO*)lp2);
    case CPL_STOP:
        return OnStop(lp1, lp2);
    case CPL_STARTWPARMS:
        return OnDblclk(hWnd, lp1, lp2);
    default:
        break;
    }

    return 1;
}


   