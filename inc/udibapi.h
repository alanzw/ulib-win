//
#ifndef DIBAPI_H
#define DIBAPI_H

#include "ulib.h"

namespace huys
{

//
DECLARE_HANDLE(HDIB);

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
BOOL WINAPI PaintDIB(HDC, LPRECT, HDIB, LPRECT, HPALETTE hPal);

BOOL WINAPI CreateDIBPalette(HDIB hDIB, HPALETTE hPal);

LPSTR WINAPI FindDIBBits(LPSTR lpbi);

DWORD WINAPI DIBWidth(LPSTR lpDIB);

DWORD WINAPI DIBHeight(LPSTR lpDIB);

WORD WINAPI PaletteSize(LPSTR lpbi);

WORD WINAPI DIBNumColors(LPSTR lpbi);

HGLOBAL WINAPI CopyHandle(HGLOBAL h);

BOOL WINAPI SaveDIB(HDIB hDib, HANDLE hFile);

HDIB WINAPI ReadDIBFile(HANDLE hFile);


//
HANDLE DDBToDIB( HBITMAP hBitmap, DWORD dwCompression, HPALETTE hPal );

BOOL WriteDIB( LPTSTR szFile, HANDLE hDIB);

void TransparentBlt( HDC hdcDest,
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
HBITMAP GetSrcBit(HDC hDC,DWORD BitWidth, DWORD BitHeight);

//
BOOL SaveBmp(HBITMAP hBitmap, LPCTSTR FileName);

}; // namespace huys

#endif

