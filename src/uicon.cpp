#include <windows.h>
#include <cassert>
#include "uicon.h"

UIcon::UIcon()
{}

UIcon::~UIcon()
{
    if (NULL != m_hObj)
    {
        destroy();
    }
}

BOOL UIcon::loadStandard(LPCTSTR lpIconName)
{
    m_hObj=::LoadIcon(NULL, lpIconName);

    assert(m_hObj!=NULL);

    if (!m_hObj)
        return FALSE;
    return TRUE;
}

BOOL UIcon::draw( HWND hwnd, const int x, const int y )
{
    HDC hdc;

    if ((hdc = ::GetDC(hwnd)) == NULL)
        return FALSE;

    ::DrawIcon(hdc, x, y, (HICON)m_hObj);

    ::ReleaseDC(hwnd, hdc);

    return TRUE;
}

BOOL UIcon::draw( HWND hwnd, const POINT *pt )
{
    return draw(hwnd, pt->x, pt->y);
}

BOOL UIcon::drawEx(HDC hdc, int x, int y, int cx, int cy)
{
    return ::DrawIconEx(hdc, x, y, (HICON)m_hObj, cx, cy, 0, NULL, DI_NORMAL | DI_COMPAT);
}

HICON UIcon::getHICON()
{
    return (HICON)m_hObj;
}

BOOL UIcon::loadIconEx( HINSTANCE hInstance, LPCTSTR lpIconName )
{
    m_hObj=::LoadIcon(hInstance, lpIconName);

    assert(m_hObj!=NULL);

    if (!m_hObj)
        return FALSE;
    return TRUE;
}

BOOL UIcon::loadIconEx( HINSTANCE hInstance, UINT nResId )
{
    return this->loadIconEx(hInstance, MAKEINTRESOURCE(nResId));
}

BOOL UIcon::loadImage(HINSTANCE hInstance, UINT nID, int cx, int cy)
{
    m_hObj = (HICON)::LoadImage( hInstance,
                                 MAKEINTRESOURCE(nID),
                                 IMAGE_ICON,
                                 cx,
                                 cy,
                                 0 );

    assert(m_hObj != NULL);
    return TRUE;
}

bool UIcon::destroy()
{
    BOOL ret = ::DestroyIcon((HICON)m_hObj);
    m_hObj = NULL;
    return TRUE == ret;
}

