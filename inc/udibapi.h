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
ULIB_API BOOL WINAPI PaintDIB(HDC, LPRECT, HDIB, LPRECT, HPALETTE hPal);

ULIB_API BOOL WINAPI CreateDIBPalette(HDIB hDIB, HPALETTE hPal);

ULIB_API LPSTR WINAPI FindDIBBits(LPSTR lpbi);

ULIB_API DWORD WINAPI DIBWidth(LPSTR lpDIB);

ULIB_API DWORD WINAPI DIBHeight(LPSTR lpDIB);

ULIB_API WORD WINAPI PaletteSize(LPSTR lpbi);

ULIB_API DWORD WINAPI BytesPerLine( LPBITMAPINFOHEADER lpBMIH );

ULIB_API WORD WINAPI DIBNumColors(LPSTR lpbi);

ULIB_API HGLOBAL WINAPI CopyHandle(HGLOBAL h);
ULIB_API LPBYTE ConvertDIBFormat( LPBITMAPINFO lpSrcDIB, UINT nWidth, UINT nHeight, UINT nColors, BOOL bStretch );
ULIB_API void SetMonoDIBPixel( LPBYTE pANDBits, DWORD dwWidth, DWORD dwHeight, DWORD x, DWORD y, BOOL bWhite );

ULIB_API BOOL WINAPI SaveDIB(HDIB hDib, HANDLE hFile);
ULIB_API BOOL WINAPI SaveDIBFilename(HDIB hDib, LPCTSTR lpFilename);

ULIB_API HDIB WINAPI ReadDIBFile(HANDLE hFile);
ULIB_API HDIB WINAPI ReadDIBFileName(LPCTSTR lpFilename);

//
ULIB_API HANDLE DDBToDIB( HBITMAP hBitmap, DWORD dwCompression, HPALETTE hPal );

ULIB_API BOOL WINAPI WriteDIB( LPCTSTR szFile, HANDLE hDIB);

ULIB_API void TransparentBlt( HDC hdcDest,
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
ULIB_API HBITMAP GetSrcBit(HDC hDC,DWORD BitWidth, DWORD BitHeight);

//
ULIB_API BOOL SaveBmp(HBITMAP hBitmap, LPCTSTR FileName);


class ULIB_API UDib
{
public:
    UDib();
    
    ~UDib();
    
};


}; // namespace huys

#endif

