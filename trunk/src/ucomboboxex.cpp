#define _WIN32_WINNT 0x0501
#define _WIN32_IE    0x0500

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ucomboboxex.h"

UComboBoxEx::UComboBoxEx(HWND hParent,
                         UINT nResource,
                         HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{}

UComboBoxEx::~UComboBoxEx()
{}

BOOL UComboBoxEx::create()
{
    INITCOMMONCONTROLSEX icex;

    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_USEREX_CLASSES;

    InitCommonControlsEx(&icex);
    
    return UControl::create(_T("ComboBoxEx32"));
}

bool UComboBoxEx::addItem(int nIndex, LPTSTR sText, int cchTextMax)
{
    COMBOBOXEXITEM cbei;
    ::ZeroMemory(&cbei, sizeof(COMBOBOXEXITEM));
    // Set the mask common to all items.
    cbei.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
    cbei.iItem = nIndex;
    cbei.pszText = sText;
    cbei.cchTextMax = cchTextMax;
    cbei.iImage = 0;
    cbei.iSelectedImage = 0;
    
    int nRet = this->sendMsg(CBEM_INSERTITEM, 0, (LPARAM)&cbei);
    return (-1 != nRet);
}

HWND UComboBoxEx::getCombo()
{
    return (HWND)this->sendMsg(CBEM_GETCOMBOCONTROL);
}

void UComboBoxEx::setImageList(HIMAGELIST himl)
{
    this->sendMsg(CBEM_SETIMAGELIST, 0, (LPARAM)himl);
}

//
BOOL UComboBoxEx::setCurSel(const int nIndex/*=0*/)
{
    return sendMsg(CB_SETCURSEL, nIndex, 0);
}

//
int UComboBoxEx::getCurSel()
{
    return sendMsg(CB_GETCURSEL, 0, 0);
}

