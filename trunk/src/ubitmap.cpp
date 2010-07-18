#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>
#include "ubitmap.h"
#include "udc.h"

#ifndef ASSERT
#define ASSERT assert
#endif

UBitmap::UBitmap()
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
    if (m_hPalette)
    {
        ::DeleteObject( m_hPalette );
    }
}

BOOL UBitmap::create(int nWidth, int nHeight, UINT cPlanes, UINT cBitsPerPel, const void * lpvBits)
{
    m_hObj = ::CreateBitmap(nWidth, nHeight, cPlanes, cBitsPerPel, lpvBits);

    return NULL != m_hObj;
}

HBITMAP UBitmap::createCompatibleBitmap(HDC hdc, int w, int h)
{
    return (HBITMAP)(m_hObj = ::CreateCompatibleBitmap(hdc, w, h));
}

BOOL UBitmap::loadFromFile(const TCHAR *path)
{
    m_hObj = (HBITMAP)LoadImage(
        NULL,
        path,
        IMAGE_BITMAP,
        0,
        0,
        LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

    //
    ASSERT(m_hObj);

    // Get the color depth of the DIBSection
   ::GetObject(m_hObj, sizeof(BITMAP), &m_bi );

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
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, m_hObj );
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

BOOL UBitmap::loadFromResource(UINT nResID, HINSTANCE hInst/*= NULL*/, int cx/*=0*/, int cy/*=0*/)
{
    m_hObj = (HBITMAP)LoadImage(
        hInst,
        MAKEINTRESOURCE(nResID),
        IMAGE_BITMAP,
        cx,
        cy,
        LR_CREATEDIBSECTION);

    //
    ASSERT(m_hObj);

    // Get the color depth of the DIBSection
   ::GetObject(m_hObj, sizeof(BITMAP), &m_bi );

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
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, m_hObj );
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
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hObj);
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
    //DeleteObject( m_hObj );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
    return TRUE;
}

BOOL UBitmap::show( HDC hdc,
               int nXOriginDest,
               int nYOriginDest,
               int nWidthDest,
               int nHeightDest,
               int nXOriginSrc,
               int nYOriginSrc)
{
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hObj);
    m_hOldPalette = ::SelectPalette(hdc, m_hPalette, FALSE);
    ::RealizePalette(hdc);

    ::BitBlt( hdc,
              nXOriginDest,
              nXOriginDest,
              nWidthDest,
              nHeightDest,
              hMemDC,
              nXOriginSrc,
              nYOriginSrc,
              SRCCOPY );
    SelectObject(hMemDC, m_hOldBitmap);
    //DeleteObject( m_hObj );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
    return TRUE;
}

BOOL UBitmap::showStretch( HDC &hdc, RECT &rc)
{
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hObj);
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
    //DeleteObject( m_hObj );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
    return TRUE;
}

