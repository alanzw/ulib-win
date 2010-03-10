#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <errno.h>
#include <math.h>
#include <direct.h>

#include <exception>

#include "udibapi.h"
#include "ufile.h"

//
namespace huys
{

#define DIB_HEADER_MARKER ((WORD)('M'<<8)|'B')

BOOL WINAPI PaintDIB(HDC hDC,
                     LPRECT lpDCRect,
                     HDIB hDIB,
                     LPRECT lpDIBRect,
                     HPALETTE hPal)
{
    LPSTR lpDIBHdr;
    LPSTR lpDIBBits;
    BOOL  bSuccess = FALSE;
    //HPALETTE hPal = NULL;
    HPALETTE hOldPal = NULL;

    //
    if (hDIB == NULL)
    {
        return FALSE;
    }

    //
    lpDIBHdr = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);
    lpDIBBits = FindDIBBits(lpDIBHdr);
    //
    if (hPal != NULL)
    {
        //hPal = (HPALETTE)pPal->m_hObject;
        hOldPal = ::SelectPalette(hDC, hPal, TRUE);
    }

    //
    ::SetStretchBltMode(hDC, COLORONCOLOR);

    //
    if ((RECTWIDTH(lpDCRect) == RECTWIDTH(lpDIBRect)) &&
         (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
    {
        //
        bSuccess = ::SetDIBitsToDevice(hDC,
                                       lpDCRect->left,
                                       lpDCRect->top,
                                       RECTWIDTH(lpDCRect),
                                       RECTHEIGHT(lpDCRect),
                                       lpDIBRect->left,
                                       (int)DIBHeight(lpDIBHdr)-lpDIBRect->top-RECTHEIGHT(lpDIBRect),
                                       0,
                                       (WORD)DIBHeight(lpDIBHdr),
                                       lpDIBBits,
                                       (LPBITMAPINFO)lpDIBHdr,
                                       DIB_RGB_COLORS);
    }
    else
    {
        //
        bSuccess = ::StretchDIBits(hDC,
                                   lpDCRect->left,
                                   lpDCRect->top,
                                   RECTWIDTH(lpDCRect),
                                   RECTHEIGHT(lpDCRect),
                                   lpDIBRect->left,
                                   lpDIBRect->top,
                                   RECTWIDTH(lpDIBRect),
                                   RECTHEIGHT(lpDIBRect),
                                   lpDIBBits,
                                   (LPBITMAPINFO)lpDIBHdr,
                                   DIB_RGB_COLORS,
                                   SRCCOPY);
    }

    ::GlobalUnlock((HGLOBAL)hDIB);

    //
    if (hOldPal != NULL)
    {
        ::SelectPalette(hDC, hOldPal, TRUE);
    }

    return bSuccess;
}

//
BOOL WINAPI CreateDIBPalette(HDIB hDIB, HPALETTE hPal)
{
    //
    LPLOGPALETTE lpPal;

    //
    HANDLE hLogPal;

    //
    //HPALETTE hPal = NULL;

    //
    int i;

    //
    WORD wNumColors;

    //
    LPSTR lpbi;

    //
    LPBITMAPINFO lpbmi;

    //
    LPBITMAPCOREINFO lpbmc;

    //
    BOOL bWinStyleDIB;

    //
    BOOL bResult = FALSE;

    //
    if (hDIB == NULL)
    {
        return FALSE;
    }

    //
    lpbi = (LPSTR)::GlobalLock((HGLOBAL)hDIB);

    //
    lpbmi = (LPBITMAPINFO)lpbi;

    //
    lpbmc = (LPBITMAPCOREINFO)lpbi;

    //
    wNumColors = DIBNumColors(lpbi);

    //
    if (wNumColors != 0)
    {
        //
        hLogPal = ::GlobalAlloc(GHND, sizeof(LPLOGPALETTE)
                                      + sizeof(PALETTEENTRY)
                                       * wNumColors);
        //
        if (hLogPal == 0)
        {
            //
            ::GlobalUnlock((HGLOBAL)hDIB);

            //
            return FALSE;
        }

        lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL)hLogPal);

        //
        lpPal->palVersion = PALVERSION;

        //
        lpPal->palNumEntries = (WORD)wNumColors;

        //
        bWinStyleDIB = IS_WIN30_DIB(lpbi);

        //
        for (i=0; i<(int)wNumColors; i++)
        {
            if (bWinStyleDIB)
            {
                //
                lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;

                //
                lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;

                //
                lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;

                //
                lpPal->palPalEntry[i].peFlags = 0;
            }
            else
            {
                //
                lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;

                //
                lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;

                //
                lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;

                //
                lpPal->palPalEntry[i].peFlags = 0;
            }
        }

        //
        hPal = ::CreatePalette(lpPal);

        //
        ::GlobalUnlock((HGLOBAL)hLogPal);

        //
        ::GlobalFree((HGLOBAL)hLogPal);
    }

