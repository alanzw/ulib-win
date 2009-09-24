#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>
#include "ubitmap.h"

#ifndef ASSERT
#define ASSERT assert
#endif

UBitmap::UBitmap()
: m_hBitmap(NULL)
{}

UBitmap::UBitmap(const TCHAR *path)
{
    this->loadFromFile(path);
}

UBitmap::UBitmap(UINT nResID, HINSTANCE hInst)
{
    this->loadFromResource(nResID, hInst);
}

UBitmap::~UBitmap()
{
    if (m_hBitmap)
    {
        ::DeleteObject( m_hBitmap );
    }

    if (m_hPalette)
    {
        ::DeleteObject( m_hPalette );
    }
}

BOOL UBitmap::loadFromFile(const TCHAR *path)
{
    m_hBitmap = (HBITMAP)LoadImage(
        NULL,
        path,
        IMAGE_BITMAP,
        0,
        0,
        LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

    //
    ASSERT(m_hBitmap);

    // Get the color depth of the DIBSection
   ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bi );

   this->m_Height = m_bi.bmHeight;
   this->m_Width  = m_bi.bmWidth;

    // If the DIBSection is 256 color or less, it has a color table
    if( ( m_bi.bmBitsPixel * m_bi.bmPlanes ) <= 8 )
    {
        HDC           hMemDC;
        HBITMAP       hOldBitmap;
        RGBQUAD       rgb[256];
        LPLOGPALETTE  pLogPal;
        WORD          i;

        // Create a memory DC and select the DIBSection into it
        hMemDC = ::CreateCompatibleDC( NULL );
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, m_hBitmap );
        // Get the DIBSection's color table
        ::GetDIBColorTable( hMemDC, 0, 256, rgb );
        // Create a palette from the color tabl
        pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
        pLogPal->palVersion = 0x300;
        pLogPal->palNumEntries = 256;
        for(i=0;i<256;i++)
        {
            pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
            pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
            pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
            pLogPal->palPalEntry[i].peFlags = 0;
        }
        m_hPalette = CreatePalette( pLogPal );
        // Clean up
        free( pLogPal );
        SelectObject( hMemDC, hOldBitmap );
        DeleteDC( hMemDC );
    }
    else   // It has no color table, so use a halftone palette
    {
        HDC    hRefDC;

        hRefDC = GetDC( NULL );
        m_hPalette = CreateHalftonePalette( hRefDC );
        ReleaseDC( NULL, hRefDC );
    }
   return TRUE;
}

BOOL UBitmap::loadFromResource(UINT nResID, HINSTANCE hInst/*= NULL*/)
{
    m_hBitmap = (HBITMAP)LoadImage(
        hInst,
        MAKEINTRESOURCE(nResID),
        IMAGE_BITMAP,
        0,
        0,
        LR_CREATEDIBSECTION);

    //
    ASSERT(m_hBitmap);

    // Get the color depth of the DIBSection
   ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bi );

   this->m_Height = m_bi.bmHeight;
   this->m_Width  = m_bi.bmWidth;

    // If the DIBSection is 256 color or less, it has a color table
    if( ( m_bi.bmBitsPixel * m_bi.bmPlanes ) <= 8 )
    {
        HDC           hMemDC;
        HBITMAP       hOldBitmap;
        RGBQUAD       rgb[256];
        LPLOGPALETTE  pLogPal;
        WORD          i;

        // Create a memory DC and select the DIBSection into it
        hMemDC = CreateCompatibleDC( NULL );
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, m_hBitmap );
        // Get the DIBSection's color table
        GetDIBColorTable( hMemDC, 0, 256, rgb );
        // Create a palette from the color tabl
        pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
        pLogPal->palVersion = 0x300;
        pLogPal->palNumEntries = 256;
        for(i=0;i<256;i++)
        {
            pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
            pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
            pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
            pLogPal->palPalEntry[i].peFlags = 0;
        }
        m_hPalette = CreatePalette( pLogPal );
        // Clean up
        free( pLogPal );
        SelectObject( hMemDC, hOldBitmap );
        DeleteDC( hMemDC );
    }
    else   // It has no color table, so use a halftone palette
    {
        HDC    hRefDC;

        hRefDC = GetDC( NULL );
        m_hPalette = CreateHalftonePalette( hRefDC );
        ReleaseDC( NULL, hRefDC );
    }
   return TRUE;
}

BOOL UBitmap::show( HDC &hdc,
                    RECT &rc)
{
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hBitmap);
    m_hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);
    ::RealizePalette(hdc);

    ::BitBlt( hdc,
              rc.left,
              rc.top,
              m_Width,
              m_Height,
              hMemDC,
              0,
              0,
              SRCCOPY );
    SelectObject(hMemDC, m_hOldBitmap);
    //DeleteObject( m_hBitmap );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
    return TRUE;
}

BOOL UBitmap::showStretch( HDC &hdc, RECT &rc)
{
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hBitmap);
    m_hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);
    ::RealizePalette(hdc);
    ::SetStretchBltMode(hdc, HALFTONE);
    ::StretchBlt(hdc,
        rc.left,
        rc.top,
        rc.right-rc.left,
        rc.bottom-rc.top,
        hMemDC,
        0,
        0,
        m_Width,
        m_Height,
        SRCCOPY);
    SelectObject(hMemDC, m_hOldBitmap);
    //DeleteObject( m_hBitmap );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
    return TRUE;
}

/*
BOOL TransparentBlt(
  __in  HDC hdcDest,
  __in  int xoriginDest,
  __in  int yoriginDest,
  __in  int wDest,
  __in  int hDest,
  __in  HDC hdcSrc,
  __in  int xoriginSrc,
  __in  int yoriginSrc,
  __in  int wSrc,
  __in  int hSrc,
  __in  UINT crTransparent
);
*/

BOOL UBitmap::showTransparent(HDC &hdc, RECT &rc)
{
#if (WINVER >= 0x0410)
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hBitmap);
    m_hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);
    //::RealizePalette(hdc);
    //::SetStretchBltMode(hdc, HALFTONE);
    ::TransparentBlt(hdc,
        rc.left,
        rc.top,
        rc.right-rc.left,
        rc.bottom-rc.top,
        hMemDC,
        0,
        0,
        m_Width,
        m_Height,
        SRCCOPY);
    SelectObject(hMemDC, m_hOldBitmap);
    //DeleteObject( m_hBitmap );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
#endif // (WINVER >= 0x0410)
    return TRUE;
}

BOOL UBitmap::createMappedBitmap(HINSTANCE hInst, UINT uImageID, huys::Color clrFrom, huys::Color clrTo )
{
    assert(NULL == m_hBitmap);
    COLORMAP clrMap = {clrFrom, clrTo};
    m_hBitmap = ::CreateMappedBitmap(hInst, uImageID, IMAGE_BITMAP, &clrMap, 1);

    if (NULL == m_hBitmap)
    {
        return FALSE;
    }


    // Get the color depth of the DIBSection
    ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bi );

    this->m_Height = m_bi.bmHeight;
    this->m_Width  = m_bi.bmWidth;

    return TRUE;
}

BOOL UBitmap::copyImage( HBITMAP hBitmap )
{
    return TRUE;
}

BOOL UBitmap::destroyBitmap()
{
    if (m_hBitmap)
    {
        ::DeleteObject( m_hBitmap );
    }

    if (m_hPalette)
    {
        ::DeleteObject( m_hPalette );
    }
    return TRUE;
}
