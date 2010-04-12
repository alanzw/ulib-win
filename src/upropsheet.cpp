#include <windows.h>
#include <tchar.h>
#include <prsht.h>

#include "upropsheet.h"

UPropSheet::UPropSheet( HWND hParent, HINSTANCE hInst )
: UControl(hParent, -1, hInst)
{
    ::ZeroMemory(&m_psh, sizeof(PROPSHEETHEADER));
    m_psh.dwSize = sizeof(PROPSHEETHEADER);
    m_psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE;
    m_psh.hwndParent = hParent;
    m_psh.hInstance = hInst;
    m_psh.nStartPage = 0;
    m_psh.pfnCallback = NULL;
}

UPropSheet::~UPropSheet()
{
}

BOOL UPropSheet::create( PROPSHEETPAGE *ppsp, int nPages, LPSTR lpCaption)
{
    m_psh.nPages = nPages;
    m_psh.ppsp = ppsp;
    m_psh.pszCaption = lpCaption;
    ::PropertySheet(&m_psh);
    return TRUE;
}