    //
    ::GlobalUnlock((HGLOBAL)hDIB);

    return bResult;
}

//
LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
    return (lpbi + *(LPDWORD)lpbi+ PaletteSize(lpbi));
}

//
DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
    //
    LPBITMAPINFOHEADER lpbmi;

    //
    LPBITMAPCOREHEADER lpbmc;

    //
    lpbmi = (LPBITMAPINFOHEADER)lpDIB;
    lpbmc = (LPBITMAPCOREHEADER)lpDIB;

    //
    if (IS_WIN30_DIB(lpDIB))
    {
        //
        return lpbmi->biWidth;
    }
    else
    {
        //
        return (DWORD)lpbmc->bcWidth;
    }
}


//
//
DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
    //
    LPBITMAPINFOHEADER lpbmi;

    //
    LPBITMAPCOREHEADER lpbmc;

    //
    lpbmi = (LPBITMAPINFOHEADER)lpDIB;
    lpbmc = (LPBITMAPCOREHEADER)lpDIB;

    //
    if (IS_WIN30_DIB(lpDIB))
    {
        //
        return lpbmi->biHeight;
    }
    else
    {
        //
        return (DWORD)lpbmc->bcHeight;
    }
}

//
WORD WINAPI PaletteSize(LPSTR lpbi)
{
    //
    if (IS_WIN30_DIB(lpbi))
    {
        //
        return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
    }
    else
    {
        //
        return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
    }
}

DWORD WINAPI BytesPerLine( LPBITMAPINFOHEADER lpBMIH )
{
    return WIDTHBYTES(lpBMIH->biWidth * lpBMIH->biPlanes * lpBMIH->biBitCount);
}

//
WORD WINAPI DIBNumColors(LPSTR lpbi)
{
    WORD wBitCount;

    //
    if (IS_WIN30_DIB(lpbi))
    {
        DWORD dwClrUsed;

        //
        dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;

        if (dwClrUsed != 0)
        {
            //
            return (WORD)dwClrUsed;
        }
    }

    //
    if (IS_WIN30_DIB(lpbi))
    {
        wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
    }
    else
    {
        //
        wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
    }

    switch(wBitCount)
    {
    case 1:
        return 2;
    case 4:
        return 16;
    case 8:
        return 256;
    default:
        return 0;
    }

}

//
HGLOBAL WINAPI CopyHandle(HGLOBAL h)
{
    if (h == NULL)
    {
        return NULL;
    }

    //
    DWORD dwLen = ::GlobalSize((HGLOBAL)h);

    //
    HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

    //
    if (hCopy != NULL)
    {
        //
        void *lpCopy = ::GlobalLock((HGLOBAL)hCopy);
        void *lp = ::GlobalLock((HGLOBAL)h);

        //
        memcpy(lpCopy, lp, dwLen);

        //
        ::GlobalUnlock(hCopy);
        ::GlobalUnlock(h);
    }

    return hCopy;
}

//
void SetMonoDIBPixel( LPBYTE pANDBits, DWORD dwWidth, DWORD dwHeight, DWORD x, DWORD y, BOOL bWhite )
{
    DWORD    ByteIndex;
    BYTE    BitNumber;

    // Find the byte on which this scanline begins
    ByteIndex = (dwHeight - y - 1) * WIDTHBYTES(dwWidth);
    // Find the byte containing this pixel
    ByteIndex += (x >> 3);
    // Which bit is it?
    BitNumber = (BYTE)( 7 - (x % 8) );

    if( bWhite )
        // Turn it on
        pANDBits[ByteIndex] |= (1<<BitNumber);
    else
        // Turn it off
        pANDBits[ByteIndex] &= ~(1<<BitNumber);
}

