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

// Image position in Canvas
#define LEFT_UP         1
#define CENTER_UP       2
#define RIGHT_UP        3
#define LEFT_CENTER     4
#define CENTER_CENTER   5
#define CENTER_RIGHT    6
#define LEFT_DOWN       7
#define CENTER_DOWN     8
#define RIGHT_DOWN      9

/* Gray Method */
#define MEAN_GRAY      0
#define MAXIMUM_GRAY   1
#define WEIGHT_GRAY    2

/* Color Model */
#define RGB_COLOR     0
#define CMYK_COLOR    1
#define HSI_COLOR     2
#define HLS_COLOR     3
#define UNDEFINED     (-1.0)

// Separate color
#define SEPARATE_RED     1
#define SEPARATE_GREEN   2
#define SEPARATE_BLUE    3

// Filte color
#define FILTE_RED      1
#define FILTE_GREEN    2
#define FILTE_BLUE     3

// Filter algorithm
#define FILTER1     1
#define FILTER2     2
#define FILTER3     3

// Edge enhancement algorithm
#define VERT       1
#define HORZ       2
#define VERTHORZ   3
#define NORTH      4
#define NORTHEAST  5
#define EAST       6
#define SOUTH      7
#define SOUTHEAST  8
#define SOUTHWEST  9
#define WEST       10
#define NORTHWEST  11
#define LAP1       12
#define LAP2       13
#define LAP3       14
#define LAP4       15
#define SOBEL      16
#define HOUGH      17


//
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi))==sizeof(BITMAPINFOHEADER))

//
#define RECTWIDTH(lpRect)   ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)  ((lpRect)->bottom - (lpRect)->top)

//
#define WIDTHBYTES(bits)  (( (bits) + 31) / 32*4)
#define BOUND(x, mn, mx)    ((x) < (mn) ? (mn) : ((x) > (mx) ? (mx) : (x)))
#define WaitCursorBegin()    HCURSOR hcURSOR = SetCursor(LoadCursor(NULL, IDC_WAIT))
#define WaitCursorEnd()        SetCursor(hcURSOR)

// DIB initialization
ULIB_API HDIB WINAPI CreateDIB(DWORD, DWORD, WORD);
ULIB_API HDIB WINAPI CreateDefaultDIB(DWORD dwWidth, DWORD dwHeight);
ULIB_API void WINAPI DestroyDIB(HDIB);

ULIB_API BOOL WINAPI SaveDIB(HDIB hDib, HANDLE hFile);
ULIB_API BOOL WINAPI SaveDIBFilename(HDIB hDib, LPCTSTR lpFilename);

ULIB_API HDIB WINAPI ReadDIBFile(HANDLE hFile);
ULIB_API HDIB WINAPI ReadDIBFileName(LPCTSTR lpFilename);

ULIB_API BOOL WINAPI WriteDIB( LPCTSTR szFile, HANDLE hDIB);

//
ULIB_API BOOL WINAPI PaintDIB(HDC, LPRECT, HDIB, LPRECT, HPALETTE hPal);

ULIB_API BOOL WINAPI PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB, LPRECT lpDDBRect, HPALETTE hPal);

ULIB_API BOOL WINAPI CreateDIBPalette(HDIB hDIB, HPALETTE hPal);

ULIB_API LPSTR WINAPI FindDIBBits(LPSTR lpbi);

ULIB_API DWORD WINAPI DIBWidth(LPSTR lpDIB);
ULIB_API DWORD WINAPI DIBHeight(LPSTR lpDIB);
ULIB_API DWORD WINAPI DIBHeight(HDIB hDIB);
ULIB_API DWORD WINAPI DIBWidth(HDIB hDIB);
ULIB_API DWORD WINAPI DIBBitCount(LPSTR lpDIB);
ULIB_API DWORD WINAPI DIBBitCount(HDIB hDIB);
ULIB_API int WINAPI PalEntriesOnDevice(HDC hDC);

ULIB_API HPALETTE WINAPI GetSystemPalette();

ULIB_API WORD WINAPI PaletteSize(LPSTR lpbi);

ULIB_API DWORD WINAPI BytesPerLine( LPBITMAPINFOHEADER lpBMIH );
ULIB_API DWORD WINAPI BytesPerLine( HDIB hDIB);
ULIB_API WORD WINAPI DIBNumColors(LPSTR lpbi);

ULIB_API HGLOBAL WINAPI CopyHandle(HGLOBAL h);

ULIB_API LPBYTE WINAPI ConvertDIBFormat( LPBITMAPINFO lpSrcDIB, UINT nWidth, UINT nHeight, UINT nColors, BOOL bStretch );
ULIB_API HDIB WINAPI ConvertDIBFormat(LPBYTE lpSrcDIB, UINT nbpp, HPALETTE hPalSrc);
ULIB_API HDIB WINAPI ConvertDIBFormat(HDIB hDIB, UINT nbpp, HPALETTE hPalSrc);


