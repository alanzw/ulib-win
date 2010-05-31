#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwdemo.h"

UWDemo::UWDemo(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UWDemo::UWDemo()
{
}

UWDemo::UWDemo(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID)
{
    m_nResourceID = nID;
}

UWDemo::~UWDemo()
{}

