#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uincome.h"

UIncome::UIncome(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UIncome::UIncome()
{
}

UIncome::~UIncome()
{}

UIncome::UIncome(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_dwStyles |= SS_NOTIFY;
    m_nResourceID = nID;
}
