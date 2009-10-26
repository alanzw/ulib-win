#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "usortctrl.h"

USortCtrl::USortCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

USortCtrl::USortCtrl(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent->getHandle(), nID, ::GetModuleHandle(NULL))
{
}

USortCtrl::USortCtrl()
{
}

USortCtrl::~USortCtrl()
{}

