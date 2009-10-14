#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ulife.h"

ULifeCtrl::ULifeCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

ULifeCtrl::ULifeCtrl()
{
}

ULifeCtrl::ULifeCtrl(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, "")
{
    m_nResourceID = nID;
}

ULifeCtrl::~ULifeCtrl()
{}

