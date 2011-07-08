#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ulistbox.h"

UListBox::UListBox(HWND hParent, UINT nID, HINSTANCE hInst)
:UControl(hParent, nID, hInst)
{}

UListBox::~UListBox()
{};

UListBox::UListBox(UBaseWindow *pWndParent, UINT nID)
: UControl(pWndParent, nID)
{}

BOOL UListBox::create()
{
    return UControl::create(_T("LISTBOX")); // WC_LISTBOX
}

int UListBox::addString(const char *lpszString)
{
    return this->sendMsg(LB_ADDSTRING, 0, (LPARAM)lpszString);
}

int UListBox::setItemData( int index, LPARAM data )
{
    return this->sendMsg(LB_SETITEMDATA, (WPARAM)index, data);
}

LONG UListBox::getItemData(int index)
{
    return (LONG)this->sendMsg(LB_GETITEMDATA, (WPARAM)index);
}


int UListBox::findString(const char *lpszString)
{
    return this->sendMsg(LB_FINDSTRING, (WPARAM)-1, (LPARAM)lpszString);
}

int UListBox::deleteString(int nIndex)
{
    return this->sendMsg(LB_DELETESTRING, (WPARAM)nIndex, 0);
}

int UListBox::getCurSel()
{
    return this->sendMsg(LB_GETCURSEL);
}

BOOL UListBox::setCurSel(int nIndex)
{
    return this->sendMsg(LB_SETCURSEL, (WPARAM)nIndex);
}

int UListBox::getText(int nIndex, char *sText)
{
    return this->sendMsg(LB_GETTEXT, (WPARAM)nIndex, (WPARAM)sText);
}

int UListBox::setColumnWidth(DWORD pixels)
{
    this->sendMsg(LB_SETHORIZONTALEXTENT, 600);
    return this->sendMsg(LB_SETCOLUMNWIDTH, pixels);
}

int UListBox::getItemHeight(int nIndex)
{
    return this->sendMsg(LB_GETITEMHEIGHT, (WPARAM)nIndex);
}

BOOL UListBox::setItemHeight(int nHeight, int nIndex)
{
    return this->sendMsg(LB_SETITEMHEIGHT, (WPARAM)nIndex, nHeight);
}
