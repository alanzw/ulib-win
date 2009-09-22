#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ulistbox.h"

UListBox::UListBox(HWND hParent, UINT nID, HINSTANCE hInst)
:UControl(hParent, nID, hInst)
{}

UListBox::~UListBox()
{};

BOOL UListBox::create()
{
    return UControl::create(_T("LISTBOX")); // WC_LISTBOX
}

int UListBox::addString(const char *lpszString)
{
    return this->sendMsg(LB_ADDSTRING, 0, (LPARAM)lpszString);
}

int UListBox::setItemData( WPARAM index, LPARAM data )
{
    return this->sendMsg(LB_SETITEMDATA, index, data);
}

int UListBox::findString(const char *lpszString)
{
    return this->sendMsg(LB_FINDSTRING, (WPARAM)-1, (LPARAM)lpszString);
}

int UListBox::deleteString(int nIndex)
{
    return this->sendMsg(LB_DELETESTRING, (WPARAM)nIndex, 0);
}

int UListBox::setColumnWidth(DWORD pixels)
{
    this->sendMsg(LB_SETHORIZONTALEXTENT, 600);
    return this->sendMsg(LB_SETCOLUMNWIDTH, pixels);
}

