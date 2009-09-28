/*
 * =====================================================================================
 *
 *       Filename:  udc.h
 *
 *    Description:  Class UDevContext
 *
 *        Version:  1.0
 *        Created:  2009-8-23 4:45:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_DEVICE_CONTEXT_H
#define U_DEVICE_CONTEXT_H

#include "uobject.h"
#include "colors.h"

class ULIB_API UDevContext : public UGDIObject
{
public:
    UDevContext();
    ~UDevContext();

    operator HDC()
    { return (HDC)m_hObj; }

    bool attach(HDC hdc);
    bool dettach();

    bool getDC(HWND hWnd);

    huys::Color setPenColor(huys::Color clr);
    huys::Color setBrushColor(huys::Color clr);
    huys::Color getPenColor();
    huys::Color getBrushColor();

    HGDIOBJ getCurObj(UINT uObjectType);

    //
    int setStretchBltMode(int iStretchMode);
    BOOL stretchBlt(
        int nXOriginDest,
        int nYOriginDest,
        int nWidthDest,
        int nHeightDest,
        HDC hdcSrc,
        int nXOriginSrc,
        int nYOriginSrc,
        int nWidthSrc,
        int nHeightSrc,
        DWORD dwRop);
    BOOL bitBlt(
        int nXOriginDest,
        int nYOriginDest,
        int nWidthDest,
        int nHeightDest,
        HDC hdcSrc,
        int nXOriginSrc,
        int nYOriginSrc,
        DWORD dwRop);
private:
};

typedef UDevContext UDC;

class ULIB_API UPaintDC : public UDevContext
{
public:
    UPaintDC(HWND hWnd);
    ~UPaintDC();
private:
    HWND m_hWnd;
    PAINTSTRUCT m_ps;
};

class ULIB_API UMemDC : public UDevContext
{
public:
    UMemDC(HDC hdc);
    ~UMemDC();
};

#endif // U_DEVICE_CONTEXT_H

