#define _WIN32_IE 0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "utoolbar.h"

UToolBar::UToolBar(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{}

UToolBar::~UToolBar()
{}

BOOL UToolBar::create()
{
    return UControl::create(_T("ToolbarWindow32")); // TOOLBARCLASSNAME
}

BOOL UToolBar::setImageList(HIMAGELIST himl)
{
    return this->sendMsg(TB_SETIMAGELIST, 0, (LPARAM)himl);
}

BOOL UToolBar::loadImages(UINT nIDBitmap)
{
    return this->sendMsg(TB_LOADIMAGES, (WPARAM)nIDBitmap, (LPARAM)HINST_COMMCTRL);
}

BOOL UToolBar::addButtons(int num, TBBUTTON *ptbb)
{
    this->sendMsg(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    this->sendMsg(TB_ADDBUTTONS, (WPARAM)num, (LPARAM)ptbb);

    return TRUE;
}

BOOL UToolBar::autosize()
{
    return this->sendMsg(TB_AUTOSIZE);
}

BOOL UToolBar::enableButton(int idButton)
{
    return this->sendMsg(TB_ENABLEBUTTON, (WPARAM)(int)idButton, (LPARAM) MAKELONG (TRUE, 0));
}

BOOL UToolBar::disableButton(int idButton)
{
    return this->sendMsg(TB_ENABLEBUTTON, (WPARAM)(int)idButton, (LPARAM) MAKELONG (FALSE, 0));
}

DWORD UToolBar::setExtendStyle(DWORD dwExStyle)
{
#if (_WIN32_IE >= 0x0400)
    return this->sendMsg(TB_SETEXTENDEDSTYLE, 0, (LPARAM)(DWORD)dwExStyle);
#else
    return 0;
#endif
}

DWORD UToolBar::getExtendStyle()
{
#if (_WIN32_IE >= 0x0400)
    return this->sendMsg(TB_GETEXTENDEDSTYLE);
#else
    return 0;
#endif
}

BOOL UToolBar::setHotImageList(HIMAGELIST himl)
{
    return this->sendMsg(TB_SETHOTIMAGELIST, 0, (LPARAM)himl);
}

DWORD UToolBar::getButtonSize()
{
    return this->sendMsg(TB_GETBUTTONSIZE);
}

DWORD UToolBar::getButtonRect( int nId, LPRECT lpRect)
{
    return this->sendMsg(TB_GETBUTTONSIZE, (WPARAM)nId, (LPARAM)lpRect);
}

BOOL UToolBar::hideButton( int nId )
{
    return this->sendMsg(TB_HIDEBUTTON, (WPARAM)nId, (LPARAM) MAKELONG (TRUE, 0));
}

BOOL UToolBar::showButton( int nId )
{
    return this->sendMsg(TB_HIDEBUTTON, (WPARAM)nId, (LPARAM) MAKELONG (FALSE, 0));
}

int UToolBar::getButtonCount()
{
    return this->sendMsg(TB_BUTTONCOUNT);
}

BOOL UToolBar::getString( int nIndex, LPTSTR lpText, BYTE cchMax )
{
//    return this->sendMsg(TB_GETSTRING, MAKEWPARAM(cchMax, nIndex), (LPARAM)lpText);
	return FALSE;
}

BOOL UToolBar::getButton( int nIndex, LPTBBUTTON lpTbBtn )
{
    return this->sendMsg(TB_GETBUTTON, (WPARAM)nIndex, (LPARAM)lpTbBtn);
}

BOOL UToolBar::getButtonInfo( UINT nID, LPTBBUTTONINFO lptbbi )
{
    return this->sendMsg(TB_GETBUTTONINFO, (WPARAM)nID, (LPARAM)lptbbi);
}

BOOL UToolBar::setButtonInfo(UINT nID, LPTBBUTTONINFO lptbbi)
{
    return this->sendMsg(TB_SETBUTTONINFO, (WPARAM)nID, (LPARAM)lptbbi);
}

BOOL UToolBar::getButtonText( UINT nID, LPTSTR lpszText )
{
    return this->sendMsg(TB_GETBUTTONTEXT, (WPARAM)nID, (LPARAM)lpszText);
}

BOOL UToolBar::setButtonWidth(int cxMin, int cxMax)
{
    return this->sendMsg(TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax));
}

 BOOL UToolBar::setButtonSize(int dx, int dy)
 {
    return this->sendMsg(TB_SETBUTTONSIZE, 0, MAKELPARAM(dx, dy));
 }

 BOOL UToolBar::setBitmapSize(int dx, int dy)
 {
    return this->sendMsg(TB_SETBITMAPSIZE, 0, MAKELPARAM(dx, dy));
 }
