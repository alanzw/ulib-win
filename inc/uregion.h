/*
 * =====================================================================================
 *
 *       Filename:  uregion.h
 *
 *    Description:  region
 *
 *        Version:  1.0
 *        Created:  2009-7-3 2:02:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_REGION_H
#define U_REGION_H

#include "uobject.h"

class ULIB_API URegion : public UGDIObject
{
public:
    URegion();
    ~URegion();

    ///
    operator HRGN ()
    {
        return (HRGN)m_hObj;
    }

    ///
    bool createRect(int left, int top, int right, int bottom);

    bool createRectIndirect(const LPRECT lpRect);

    bool createRoundRect(int left, int top, int right, int bottom, int w, int h);

    //
    bool createElliptic(int left, int top, int right, int bottom);
    bool createEllipticIndirect(const LPRECT lpRect);

    //
    bool createPolygon(const POINT *lppt, int cPoints, int fnPolyFillMode);
    bool createPolyPolygon(const POINT *lppt, const INT *lpPolyCounts, int nCount, int fnPolyFillMode);
    //
    bool ptInRgn(int x, int y);

    bool rectInRgn(const LPRECT lpRect);

    int getRgnBox(LPRECT lpRect);

    /*
     * RGN_AND  Creates the intersection of the two combined regions.
     * RGN_COPY Creates a copy of the region identified by hrgnSrc1.
     * RGN_DIFF Combines the parts of hrgnSrc1 that are not part of hrgnSrc2.
     * RGN_OR   Creates the union of two combined regions.
     * RGN_XOR  Creates the union of two combined regions except for any overlapping areas.
     */
    int combine(HRGN hRgnSrc1, HRGN hRgnSrc2, int nMode);

    int combine(URegion &ur, int nMode);
    //
    bool isEqual(HRGN hRgn);

    //
    bool fill(HDC hdc, HBRUSH hBrush);
    bool paint(HDC hdc);
    bool frame(HDC hdc, HBRUSH hbr, int nWidth, int nHeight);

    bool invert(HDC hdc);

    //
    int offset(int x, int y);

    // PolyFillMode
    // ** ALTERNATE
    //      Selects alternate mode
    //      (fills the area between odd-numbered and even-numbered polygon sides on each scan line).
    // ** WINDING
    //      Selects winding mode
    //      (fills any region with a nonzero winding value).
    int getPolyFillMode(HDC hdc);
    int setPolyFillMode(HDC hdc, int nMode);

    //
    DWORD getData(DWORD dwCount, LPRGNDATA lpRgnData);

    //
    URegion & operator = (HRGN hRgn)
    {
        m_hObj = hRgn;
        return *this;
    }

    void setRgn(HRGN hRgn)
    {
        m_hObj = hRgn;
    }

    HRGN bitmapRegion(HBITMAP hBitmap,
                      COLORREF cTransparentColor = RGB(0,0,0),
                      BOOL bIsTransparent=TRUE);
private:
};

#endif // U_REGION_H

