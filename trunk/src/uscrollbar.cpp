#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uscrollbar.h"

UScrollBar :: UScrollBar(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst)
{}

UScrollBar::~UScrollBar()
{}

BOOL UScrollBar::create()
{
    return UControl::create(_T("ScrollBar"));
}

