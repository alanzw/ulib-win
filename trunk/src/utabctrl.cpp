#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "utabctrl.h"

UTabCtrl::UTabCtrl(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{}

UTabCtrl::~UTabCtrl()
{}

BOOL UTabCtrl::create()
{
    ::InitCommonControls();

    return UControl::create(_T("SysTabControl32"));
}

BOOL UTabCtrl::insertItem(int nIndex, LPTCITEM pitem)
{
    return this->sendMsg(TCM_INSERTITEM, (WPARAM)nIndex, (LPARAM)pitem);
}

BOOL UTabCtrl::insertItemTextImage(int nIndex, LPCTSTR lpText, int iImage /* = 0*/)
{
    TCITEM tie = {0};
    tie.mask = TCIF_TEXT | TCIF_IMAGE;
    tie.iImage = iImage;
    tie.pszText = (LPTSTR)lpText;
    return this->insertItem(nIndex, &tie);
}

BOOL UTabCtrl::setImageList( HIMAGELIST himl )
{
    return this->sendMsg(TCM_SETIMAGELIST, 0, (LPARAM)himl);
}

int UTabCtrl::getCurSel()
{
    return this->sendMsg(TCM_GETCURSEL);
}

int UTabCtrl::setCurSel(int nIndex)
{
    return this->sendMsg(TCM_SETCURSEL, (WPARAM)nIndex);
}

int UTabCtrl::getItemCount()
{
    return this->sendMsg(TCM_GETITEMCOUNT);
}

LONG UTabCtrl::setItemSize(int cx, int cy)
{
    return this->sendMsg(TCM_SETITEMSIZE, 0, (LPARAM) MAKELPARAM (cx, cy));
}

BOOL UTabCtrl::getItemRect(int nIndex, LPRECT lpRect)
{
    return this->sendMsg(TCM_GETITEMRECT, (WPARAM)nIndex, (LPARAM)lpRect);
}