//
BOOL WINAPI SaveDIB(HDIB hDib, HANDLE hFile)
{
    //
    BITMAPFILEHEADER bmfHeader;

    //
    LPBITMAPINFOHEADER lpBI;

    //
    DWORD dwDIBSize;
    //
    //LPBYTE lpBits;              // memory pointer

    if (hDib==NULL)
    {
        //
        return FALSE;
    }

    //
    lpBI = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);

    if (lpBI == NULL)
    {
        //
        return FALSE;
    }

    //
    if (!IS_WIN30_DIB(lpBI))
    {
        //
        //
        ::GlobalUnlock((HGLOBAL)hDib);

        //
        return FALSE;
    }

    //
    bmfHeader.bfType = DIB_HEADER_MARKER;

    //
    //
    //
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);

    //
    if ((lpBI->biCompression == BI_RLE8)||(lpBI->biCompression == BI_RLE4))
    {
        //
        dwDIBSize += lpBI->biSizeImage;
    }
    else
    {
        //
        DWORD dwBmBitsSize;
        //
        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
        //
        dwDIBSize += dwBmBitsSize;

        //
        lpBI->biSizeImage = dwBmBitsSize;
    }

    //
    bmfHeader.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);

    //
    bmfHeader.bfReserved1 = 0;
    bmfHeader.bfReserved2 = 0;

    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
                            + PaletteSize((LPSTR)lpBI);

    //
    try {
        //
        ::WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), NULL, NULL);

        //
        ::WriteFile(hFile, lpBI, dwDIBSize, NULL, NULL);

    }
    catch (std::exception *e)
    {
        ::GlobalUnlock((HGLOBAL)hDib);

        throw e;
    }

    ::GlobalUnlock((HGLOBAL)hDib);

    return TRUE;
}

BOOL WINAPI SaveDIBFilename(HDIB hDib, LPCTSTR lpFilename)
{
    UFile ufile;
    if (!ufile.open(lpFilename))
    {
        return FALSE;
    }
    return SaveDIB(hDib, ufile);
}

//
HDIB WINAPI ReadDIBFile(HANDLE hFile)
{
    BITMAPFILEHEADER bmfHeader;
    DWORD dwBitsSize;
    HDIB hDIB;
    LPSTR pDIB;

    ULARGE_INTEGER BitsSize;
    //
    //dwBitsSize = (DWORD)file.GetLength();
    BitsSize.LowPart = ::GetFileSize(hFile, &BitsSize.HighPart);
    dwBitsSize = BitsSize.QuadPart;

    //
    if (!::ReadFile(hFile, (LPSTR)&bmfHeader, sizeof(bmfHeader), NULL, NULL))
    {
        //
        return NULL;
    }

    //
    if (bmfHeader.bfType != DIB_HEADER_MARKER)
    {
        //
        return NULL;
    }

    //
    hDIB = (HDIB)::GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT, dwBitsSize);
    if (hDIB == 0)
    {
        //
        return NULL;
    }

    //
    pDIB = (LPSTR)::GlobalLock((HGLOBAL)hDIB);

    //
    //if (file.ReadHuge(pDIB, dwBitsSize-sizeof(BITMAPFILEHEADER))!=
    //        dwBitsSize-sizeof(BITMAPFILEHEADER))
    DWORD dwRead;
    ::ReadFile(hFile, pDIB, dwBitsSize-sizeof(BITMAPFILEHEADER), &dwRead, NULL);
    if (dwRead != dwBitsSize-sizeof(BITMAPFILEHEADER))
    {
        //
        ::GlobalUnlock((HGLOBAL)hDIB);

        //
        ::GlobalFree((HGLOBAL)hDIB);

        //
        return NULL;
    }

    //
    ::GlobalUnlock((HGLOBAL)hDIB);

    return hDIB;
}

HDIB WINAPI ReadDIBFileName(LPCTSTR lpFilename)
{
    UFile ufile;
    if (!ufile.open(lpFilename))
    {
        return NULL;
    }

    return ReadDIBFile(ufile);
}

