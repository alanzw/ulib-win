/*
 * =====================================================================================
 *
 *       Filename:  uregion.cpp
 *
 *    Description:  implement
 *
 *        Version:  1.0
 *        Created:  2009-7-3 4:04:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>

#include "uregion.h"

URegion::URegion()
: UGDIObject()
{}

URegion::~URegion()
{

}

bool URegion::createRect(int left, int top, int right, int bottom)
{
    m_hObj = ::CreateRectRgn(left, top, right, bottom);

    return (NULL != m_hObj);
}

bool URegion::createRectIndirect(const LPRECT lpRect)
{
    m_hObj = ::CreateRectRgnIndirect(lpRect);

    return (NULL != m_hObj);
}

bool URegion::createRoundRect(int left, int top, int right, int bottom, int w, int h)
{
    m_hObj = ::CreateRoundRectRgn(left, top, right, bottom, w, h);
    return (NULL != m_hObj);
}

bool URegion::createElliptic( int left, int top, int right, int bottom )
{
    m_hObj = ::CreateEllipticRgn(left, top, right, bottom);
    return (NULL != m_hObj);
}

bool URegion::createEllipticIndirect( const LPRECT lpRect )
{
    m_hObj = ::CreateEllipticRgnIndirect(lpRect);
    return (NULL != m_hObj);
}

bool URegion::createPolygon( const POINT *lppt, int cPoints, int fnPolyFillMode )
{
    m_hObj = ::CreatePolygonRgn(lppt, cPoints, fnPolyFillMode);
    return (NULL != m_hObj);
}

bool URegion::createPolyPolygon( const POINT *lppt, const INT *lpPolyCounts, int nCount, int fnPolyFillMode )
{
    m_hObj = ::CreatePolyPolygonRgn(lppt, lpPolyCounts, nCount, fnPolyFillMode);
    return (NULL != m_hObj);
}

bool URegion::ptInRgn( int x, int y )
{
    return TRUE == ::PtInRegion((HRGN)m_hObj, x, y);
}

bool URegion::rectInRgn( const LPRECT lpRect )
{
    return TRUE == ::RectInRegion((HRGN)m_hObj, lpRect);
}

int URegion::combine(HRGN hRgnSrc1, HRGN hRgnSrc2, int nMode)
{
    return ::CombineRgn((HRGN)m_hObj, hRgnSrc1, hRgnSrc2, nMode);
}

int URegion::combine( URegion &ur, int nMode )
{
    // This region must exist before CombineRgn is called.
    HRGN hRgn = CreateRectRgn (0, 0, 1, 1) ;
    int nRet = ::CombineRgn(hRgn, (HRGN)m_hObj, (HRGN)ur.m_hObj, nMode);
    m_hObj = hRgn;
    return nRet;
}

bool URegion::isEqual(HRGN hRgn)
{
    return TRUE == ::EqualRgn((HRGN)m_hObj, hRgn);
}

bool URegion::fill(HDC hdc, HBRUSH hBrush)
{
    return TRUE == ::FillRgn(hdc, (HRGN)m_hObj, hBrush);
}

bool URegion::paint( HDC hdc )
{
    return TRUE == ::PaintRgn(hdc, (HRGN)m_hObj);
}

int URegion::offset( int x, int y )
{
    return ::OffsetRgn((HRGN)m_hObj, x, y);
}

int URegion::getRgnBox( LPRECT lpRect )
{
    return ::GetRgnBox((HRGN)m_hObj, lpRect);
}

bool URegion::invert( HDC hdc )
{
    return TRUE == ::InvertRgn(hdc, (HRGN)m_hObj);
}

bool URegion::frame( HDC hdc, HBRUSH hbr, int nWidth, int nHeight )
{
    return TRUE == ::FrameRgn(hdc, (HRGN)m_hObj, hbr, nWidth, nHeight);
}

int URegion::getPolyFillMode( HDC hdc )
{
    return ::GetPolyFillMode(hdc);
}

int URegion::setPolyFillMode( HDC hdc, int nMode )
{
    return ::SetPolyFillMode(hdc, nMode);
}

DWORD URegion::getData( DWORD dwCount, LPRGNDATA lpRgnData )
{
    return ::GetRegionData((HRGN)m_hObj, dwCount, lpRgnData);
}
