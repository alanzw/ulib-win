//
#ifndef DIBAPI_H
#define DIBAPI_H

#include "ulib.h"

namespace huys
{

//
DECLARE_HANDLE(HDIB);

/* Print Area selection */
#define PW_WINDOW        1
#define PW_CLIENT        2


/* Print Options selection */
#define PW_BESTFIT       1
#define PW_STRETCHTOPAGE 2
#define PW_SCALE         3

//
#define PALVERSION 0x300

//
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi))==sizeof(BITMAPINFOHEADER))

//
#define RECTWIDTH(lpRect)   ((lpRect)->right - (lpRect)->left)

//
#define RECTHEIGHT(lpRect)  ((lpRect)->bottom - (lpRect)->top)

//
#define WIDTHBYTES(bits)  (( (bits) + 31) / 32*4)

//
ULIB_API BOOL WINAPI PaintDIB(HDC, LPRECT, HDIB, LPRECT, HPALETTE hPal);

ULIB_API BOOL WINAPI PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB, LPRECT lpDDBRect, HPALETTE hPal);

ULIB_API BOOL WINAPI CreateDIBPalette(HDIB hDIB, HPALETTE hPal);

ULIB_API LPSTR WINAPI FindDIBBits(LPSTR lpbi);

ULIB_API DWORD WINAPI DIBWidth(LPSTR lpDIB);

ULIB_API DWORD WINAPI DIBHeight(LPSTR lpDIB);

ULIB_API int WINAPI PalEntriesOnDevice(HDC hDC);

ULIB_API HPALETTE WINAPI GetSystemPalette();

ULIB_API WORD WINAPI PaletteSize(LPSTR lpbi);

ULIB_API DWORD WINAPI BytesPerLine( LPBITMAPINFOHEADER lpBMIH );

ULIB_API WORD WINAPI DIBNumColors(LPSTR lpbi);

ULIB_API HGLOBAL WINAPI CopyHandle(HGLOBAL h);

ULIB_API LPBYTE ConvertDIBFormat( LPBITMAPINFO lpSrcDIB, UINT nWidth, UINT nHeight, UINT nColors, BOOL bStretch );

ULIB_API void SetMonoDIBPixel( LPBYTE pANDBits, DWORD dwWidth, DWORD dwHeight, DWORD x, DWORD y, BOOL bWhite );

ULIB_API HDIB CopyWindowToDIB(HWND hWnd, WORD fPrintArea);

ULIB_API HDIB CopyScreenToDIB(LPRECT lpRect);

ULIB_API HBITMAP CopyWindowToBitmap(HWND hWnd, WORD fPrintArea);

ULIB_API HBITMAP CopyScreenToBitmap(LPRECT lpRect);

ULIB_API BOOL WINAPI SaveDIB(HDIB hDib, HANDLE hFile);
ULIB_API BOOL WINAPI SaveDIBFilename(HDIB hDib, LPCTSTR lpFilename);

ULIB_API HDIB WINAPI ReadDIBFile(HANDLE hFile);
ULIB_API HDIB WINAPI ReadDIBFileName(LPCTSTR lpFilename);

//
ULIB_API HANDLE DDBToDIB( HBITMAP hBitmap, DWORD dwCompression, HPALETTE hPal );

ULIB_API HDIB BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);

ULIB_API BOOL WINAPI WriteDIB( LPCTSTR szFile, HANDLE hDIB);

ULIB_API void WINAPI TransparentBlt( HDC hdcDest,
                     int nXDest,
                     int nYDest,
                     int nWidth,
                     int nHeight,
                     HBITMAP hBitmap,
                     int nXSrc,
                     int nYSrc,
                     COLORREF colorTransparent,
                     HPALETTE hPal );

//
ULIB_API HBITMAP WINAPI GetSrcBit(HDC hDC,DWORD BitWidth, DWORD BitHeight);

//
ULIB_API BOOL WINAPI SaveBmp(HBITMAP hBitmap, LPCTSTR FileName);


class ULIB_API UDib
{
public:
    UDib();

    ~UDib();

};


}; // namespace huys

#endif