ULIB_API void WINAPI SetMonoDIBPixel( LPBYTE pANDBits, DWORD dwWidth, DWORD dwHeight, DWORD x, DWORD y, BOOL bWhite );

ULIB_API HDIB WINAPI CopyWindowToDIB(HWND hWnd, WORD fPrintArea);

ULIB_API HDIB WINAPI CopyScreenToDIB(LPRECT lpRect);

ULIB_API HBITMAP WINAPI CopyWindowToBitmap(HWND hWnd, WORD fPrintArea);

ULIB_API HBITMAP WINAPI CopyScreenToBitmap(LPRECT lpRect);


//
ULIB_API HANDLE WINAPI DDBToDIB( HBITMAP hBitmap, DWORD dwCompression, HPALETTE hPal );

ULIB_API HDIB WINAPI BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);


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


// DIB transform
ULIB_API HBITMAP WINAPI CropBitmap(HBITMAP, LPRECT);
ULIB_API HDIB WINAPI CropDIB(HDIB, LPRECT);
ULIB_API HDIB WINAPI CutDIB(HDIB, LPRECT);
ULIB_API HDIB WINAPI MergeDIB(HDIB hDib1, HDIB hDib2, POINT ptTopLeft);
ULIB_API HDIB WINAPI RotateDIB(HDIB hDIB, double fDegrees, COLORREF clrBack);
ULIB_API HDIB WINAPI RotateDIB(HDIB hDib);
ULIB_API HDIB WINAPI FlipHorzDIB(HDIB hDib);
ULIB_API HDIB WINAPI FlipVertDIB(HDIB hDib);
ULIB_API HDIB WINAPI ChangeDIBSize(HDIB hDIB, int nWidth, int nHeight);
ULIB_API HDIB WINAPI ChangeDIBCanvasSize(HDIB hDIB, int nWidth, int nHeight, int nPosition);
// Color quantization
//ULIB_API HPALETTE WINAPI CreateOctreePalette(HDIB hDIB, UINT nMaxColors, UINT nColorBits);
//ULIB_API HPALETTE WINAPI CreateOctreePalette(LPBYTE lpDIB, UINT nMaxColors, UINT nColorBits);
// color process
ULIB_API HDIB WINAPI SeparateRGBfromDIB(HDIB hDib, int nIndex);
ULIB_API HDIB WINAPI FilteRGBfromDIB(HDIB hDib, int nIndex);
ULIB_API HDIB WINAPI ColorQuantizeDIB(HDIB hDIB, UINT nColorBits, UINT nMaxColors);
ULIB_API HPALETTE WINAPI ConvertToGrayscale(HDIB hDib, int nMethod,
                            double fRedWeight, double fGreenWeight, double fBlueWeight);
ULIB_API BOOL WINAPI AdjustDIBColor(HDIB hDib, int nColorModel, int v1, int v2, int v3);
ULIB_API BOOL WINAPI AdjustDIBBrightness(HDIB hDib, int v);
ULIB_API BOOL WINAPI AdjustDIBContrast(HDIB hDib, int v);
ULIB_API void WINAPI RGBtoHSI(BYTE r, BYTE g, BYTE b, double *h, double *s, double *i);
ULIB_API void WINAPI HSItoRGB(double h, double s, double i, BYTE *r, BYTE *g, BYTE *b);
ULIB_API void WINAPI RGBtoHLS(BYTE r, BYTE g, BYTE b, double *h, double *l, double *s);
ULIB_API void WINAPI HLStoRGB(double h, double l, double s, BYTE *r, BYTE *g, BYTE *b);
ULIB_API void WINAPI CMYKtoRGB(BYTE c, BYTE m, BYTE y, BYTE k, BYTE *r, BYTE *g, BYTE *b);
ULIB_API void WINAPI RGBtoCMYK(BYTE r, BYTE g, BYTE b, BYTE *c, BYTE *m, BYTE *y, BYTE *k);
ULIB_API void WINAPI ChangeContrast(int nDelta, BYTE *r, BYTE *g, BYTE *b);
ULIB_API void WINAPI ChangeBrightness(int nDelta, BYTE *r, BYTE *g, BYTE *b);

// image process
ULIB_API BOOL WINAPI HighPassDIB(HDIB hDib, int Strength, int nAlgorithm);
ULIB_API BOOL WINAPI LowPassDIB(HDIB hDib, int Strength, int nAlgorithm);
ULIB_API BOOL WINAPI EdgeEnhanceDIB(HDIB hDib, int Strength, int nDirection);
ULIB_API BOOL WINAPI MedianFilterDIB(HDIB hDib);


class ULIB_API UDib
{
public:
    UDib();

    ~UDib();

};


}; // namespace huys

#endif

