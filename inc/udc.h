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

    HANDLE selectObj(HANDLE hObj);

    //
    huys::Color setBKColor(huys::Color clr);
    huys::Color getBKColor();
    int setBKMode(int iMode);
    int getBKMode();

    huys::Color setTextColor(huys::Color clr);
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
    
    BOOL patBlt(
        int nLeft,
        int nTop,
        int nWidth,
        int nHeight,
        DWORD dwRop);

    // Mapping
    /*
        MM_ANISOTROPIC
        MM_HIENGLISH
        MM_HIMETRIC
        MM_ISOTROPIC
        MM_LOENGLISH
        MM_LOMETRIC
        MM_TEXT
        MM_TWIPS
     */
    int getMapMode();
    void setMapMode(int nMode);

    // Viewport
    void setViewportOrg(int x, int y);

    //
    void setWindowOrg(int x, int y);

    //
    BOOL rectangle(int nLeft, int nTop, int nRight, int nBottom);
    BOOL rectangle(LPCRECT lpRect);
    //
    int fillRect(LPCRECT lpRect, HBRUSH hBrush);

    //
    int frameRect(LPCRECT lpRect, HBRUSH hBrush);

    //
    BOOL invertRect(LPCRECT lpRect);

    //
    BOOL roundRect(int nLeft, int nTop, int nRight, int nBottom, int nWidth, int nHeight);

    //
    BOOL DPToLP(LPPOINT lpPoints, int nCount);
    BOOL LPToDP(LPPOINT lpPoints, int nCount);

    //
    BOOL drawLine(int nX1, int nY1, int nX2, int nY2);

    //
    BOOL textOut(int nX, int nY, LPCTSTR lpString, int cbString);

    // uFormat :
    //   DT_BOTTOM
    //   DT_CALCRECT
    //   DT_CENTER
    //   DT_END_ELLIPSIS
    //   DT_EXPANDTABS
    //   DT_INTERNAL
    //   DT_LEFT
    //   DT_NOCLIP
    //   DT_NOPREFIX
    //   DT_RIGHT
    //   DT_RTLREADING
    //   DT_SINGLELINE
    //   DT_TABSTOP
    //   DT_TOP
    //   DT_VCENTER
    //   DT_WORD_ELLIPSIS
    //   DT_WORDBREAK
    int drawText(LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat);
    int drawTextEx(LPTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat, LPDRAWTEXTPARAMS lpDTParams = NULL);

    //
    BOOL extTextOut(int x, int y, UINT fuOptions, LPRECT lpRect, LPCTSTR lpchText, int nCount, const INT *lpDx);
private:
    HBRUSH m_hOldBrush;
    HPEN m_hOldPen;
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

class ULIB_API UPrivateDC : public UDevContext
{
public:
    UPrivateDC(HWND hWnd);
    ~UPrivateDC();
private:
    HWND m_hWnd;
};

class ULIB_API USmartDC : public UDevContext
{
public:
    USmartDC(HDC hdc);
    ~USmartDC();
};

class ULIB_API UMemDC : public UDevContext
{
public:
    UMemDC(HDC hdc);
    ~UMemDC();
};

#endif // U_DEVICE_CONTEXT_H