// DDBToDIB        - Creates a DIB from a DDB
// bitmap        - Device dependent bitmap
// dwCompression    - Type of compression - see BITMAPINFOHEADER
// pPal            - Logical palette
HANDLE DDBToDIB( HBITMAP hBitmap, DWORD dwCompression, HPALETTE hPal )
{
    BITMAP            bm;
    BITMAPINFOHEADER    bi;
    LPBITMAPINFOHEADER     lpbi;
    DWORD            dwLen;
    HANDLE            hDIB;
    HANDLE            handle;
    HDC             hDC;
    //HPALETTE        hPal;


    //ASSERT( bitmap.GetSafeHandle() );

    // The function has no arg for bitfields
    if( dwCompression == BI_BITFIELDS )
        return NULL;

    // If a palette has not been supplied use defaul palette
    // hPal = (HPALETTE) pPal->GetSafeHandle();
    if (hPal==NULL)
        hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

    // Get bitmap information
    ::GetObject(hBitmap, sizeof(bm),(LPSTR)&bm);

    // Initialize the bitmapinfoheader
    bi.biSize        = sizeof(BITMAPINFOHEADER);
    bi.biWidth        = bm.bmWidth;
    bi.biHeight         = bm.bmHeight;
    bi.biPlanes         = 1;
    bi.biBitCount        = bm.bmPlanes * bm.bmBitsPixel;
    bi.biCompression    = dwCompression;
    bi.biSizeImage        = 0;
    bi.biXPelsPerMeter    = 0;
    bi.biYPelsPerMeter    = 0;
    bi.biClrUsed        = 0;
    bi.biClrImportant    = 0;

    // Compute the size of the  infoheader and the color table
    int nColors = (1 << bi.biBitCount);
    if( nColors > 256 )
        nColors = 0;
    dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

    // We need a device context to get the DIB from
    hDC = GetDC(NULL);
    hPal = ::SelectPalette(hDC,hPal,FALSE);
    ::RealizePalette(hDC);

    // Allocate enough memory to hold bitmapinfoheader and color table
    hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

    if (!hDIB){
        ::SelectPalette(hDC,hPal,FALSE);
        ::ReleaseDC(NULL,hDC);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)hDIB;

    *lpbi = bi;

    // Call GetDIBits with a NULL lpBits param, so the device driver
    // will calculate the biSizeImage field
    GetDIBits(hDC, hBitmap, 0L, (DWORD)bi.biHeight,
            (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

    bi = *lpbi;

    // If the driver did not fill in the biSizeImage field, then compute it
    // Each scan line of the image is aligned on a DWORD (32bit) boundary
    if (bi.biSizeImage == 0){
        bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
                        * bi.biHeight;

        // If a compression scheme is used the result may infact be larger
        // Increase the size to account for this.
        if (dwCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    // Realloc the buffer so that it can hold all the bits
    dwLen += bi.biSizeImage;
    if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
        hDIB = handle;
    else{
        GlobalFree(hDIB);

        // Reselect the original palette
        SelectPalette(hDC,hPal,FALSE);
        ReleaseDC(NULL,hDC);
        return NULL;
    }

    // Get the bitmap bits
    lpbi = (LPBITMAPINFOHEADER)hDIB;

    // FINALLY get the DIB
    BOOL bGotBits = GetDIBits( hDC, hBitmap,
                0L,                // Start scan line
                (DWORD)bi.biHeight,        // # of scan lines
                (LPBYTE)lpbi             // address for bitmap bits
                + (bi.biSize + nColors * sizeof(RGBQUAD)),
                (LPBITMAPINFO)lpbi,        // address of bitmapinfo
                (DWORD)DIB_RGB_COLORS);        // Use RGB for color table

    if( !bGotBits )
    {
        GlobalFree(hDIB);

        SelectPalette(hDC,hPal,FALSE);
        ReleaseDC(NULL,hDC);
        return NULL;
    }

    SelectPalette(hDC,hPal,FALSE);
    ReleaseDC(NULL,hDC);
    return hDIB;
}

// WriteDIB        - Writes a DIB to file
// Returns        - TRUE on success
// szFile        - Name of file to write to
// hDIB            - Handle of the DIB
BOOL WINAPI WriteDIB( LPCTSTR szFile, HANDLE hDIB)
{
    BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER    lpbi;

    if (!hDIB)
        return FALSE;

    HANDLE hFile = NULL;
    //OFSTRUCT ofs;
    //::ZeroMemory(ofs, sizeof(OFSTRUCT));
    //hFile = ::OpenFile( szFile, &ofs, OF_WRITE|OF_CREATE);
    hFile = ::CreateFile(szFile, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if( !hFile )
        return FALSE;

    lpbi = (LPBITMAPINFOHEADER)hDIB;

    int nColors = 1 << lpbi->biBitCount;

    // Fill in the fields of the file header
    hdr.bfType        = ((WORD) ('M' << 8) | 'B');    // is always "BM"
    hdr.bfSize        = GlobalSize (hDIB) + sizeof( hdr );
    hdr.bfReserved1     = 0;
    hdr.bfReserved2     = 0;
    hdr.bfOffBits        = (DWORD) (sizeof( hdr ) + lpbi->biSize +
        nColors * sizeof(RGBQUAD));

    // Write the file header
    ::WriteFile(hFile, &hdr, sizeof(hdr), NULL, NULL );

    // Write the DIB header and the bits
    ::WriteFile(hFile, lpbi, GlobalSize(hDIB), NULL, NULL);

    return TRUE;
}
// TransparentBlt    - Copies a bitmap transparently onto the destination DC
// hdcDest        - Handle to destination device context
// nXDest        - x-coordinate of destination rectangle's upper-left corner
// nYDest        - y-coordinate of destination rectangle's upper-left corner
// nWidth        - Width of destination rectangle
// nHeight        - height of destination rectangle
// hBitmap        - Handle of the source bitmap
// nXSrc        - x-coordinate of source rectangle's upper-left corner
// nYSrc        - y-coordinate of source rectangle's upper-left corner
// colorTransparent    - The transparent color
// hPal            - Logical palette to be used with bitmap. Can be NULL

void TransparentBlt( HDC hdcDest, int nXDest, int nYDest, int nWidth,
                    int nHeight, HBITMAP hBitmap, int nXSrc, int nYSrc,
                    COLORREF colorTransparent, HPALETTE hPal )
{
    HDC hdc, hMemDC, hMaskDC, hTempDC;
    hdc = hdcDest;
    hMaskDC = ::CreateCompatibleDC(hdc);
    HBITMAP hMaskBitmap;

    //add these to store return of SelectObject() calls
    HBITMAP hOldMemBmp = NULL;
    HBITMAP hOldMaskBmp = NULL;
    HBITMAP hOldTempBmp = NULL;

    hMemDC = CreateCompatibleDC(hdc);
    hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hBmpImage;
    hBmpImage = CreateCompatibleBitmap( hdc, nWidth, nHeight );
    hOldMemBmp = (HBITMAP)::SelectObject(hMemDC, hBmpImage);

    // Select and realize the palette
    if( ::GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE && hPal )
    {
        ::SelectPalette( hdc, hPal, FALSE );
        ::RealizePalette(hdc);

        ::SelectPalette( hMemDC, hPal, FALSE );
    }

    hOldTempBmp = (HBITMAP) ::SelectObject( hTempDC, hBitmap );

    ::BitBlt(hMemDC, 0,0,nWidth, nHeight, hTempDC, nXSrc, nYSrc, SRCCOPY );

    // Create monochrome bitmap for the mask
    hMaskBitmap = ::CreateBitmap(nWidth, nHeight, 1, 1, NULL );
    hOldMaskBmp = (HBITMAP)::SelectObject(hMaskDC, hMaskBitmap);
    ::SetBkColor( hMemDC, colorTransparent );

    // Create the mask from the memory DC
    ::BitBlt(hMaskDC, 0, 0, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY );

    // Set the background in memDC to black. Using SRCPAINT with black
    // and any other color results in the other color, thus making
    // black the transparent color
    ::SetBkColor(hMemDC, RGB(0,0,0));
    ::SetTextColor(hMemDC, RGB(255,255,255));
    ::BitBlt(hMemDC, 0, 0, nWidth, nHeight, hMaskDC, 0, 0, SRCAND);

    // Set the foreground to black. See comment above.
    ::SetBkColor(hdc, RGB(255,255,255));
    ::SetTextColor(hdc, RGB(0,0,0));
    BitBlt(hdc, nXDest, nYDest, nWidth, nHeight, hMaskDC, 0, 0, SRCAND);

    // Combine the foreground with the background
    BitBlt(hdc, nXDest, nYDest, nWidth, nHeight, hMemDC,
        0, 0, SRCPAINT);


    if (hOldTempBmp)
        ::SelectObject( hTempDC, hOldTempBmp);
    if (hOldMaskBmp)
        SelectObject( hMaskDC, hOldMaskBmp );
    if (hOldMemBmp)
        SelectObject( hMemDC, hOldMemBmp );
}

HBITMAP GetSrcBit(HDC hDC,DWORD BitWidth, DWORD BitHeight)
{
    HDC hBufDC;
    HBITMAP hBitmap, hBitTemp;
    hBufDC = CreateCompatibleDC(hDC);
    //
    hBitmap = CreateCompatibleBitmap(hDC, BitWidth, BitHeight);
    hBitTemp = (HBITMAP) SelectObject(hBufDC, hBitmap);
    //
    StretchBlt(hBufDC, 0, 0, BitWidth, BitHeight,hDC, 0, 0, BitWidth, BitHeight, SRCCOPY);
    //
    hBitmap = (HBITMAP) SelectObject(hBufDC, hBitTemp);
    //
    DeleteObject(hBitTemp);
    ::DeleteDC(hBufDC);
    return hBitmap;
}

//HANDLE DDBToDIB( HBITMAP hbitmap, DWORD dwCompression, HPALETTE hPal )
//{
//    BITMAP            bm;
//    BITMAPINFOHEADER    bi;
//    LPBITMAPINFOHEADER     lpbi;
//    DWORD            dwLen;
//    HANDLE            hDIB;
//    HANDLE            handle;
//    HDC             hDC;
//    //HPALETTE        hPal;
//
//
//    ::GetObject(hbitmap, sizeof(BITMAP), &bm);
//
//    // The function has no arg for bitfields
//    if( dwCompression == BI_BITFIELDS )
//        return NULL;
//
//    // If a palette has not been supplied use defaul palette
//    //hPal = (HPALETTE) pPal->GetSafeHandle();
//    if (hPal==NULL)
//        hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
//
//    // Initialize the bitmapinfoheader
//    bi.biSize            = sizeof(BITMAPINFOHEADER);
//    bi.biWidth            = bm.bmWidth;
//    bi.biHeight         = bm.bmHeight;
//    bi.biPlanes         = 1;
//    bi.biBitCount        = bm.bmPlanes * bm.bmBitsPixel;
//    bi.biCompression    = dwCompression;
//    bi.biSizeImage        = 0;
//    bi.biXPelsPerMeter    = 0;
//    bi.biYPelsPerMeter    = 0;
//    bi.biClrUsed        = 0;
//    bi.biClrImportant    = 0;
//
//    // Compute the size of the  infoheader and the color table
//    int nColors = (1 << bi.biBitCount);
//    if( nColors > 256 )
//        nColors = 0;
//    dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);
//
//    // We need a device context to get the DIB from
//    hDC = GetDC(NULL);
//    hPal = SelectPalette(hDC,hPal,FALSE);
//    RealizePalette(hDC);
//
//    // Allocate enough memory to hold bitmapinfoheader and color table
//    hDIB = GlobalAlloc(GMEM_FIXED,dwLen);
//
//    if (!hDIB){
//        SelectPalette(hDC,hPal,FALSE);
//        ReleaseDC(NULL,hDC);
//        return NULL;
//    }
//
//    lpbi = (LPBITMAPINFOHEADER)hDIB;
//
//    *lpbi = bi;
//
//    // Call GetDIBits with a NULL lpBits param, so the device driver
//    // will calculate the biSizeImage field
//    GetDIBits(hDC, hbitmap, 0L, (DWORD)bi.biHeight,
//            (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);
//
//    bi = *lpbi;
//
//    // If the driver did not fill in the biSizeImage field, then compute it
//    // Each scan line of the image is aligned on a DWORD (32bit) boundary
//    if (bi.biSizeImage == 0){
//        bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
//                        * bi.biHeight;
//
//        // If a compression scheme is used the result may infact be larger
//        // Increase the size to account for this.
//        if (dwCompression != BI_RGB)
//            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
//    }
//
//    // Realloc the buffer so that it can hold all the bits
//    dwLen += bi.biSizeImage;
//    if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
//        hDIB = handle;
//    else{
//        GlobalFree(hDIB);
//
//        // Reselect the original palette
//        SelectPalette(hDC,hPal,FALSE);
//        ReleaseDC(NULL,hDC);
//        return NULL;
//    }
//
//    // Get the bitmap bits
//    lpbi = (LPBITMAPINFOHEADER)hDIB;
//
//    // FINALLY get the DIB
//    BOOL bGotBits = GetDIBits( hDC, hbitmap,
//                0L,                // Start scan line
//                (DWORD)bi.biHeight,        // # of scan lines
//                (LPBYTE)lpbi             // address for bitmap bits
//                + (bi.biSize + nColors * sizeof(RGBQUAD)),
//                (LPBITMAPINFO)lpbi,        // address of bitmapinfo
//                (DWORD)DIB_RGB_COLORS);        // Use RGB for color table
//
//    if( !bGotBits )
//    {
//        GlobalFree(hDIB);
//
//        SelectPalette(hDC,hPal,FALSE);
//        ReleaseDC(NULL,hDC);
//        return NULL;
//    }
//
//    SelectPalette(hDC,hPal,FALSE);
//    ReleaseDC(NULL,hDC);
//    return hDIB;
//}

BOOL SaveBmp(HBITMAP hBitmap, LPCTSTR FileName)
{
    //设备描述表
    HDC hDC;
    //当前分辨率下每象素所占字节数
    int iBits;
    //位图中每象素所占字节数
    WORD wBitCount;
    //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数
    DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
    //位图属性结构
    BITMAP Bitmap;
    //位图文件头结构
    BITMAPFILEHEADER bmfHdr;
    //位图信息头结构
    BITMAPINFOHEADER bi;
    //指向位图信息头结构
    LPBITMAPINFOHEADER lpbi;
    //定义文件，分配内存句柄，调色板句柄
    HANDLE fh, hDib, hPal,hOldPal=NULL;
    //计算位图文件每个像素所占字节数
    hDC = ::CreateDC("DISPLAY", NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1) wBitCount = 1;
    else if (iBits <= 4) wBitCount = 4;
    else if (iBits <= 8) wBitCount = 8;
    else wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap.bmWidth;
    bi.biHeight = Bitmap.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 0;
    dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
    //为位图内容分配内存
    hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;
    // 处理调色板
    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = ::GetDC(NULL);
        hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }
    // 获取该调色板下新的像素值
    GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
    //恢复调色板
    if (hOldPal)
    {
        ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
        RealizePalette(hDC);
        ::ReleaseDC(NULL, hDC);
    }
    //创建位图文件
    fh = CreateFile(FileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (fh == INVALID_HANDLE_VALUE) return FALSE;
    // 设置位图文件头
    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    // 写入位图文件头
    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    // 写入位图文件其余内容
    // WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); //清除
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

BOOL CopyColorTable( LPBITMAPINFO lpTarget, LPBITMAPINFO lpSource )
{
    // What we do depends on the target's color depth
    switch( lpTarget->bmiHeader.biBitCount )
    {
        // 8bpp - need 256 entry color table
    case 8:
        if( lpSource->bmiHeader.biBitCount == 8 )
        { // Source is 8bpp too, copy color table
            memcpy( lpTarget->bmiColors, lpSource->bmiColors, 256*sizeof(RGBQUAD) );
            return TRUE;
        }
        else
        { // Source is != 8bpp, use halftone palette
            HPALETTE        hPal;
            HDC                hDC = GetDC( NULL );
            PALETTEENTRY    pe[256];
            UINT            i;

            hPal = CreateHalftonePalette( hDC );
            ReleaseDC( NULL, hDC );
            GetPaletteEntries( hPal, 0, 256, pe );
            DeleteObject( hPal );
            for(i=0;i<256;i++)
            {
                lpTarget->bmiColors[i].rgbRed = pe[i].peRed;
                lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen;
                lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue;
                lpTarget->bmiColors[i].rgbReserved = pe[i].peFlags;
            }
            return TRUE;
        }
        break; // end 8bpp

        // 4bpp - need 16 entry color table
    case 4:
        if( lpSource->bmiHeader.biBitCount == 4 )
        { // Source is 4bpp too, copy color table
            memcpy( lpTarget->bmiColors, lpSource->bmiColors, 16*sizeof(RGBQUAD) );
            return TRUE;
        }
        else
        { // Source is != 4bpp, use system palette
            HPALETTE        hPal;
            PALETTEENTRY    pe[256];
            UINT            i;

            hPal = (HPALETTE)GetStockObject( DEFAULT_PALETTE );
            GetPaletteEntries( hPal, 0, 16, pe );
            for(i=0;i<16;i++)
            {
                lpTarget->bmiColors[i].rgbRed = pe[i].peRed;
                lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen;
                lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue;
                lpTarget->bmiColors[i].rgbReserved = pe[i].peFlags;
            }
            return TRUE;
        }
        break; // end 4bpp

        // 1bpp - need 2 entry mono color table
    case 1:
        lpTarget->bmiColors[0].rgbRed = 0;
        lpTarget->bmiColors[0].rgbGreen = 0;
        lpTarget->bmiColors[0].rgbBlue = 0;
        lpTarget->bmiColors[0].rgbReserved = 0;
        lpTarget->bmiColors[1].rgbRed = 255;
        lpTarget->bmiColors[1].rgbGreen = 255;
        lpTarget->bmiColors[1].rgbBlue = 255;
        lpTarget->bmiColors[1].rgbReserved = 0;
        break; // end 1bpp

        // no color table for the > 8bpp modes
    case 32:
    case 24:
    case 16:
    default:
        return TRUE;
        break;
    }
    return TRUE;
}

LPBYTE ConvertDIBFormat( LPBITMAPINFO lpSrcDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch )
{
    LPBITMAPINFO    lpbmi = NULL;
    LPBYTE            lpSourceBits, lpTargetBits, lpResult;
    HDC                hDC = NULL, hSourceDC, hTargetDC;
    HBITMAP            hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap;
    DWORD            dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize;

    // Allocate and fill out a BITMAPINFO struct for the new DIB
    // Allow enough room for a 256-entry color table, just in case
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( 256 * sizeof( RGBQUAD ) );
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize );
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    lpbmi->bmiHeader.biWidth = nWidth;
    lpbmi->bmiHeader.biHeight = nHeight;
    lpbmi->bmiHeader.biPlanes = 1;
    lpbmi->bmiHeader.biBitCount = nbpp;
    lpbmi->bmiHeader.biCompression = BI_RGB;
    lpbmi->bmiHeader.biSizeImage = 0;
    lpbmi->bmiHeader.biXPelsPerMeter = 0;
    lpbmi->bmiHeader.biYPelsPerMeter = 0;
    lpbmi->bmiHeader.biClrUsed = 0;
    lpbmi->bmiHeader.biClrImportant = 0;
    // Fill in the color table
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB ) )
    {
        free( lpbmi );
        return NULL;
    }

    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em
    hDC = GetDC( NULL );
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (void **)&lpTargetBits, NULL, 0 );
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (void **)&lpSourceBits, NULL, 0 );
    hSourceDC = CreateCompatibleDC( hDC );
    hTargetDC = CreateCompatibleDC( hDC );

    // Flip the bits on the source DIBSection to match the source DIB
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine(&(lpSrcDIB->bmiHeader));
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine(&(lpbmi->bmiHeader));
    memcpy( lpSourceBits, FindDIBBits((LPSTR)lpSrcDIB), dwSourceBitsSize );

    // Select DIBSections into DCs
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap );
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap );

    // Set the color tables for the DIBSections
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 )
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors );
    if( lpbmi->bmiHeader.biBitCount <= 8 )
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors );

    // If we are asking for a straight copy, do it
    if( (lpSrcDIB->bmiHeader.biWidth==lpbmi->bmiHeader.biWidth) && (lpSrcDIB->bmiHeader.biHeight==lpbmi->bmiHeader.biHeight) )
    {
        BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY );
    }
    else
    {
        // else, should we stretch it?
        if( bStretch )
        {
            SetStretchBltMode( hTargetDC, COLORONCOLOR );
            StretchBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, lpSrcDIB->bmiHeader.biWidth, lpSrcDIB->bmiHeader.biHeight, SRCCOPY );
        }
        else
        {
            // or just take the upper left corner of the source
            BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY );
        }
    }

    // Clean up and delete the DCs
    SelectObject( hSourceDC, hOldSourceBitmap );
    SelectObject( hSourceDC, hOldTargetBitmap );
    DeleteDC( hSourceDC );
    DeleteDC( hTargetDC );
    ReleaseDC( NULL, hDC );

    // Flush the GDI batch, so we can play with the bits
    GdiFlush();

    // Allocate enough memory for the new CF_DIB, and copy bits
    lpResult = (LPBYTE)malloc( dwTargetHeaderSize + dwTargetBitsSize );
    memcpy( lpResult, lpbmi, dwTargetHeaderSize );
    memcpy( huys::FindDIBBits( (LPSTR)lpResult ), lpTargetBits, dwTargetBitsSize );

    // final cleanup
    DeleteObject( hTargetBitmap );
    DeleteObject( hSourceBitmap );
    free( lpbmi );

    return lpResult;
}

}; // namespace huys
