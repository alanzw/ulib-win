#include <windows.h>
#include <tchar.h>

#include "uedit.h"

UEdit::UEdit(HWND hParent, UINT nResource, HINSTANCE hInst)
:UControl(hParent, nResource, hInst)
{}

UEdit::UEdit(UBaseWindow *pWndParent, UINT nID)
:UControl(pWndParent, nID)
{}

UEdit::~UEdit()
{};

BOOL UEdit::create()
{
    return UControl::create(_T("EDIT")); // EDITCLASS
}

BOOL UEdit::setText(const TCHAR *lpText)
{
    return this->sendMsg(WM_SETTEXT, 0, (LPARAM)lpText);
}

int UEdit::getText(TCHAR *lpText)
{
    return this->sendMsg(WM_GETTEXT, (WPARAM)EDIT_MAX_LENGTH, (LPARAM)lpText);
}

BOOL UEdit::clear()
{
    return this->sendMsg(WM_SETTEXT, 0, (LPARAM)"");
}

int UEdit::getLineCount()
{
    return this->sendMsg(EM_GETLINECOUNT);
}

int UEdit::getLineLength(int n)
{
    return this->sendMsg(EM_LINELENGTH, (WPARAM)n);
}

LONG UEdit::scrollLineDown()
{
    return this->sendMsg(EM_SCROLL, (WPARAM)SB_LINEDOWN);
}

BOOL UEdit::setLimitText( int n )
{
    return this->sendMsg(EM_SETLIMITTEXT, (WPARAM)n);
}

BOOL UEdit::limitText( int n )
{
    return this->sendMsg(EM_LIMITTEXT, (WPARAM)n);
}

BOOL UEdit::addLine(LPCTSTR sLine)
{
    TCHAR buf[1024] = {0};
    this->getText(buf);
    if ('\0' != buf[0])
    {
        lstrcat(buf, _T("\r\n"));
    }
    lstrcat(buf, sLine);
    return this->setText(buf);
}
