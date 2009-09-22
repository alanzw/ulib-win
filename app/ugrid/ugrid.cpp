#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ugrid.h"

UGridCtrl::UGridCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
: UListView(hParent, nID, hInst)
{
    this->setStyles( LVS_REPORT | WS_BORDER );
}

UGridCtrl::UGridCtrl()
{
}

UGridCtrl::~UGridCtrl()
{}

