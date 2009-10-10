#include <windows.h>
#include <tchar.h>

#include "ustatic.h"

#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif

UStatic::UStatic(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst),
  m_pText(0)
{
    m_dwStyles |= SS_SIMPLE;
}

UStatic::UStatic(UBaseWindow *pWndParent, LPCTSTR lpText)
: UControl(pWndParent, IDC_STATIC),
  m_pText(lpText)
{
	m_dwStyles |= SS_SIMPLE;
}


BOOL UStatic::setText( const TCHAR *pText )
{
    m_pText = pText;
    return TRUE;
}

BOOL UStatic::setIcon( UINT nResID )
{
    return this->sendMsg(STM_SETICON, (WPARAM)(HICON)(::LoadIcon(m_hInstance, MAKEINTRESOURCE(nResID))));
}

UStatic::~UStatic()
{}

BOOL UStatic::create()
{
    if (!m_pText)
    {
        m_pText = _T("STATIC");
    }

    return UControl::create(_T("STATIC"), m_pText);
}

HICON UStatic::getIcon()
{
    return (HICON)this->sendMsg(STM_GETICON);
}

BOOL UStatic::setBitmap(HANDLE hImage)
{
    return this->sendMsg(STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImage);
}

HANDLE UStatic::getBitmap()
{
    return (HANDLE)this->sendMsg(STM_GETIMAGE, (WPARAM)IMAGE_BITMAP);
}

