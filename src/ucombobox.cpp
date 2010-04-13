#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "ucombobox.h"

UComboBox::UComboBox(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{
    m_dwStyles |= CBS_DROPDOWN;
}

UComboBox::UComboBox()
: UControl()
{}

UComboBox::UComboBox(UBaseWindow *pWndParent, UINT nID)
: UControl(pWndParent, nID)
{
    m_dwStyles |= CBS_DROPDOWN;
}


UComboBox::~UComboBox()
{}

BOOL UComboBox::create()
{
    return UControl::create(_T("COMBOBOX"));
}

//
BOOL UComboBox::setCurSel(const int nIndex/*=0*/)
{
    return sendMsg(CB_SETCURSEL, nIndex, 0);
}

//
int UComboBox::getCurSel()
{
    return sendMsg(CB_GETCURSEL, 0, 0);
}

//
BOOL UComboBox::addText(const TCHAR *lpszText)
{
    int index = 0;
    index = sendMsg(CB_ADDSTRING, 0, (LPARAM)lpszText);

    if (CB_ERR == index)
    {
        return FALSE;
    }
    return TRUE;
}

int UComboBox::getText(TCHAR *lpszText)
{
    return this->getWindowText(lpszText, 256);
}

BOOL UComboBox::insertText(int nIndex, const TCHAR *lpszText)
{
    LONG lResult = sendMsg(CB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)lpszText);
    if (CB_ERR == lResult)
    {
        return FALSE;
    }
    return TRUE;
}

BOOL UComboBox::getLBText(int nIndex, TCHAR *lpszText)
{
    LONG lResult = sendMsg(CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)lpszText);
    if (CB_ERR == lResult)
    {
        return FALSE;
    }
    return TRUE;   
}

BOOL UComboBox::setItemData(int nIndex, DWORD dwData)
{
    return this->sendMsg(CB_SETITEMDATA, (WPARAM)nIndex, (LPARAM)dwData);
}

DWORD UComboBox::getItemData(int nIndex)
{
    return this->sendMsg(CB_GETITEMDATA, (WPARAM)nIndex);
}
