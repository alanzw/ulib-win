#define _WIN32_IE 0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ustatusbar.h"

UStatusBar::UStatusBar(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{
    m_dwStyles |= SBARS_SIZEGRIP;
}

UStatusBar::~UStatusBar()
{}

BOOL UStatusBar::create()
{
    // Ensure that the common control DLL is loaded.
    InitCommonControls();
    //
    return UControl::create(_T("msctls_statusbar32"));
}

BOOL UStatusBar::setText(int nPart, TCHAR *lpText)
{
    return this->sendMsg(SB_SETTEXT, (WPARAM)(INT)nPart, (LPARAM)(LPSTR)lpText);
}

BOOL UStatusBar::setIcon(int nPart, HICON hIcon)
{
    return this->sendMsg(SB_SETICON, (WPARAM)nPart, (LPARAM)hIcon);
}

HICON UStatusBar::setIcon(int nPart)
{
    return (HICON)this->sendMsg(SB_GETICON, (WPARAM)nPart);
}

BOOL UStatusBar::setParts(int nCount, int *aWidths)
{
    return this->sendMsg(SB_SETPARTS, (WPARAM)nCount, (LPARAM)aWidths);;
}

BOOL UStatusBar::getParts(int nCount, int *aWidths)
{
    return this->sendMsg(SB_GETPARTS, (WPARAM)nCount, (LPARAM)aWidths);;
}

BOOL UStatusBar::getPartRect(int nPart, LPRECT lpRect)
{
    return this->sendMsg(SB_GETRECT, (WPARAM)nPart, (LPARAM)lpRect);
}

BOOL UStatusBar::setMinHeight(int nHeight)
{
    this->sendMsg(SB_SETMINHEIGHT, (WPARAM)nHeight);
    return this->sendMsg(WM_SIZE);
}

BOOL UStatusBar::simplify(BOOL bSimple /*= FALSE*/)
{
    return this->sendMsg(SB_SIMPLE, (WPARAM)bSimple);
}

BOOL UStatusBar::isSimple()
{
    return this->sendMsg(SB_ISSIMPLE);
}
