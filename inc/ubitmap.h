#ifndef U_BITMAP_H
#define U_BITMAP_H

#include "uobject.h"
#include "colors.h"

class ULIB_API UBitmap : public UGDIObject
{
public:
    UBitmap();
    UBitmap(const TCHAR *path);
    UBitmap(UINT nResID, HINSTANCE hInst=NULL);
    virtual ~UBitmap();
    //
    BOOL loadFromFile(const TCHAR *path);
    BOOL loadFromResource(UINT nResID, HINSTANCE hInst=NULL, int cx=0, int cy=0);

    //
    LONG getHeight() const {return m_Height;}
    LONG getWidth() const {return m_Width;}

    //
    operator HBITMAP ()
    {
        return (HBITMAP)m_hObj;
    }

    //
    operator BITMAP ()
    {
        return m_bi;
    }

    //HBITMAP getHandle() {return m_hObj;}

    //
    BOOL create(int nWidth, int nHeight, UINT cPlanes, UINT cBitsPerPel, const void * lpvBits);
    HBITMAP createCompatibleBitmap(HDC hdc, int w, int h);
    //
    BOOL show(HDC &hdc, RECT &rc);
    BOOL show( HDC hdc,
               int nXOriginDest,
               int nYOriginDest,
               int nWidthDest,
               int nHeightDest,
               int  nXOriginSrc,
               int nYOriginSrc);
    BOOL showStretch(HDC &hdc, RECT &rc);
    BOOL showTransparent(HDC &hdc, RECT &rc, UINT crTransparent);
    BOOL showTransparentEx(HDC hdc, LPRECT rc, UINT crTransparent);
    BOOL showTransparentMask(HDC hdc, LPRECT lpRect, HBITMAP mask, UINT crTransparent);
    //
    BOOL drawImage(HDC hdc, int x, int y, int nX, int nY, int nCol, int nRow);

    //
    BOOL showExtend(HDC hdc, LPRECT lpRect, int nX, int nY);

    //
    BOOL createMappedBitmap(HINSTANCE hInst, UINT uImageID, huys::Color clrFrom, huys::Color clrTo);
    BOOL copyImage(HBITMAP hBitmap);

    BOOL destroyBitmap();

    HRGN convert2Rgn(COLORREF cTransparentColor, COLORREF cTolerance);

public:

protected:
    LONG m_Height;
    LONG m_Width;
    //HBITMAP m_hObj;
    BITMAP m_bi;
    HBITMAP m_hOldBitmap;
    HPALETTE m_hPalette;
    HPALETTE m_hOldPalette;
private:
};

#endif // U_BITMAP_H

