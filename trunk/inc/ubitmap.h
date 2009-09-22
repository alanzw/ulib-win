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
    BOOL loadFromResource(UINT nResID, HINSTANCE hInst=NULL);

    //
    LONG getHeight() const {return m_Height;}
    LONG getWidth() const {return m_Width;}

    //
    operator HBITMAP ()
    {
        return m_hBitmap;
    }

    //
    operator BITMAP ()
    {
        return m_bi;
    }

    HBITMAP getHandle() const {return m_hBitmap;}

    //
    BOOL show(HDC &hdc, RECT &rc);
    BOOL showStretch(HDC &hdc, RECT &rc);
    BOOL showTransparent(HDC &hdc, RECT &rc);

    //
    BOOL createMappedBitmap(HINSTANCE hInst, UINT uImageID, huys::Color clrFrom, huys::Color clrTo);
    BOOL copyImage(HBITMAP hBitmap);

    BOOL destroyBitmap();
protected:
    LONG m_Height;
    LONG m_Width;
    HBITMAP m_hBitmap;
    BITMAP m_bi;
    HBITMAP m_hOldBitmap;
    HPALETTE m_hPalette;
    HPALETTE m_hOldPalette;
private:
};

#endif // U_BITMAP_H