/*
BOOL TransparentBlt(
  __in  HDC hdc,
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

BOOL UBitmap::showTransparent(HDC &hdc, RECT &rc, UINT crTransparent)
{
#if (WINVER >= 0x0410)
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hObj);
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
        crTransparent);
    SelectObject(hMemDC, m_hOldBitmap);
    //DeleteObject( m_hObj );
    SelectPalette(hdc, m_hOldPalette, FALSE);
    //DeleteObject( m_hPalette );
#endif // (WINVER >= 0x0410)
    return TRUE;
}

BOOL UBitmap::showTransparentEx(HDC hdc, LPRECT lpRect, UINT crTransparent)
{
    int nWidthDest = lpRect->right - lpRect->left;
    int nHeightDest = lpRect->bottom - lpRect->top;

    HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdc, nWidthDest, nHeightDest);    // 创建兼容位图
    HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);            // 创建单色掩码位图
    HDC hImageDC = CreateCompatibleDC(hdc);//临时DC
    HDC hMaskDC = CreateCompatibleDC(hdc);//临时掩码DC
    hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
    hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

    // 将源DC中的位图拷贝到临时DC中,源DC已经载入位图
    //BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
    ::SelectObject(hImageDC, m_hObj);

    // 设置临时DC的透明色
    SetBkColor(hImageDC, crTransparent);

    // 生成透明区域为白色，其它区域为黑色的临时掩码DC的掩码位图
    // 位图来自临时DC
    BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

    //
    SetBkColor(hImageDC, RGB(0,0,0));
    SetTextColor(hImageDC, RGB(255,255,255));
    BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

    //
    SetBkColor(hdc,RGB(255,255,255));
    SetTextColor(hdc,RGB(0,0,0));
    BitBlt(hdc, lpRect->left, lpRect->top, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

    //
    BitBlt(hdc, lpRect->left, lpRect->top, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);

    //
    SelectObject(hImageDC, hOldImageBMP);
    DeleteDC(hImageDC);
    SelectObject(hMaskDC, hOldMaskBMP);
    DeleteDC(hMaskDC);
    DeleteObject(hImageBMP);
    DeleteObject(hMaskBMP);

    return TRUE;
}

BOOL UBitmap::showTransparentMask(HDC hdc, LPRECT lpRect, HBITMAP mask, UINT crTransparent)
{
    int nWidthDest = lpRect->right - lpRect->left;
    int nHeightDest = lpRect->bottom - lpRect->top;

    HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdc, nWidthDest, nHeightDest);
    HDC hImageDC = CreateCompatibleDC(hdc);
    hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
    HDC hMaskDC = CreateCompatibleDC(hdc);//临时掩码DC



    //
    //BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
    SelectObject(hImageDC, m_hObj);
    SelectObject(hMaskDC, mask);

     // 设置临时DC的透明色
    SetBkColor(hImageDC, crTransparent);
    // 生成透明区域为黑色，其它区域保持不变的位图
    SetBkColor(hImageDC, RGB(0,0,0));
    SetTextColor(hImageDC, RGB(255,255,255));
    BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
    // 透明部分保持屏幕不变，其它部分变成黑色
    SetBkColor(hdc,RGB(255,255,255));
    SetTextColor(hdc,RGB(0,0,0));
    BitBlt(hdc, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
     // "或"运算,生成最终效果
    BitBlt(hdc, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
    // 清理、恢复
    SelectObject(hImageDC, hOldImageBMP);
    DeleteDC(hImageDC);
    DeleteObject(hImageBMP);

    return TRUE;
}

BOOL UBitmap::drawImage(HDC hdc, int x, int y, int nX, int nY, int nCol, int nRow)
{
    //nX-=1;
    //nY-=1;
    //
    int w = getWidth()/nCol;
    int h = getHeight()/nRow;

    HDC hMemDC = ::CreateCompatibleDC(hdc);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hObj );

    ::StretchBlt( hdc,
                  x,
                  y,
                  w,
                  h,
                  hMemDC,
                  w*nX,
                  h*nY,
                  w,
                  h,
                  SRCCOPY );

    ::SelectObject(hMemDC, m_hOldBitmap);


    return TRUE;
}

BOOL UBitmap::createMappedBitmap(HINSTANCE hInst, UINT uImageID, huys::Color clrFrom, huys::Color clrTo )
{
    assert(NULL == m_hObj);
    COLORMAP clrMap = {clrFrom, clrTo};
    m_hObj = ::CreateMappedBitmap(hInst, uImageID, IMAGE_BITMAP, &clrMap, 1);

    if (NULL == m_hObj)
    {
        return FALSE;
    }


    // Get the color depth of the DIBSection
    ::GetObject(m_hObj, sizeof(BITMAP), &m_bi );

    this->m_Height = m_bi.bmHeight;
    this->m_Width  = m_bi.bmWidth;

    return TRUE;
}

HBITMAP UBitmap::copyImage( HBITMAP hbm )
{
    HDC hdcSrc = ::CreateCompatibleDC(NULL);
    HDC hdcDst = ::CreateCompatibleDC(NULL);
    HBITMAP hbmOld, hbmOld2, hbmNew;
    BITMAP bm;
    ::GetObject(hbm, sizeof(bm), &bm);
    hbmOld = (HBITMAP)::SelectObject(hdcSrc, hbm);
    hbmNew = ::CreateBitmap( bm.bmWidth, bm.bmHeight, 
        bm.bmPlanes, bm.bmBitsPixel, NULL);
    hbmOld2 = (HBITMAP)::SelectObject(hdcDst, hbmNew);
    ::BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
    ::SelectObject(hdcSrc, hbmOld);
    ::DeleteDC(hdcSrc);
    ::DeleteDC(hdcDst);
    
    return hbmNew;
}

HBITMAP UBitmap::copyIcon(HICON hIcon, huys::Color clrBk)
{
    //HDC hdcSrc = ::CreateCompatibleDC(NULL);
    //HDC hdcMask = ::CreateCompatibleDC(NULL);
    HDC hdcDst = ::CreateCompatibleDC(NULL);
    //HBITMAP hbmOld;
    HBITMAP hbmOld2, hbmNew;
    BITMAP bm;
    HBITMAP hbm;
    ICONINFO iif = {0};
    ::GetIconInfo(hIcon, &iif);
    
    hbm = iif.hbmColor;
    
    ::GetObject(hbm, sizeof(bm), &bm);
    
    //hbmOld = (HBITMAP)::SelectObject(hdcSrc, hbm);
    hbmNew = ::CreateBitmap( bm.bmWidth, bm.bmHeight, 
        bm.bmPlanes, bm.bmBitsPixel, NULL);
    hbmOld2 = (HBITMAP)::SelectObject(hdcDst, hbmNew);
    //::BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
    
    //HBITMAP hbmOld3 = (HBITMAP)::SelectObject(hdcMask, iif.hbmMask);
    //::BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcMask, 0, 0, SRCAND);
    RECT rect = {0, 0, bm.bmWidth, bm.bmHeight};
    ::SetBkColor(hdcDst, clrBk);
    ::ExtTextOut(hdcDst, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
    ::DrawIcon(hdcDst, 0, 0, hIcon);
    
    //::SelectObject(hdcSrc, hbmOld);
    //::SelectObject(hdcMask, hbmOld2);
    //::DeleteDC(hdcSrc);
    ::DeleteDC(hdcDst);
    
    return hbmNew;
}

BOOL UBitmap::destroyBitmap()
{
    if (m_hObj)
    {
        ::DeleteObject( m_hObj );
    }

    if (m_hPalette)
    {
        ::DeleteObject( m_hPalette );
    }
    return TRUE;
}

HRGN UBitmap::convert2Rgn(COLORREF cTransparentColor, COLORREF cTolerance)
{
    ASSERT(NULL != m_hObj);
    HBITMAP hBmp = *this;
    HRGN hRgn = NULL;

    if(hBmp)
    {
        // Create a memory DC inside which we will scan the bitmap content
        HDC hMemDC = CreateCompatibleDC(NULL);
        if(hMemDC)
        {
            // Get bitmap size
            BITMAP bm;
            ::GetObject(hBmp, sizeof(bm), &bm);

            // Create a 32 bits depth bitmap and select it into the memory DC
            BITMAPINFOHEADER RGB32BITSBITMAPINFO = {
                    sizeof(BITMAPINFOHEADER),    // biSize
                    bm.bmWidth,                    // biWidth;
                    bm.bmHeight,                // biHeight;
                    1,                            // biPlanes;
                    32,                            // biBitCount
                    BI_RGB,                        // biCompression;
                    0,                            // biSizeImage;
                    0,                            // biXPelsPerMeter;
                    0,                            // biYPelsPerMeter;
                    0,                            // biClrUsed;
                    0                            // biClrImportant;
            };
            VOID * pbits32;
            HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
            if(hbm32)
            {
                HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

                // Create a DC just to copy the bitmap into the memory DC
                HDC hDC = CreateCompatibleDC(hMemDC);
                if(hDC)
                {
                    // Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
                    BITMAP bm32;
                    ::GetObject(hbm32, sizeof(bm32), &bm32);
                    while(bm32.bmWidthBytes % 4)
                        bm32.bmWidthBytes++;

                    // Copy the bitmap into the memory DC
                    HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
                    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

                    // For better performances, we will use the ExtCreateRegion() function to create the
                    // region. This function take a RGNDATA structure on entry. We will add rectangles by
                    // amount of ALLOC_UNIT number in this structure.
                    #define ALLOC_UNIT    100
                    DWORD maxRects = ALLOC_UNIT;
                    HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
                    RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
                    pData->rdh.dwSize = sizeof(RGNDATAHEADER);
                    pData->rdh.iType = RDH_RECTANGLES;
                    pData->rdh.nCount = pData->rdh.nRgnSize = 0;
                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

                    // Keep on hand highest and lowest values for the "transparent" pixels
                    BYTE lr = GetRValue(cTransparentColor);
                    BYTE lg = GetGValue(cTransparentColor);
                    BYTE lb = GetBValue(cTransparentColor);
                    BYTE hr = min(0xff, lr + GetRValue(cTolerance));
                    BYTE hg = min(0xff, lg + GetGValue(cTolerance));
                    BYTE hb = min(0xff, lb + GetBValue(cTolerance));

                    // Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
                    BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
                    for(int y = 0; y < bm.bmHeight; y++)
                    {
                        // Scan each bitmap pixel from left to right
                        for(int x = 0; x < bm.bmWidth; x++)
                        {
                            // Search for a continuous range of "non transparent pixels"
                            int x0 = x;
                            LONG *p = (LONG *)p32 + x;
                            while(x < bm.bmWidth)
                            {
                                BYTE b = GetRValue(*p);
                                if(b >= lr && b <= hr)
                                {
                                    b = GetGValue(*p);
                                    if(b >= lg && b <= hg)
                                    {
                                        b = GetBValue(*p);
                                        if(b >= lb && b <= hb)
                                            // This pixel is "transparent"
                                            break;
                                    }
                                }
                                p++;
                                x++;
                            }

                            if(x > x0)
                            {
                                // Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
                                if(pData->rdh.nCount >= maxRects)
                                {
                                    GlobalUnlock(hData);
                                    maxRects += ALLOC_UNIT;
                                    hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
                                    pData = (RGNDATA *)GlobalLock(hData);
                                }
                                RECT *pr = (RECT *)&pData->Buffer;
                                SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
                                if(x0 < pData->rdh.rcBound.left)
                                    pData->rdh.rcBound.left = x0;
                                if(y < pData->rdh.rcBound.top)
                                    pData->rdh.rcBound.top = y;
                                if(x > pData->rdh.rcBound.right)
                                    pData->rdh.rcBound.right = x;
                                if(y+1 > pData->rdh.rcBound.bottom)
                                    pData->rdh.rcBound.bottom = y+1;
                                pData->rdh.nCount++;

                                // On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
                                // large (ie: > 4000). Therefore, we have to create the region by multiple steps.
                                if(pData->rdh.nCount == 2000)
                                {
                                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
                                    if(hRgn)
                                    {
                                        CombineRgn(hRgn, hRgn, h, RGN_OR);
                                        ::DeleteObject(h);
                                    }
                                    else
                                        hRgn = h;
                                    pData->rdh.nCount = 0;
                                    SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
                                }
                            }
                        }

                        // Go to next row (remember, the bitmap is inverted vertically)
                        p32 -= bm32.bmWidthBytes;
                    }

                    // Create or extend the region with the remaining rectangles
                    HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
                    if(hRgn)
                    {
                        CombineRgn(hRgn, hRgn, h, RGN_OR);
                        ::DeleteObject(h);
                    }
                    else
                        hRgn = h;

                    // Clean up
                    GlobalFree(hData);
                    SelectObject(hDC, holdBmp);
                    DeleteDC(hDC);
                }

                ::DeleteObject(SelectObject(hMemDC, holdBmp));
            }

            DeleteDC(hMemDC);
        }
    }

    return hRgn;
}

BOOL UBitmap::showExtend(HDC hdc, LPRECT lpRect, int nX, int nY)
{
    HDC hMemDC;
    LONG width = lpRect->right - lpRect->left;
    LONG height = lpRect->bottom - lpRect->top;

    hMemDC = ::CreateCompatibleDC(hdc);
    m_hObj = ::CreateCompatibleBitmap(hdc, width, height);
    m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_hObj);

    if (nX==0 && nY==0)
    {
        showStretch(hMemDC, *lpRect);

        return TRUE;
    }

    HDC hdc2 = ::CreateCompatibleDC(hMemDC);
    HBITMAP hOldBitmap2 = (HBITMAP)::SelectObject(hdc2, m_hObj);

    //dc.SetStretchBltMode(COLORONCOLOR);
    if (nX!=0 && nY==0)
    {

        //upper left
        ::BitBlt(hMemDC, 0, 0, nX, height, hdc2, 0, 0,
                SRCCOPY );
        ::StretchBlt(hMemDC, nX, 0, width-getWidth(), height ,  hdc2,nX, 0, 1, getHeight(),
                SRCCOPY );
        //upper right
        ::BitBlt(hMemDC, lpRect->right-(getWidth()-nX), 0, getWidth()-nX, height ,  hdc2,nX, 0,
                SRCCOPY );

    }
    else if (nX==0 && nY!=0)
    {
        //upper left
        ::BitBlt(hMemDC, 0, 0, width, nY, hdc2, 0, 0,
                SRCCOPY );
        ::StretchBlt(hMemDC, 0, nY, getWidth(), height-getHeight(),   hdc2,0, nY, getWidth(), 1,
                SRCCOPY );
        //upper right
        ::BitBlt(hMemDC, 0, lpRect->bottom-(getHeight()-nY), getWidth(), getHeight()-nY ,  hdc2,0, nY,
                SRCCOPY );

    }
    else
    {
        //upper left
        ::StretchBlt(hMemDC,  0, 0, nX, nY, hdc2, 0, 0, nX, nY ,
                SRCCOPY );
        //upper middle
        ::StretchBlt(hMemDC, nX, 0, width-getWidth(),nY, hdc2, nX, 0, 1, nY ,
                SRCCOPY );
        //upper right
        ::StretchBlt(hMemDC, width-(getWidth()-nX), 0, getWidth()-nX, nY ,  hdc2,nX, 0, getWidth()-nX, nY,
                SRCCOPY );
        //left middle
        ::StretchBlt(hMemDC, 0, nY, nX,height-getHeight(), hdc2, 0, nY, nX, 1 ,
                SRCCOPY );
        //center
        ::StretchBlt(hMemDC, nX, nY, width-getWidth(),height-getHeight(), hdc2, nX, nY, 1, 1 ,
                SRCCOPY );
        //right middle
        ::StretchBlt(hMemDC, width-(getWidth()-nX), nY, getWidth()-nX,height-getHeight(), hdc2, nX, nY, getWidth()-nX, 1 ,
                SRCCOPY );

        //bottom left
        ::StretchBlt(hMemDC,  0, height-(getHeight()-nY), nX, getHeight()-nY, hdc2, 0, nY, nX,getHeight()-nY ,
                SRCCOPY );
        //bottom middle
        ::StretchBlt(hMemDC, nX, height-(getHeight()-nY), width-getWidth(),getHeight()-nY, hdc2, nX, nY, 1, getHeight()-nY ,
                SRCCOPY );
        //bottom right
        ::StretchBlt(hMemDC,  width-(getWidth()-nX), height-(getHeight()-nY), getWidth()-nX, getHeight()-nY, hdc2, nX, nY, getWidth()-nX,getHeight()-nY ,
                SRCCOPY );
    }
    ::SelectObject(hdc2, hOldBitmap2);
    ::SelectObject(hMemDC, m_hOldBitmap);

    return TRUE;
}

