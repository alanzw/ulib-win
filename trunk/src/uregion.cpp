/*
 * =====================================================================================
 *
 *       Filename:  uregion.cpp
 *
 *    Description:  implement
 *
 *        Version:  1.0
 *        Created:  2009-7-3 4:04:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>

#include "uregion.h"

URegion::URegion()
: UGDIObject()
{}

URegion::~URegion()
{

}

bool URegion::createRect(int left, int top, int right, int bottom)
{
    m_hObj = ::CreateRectRgn(left, top, right, bottom);

    return (NULL != m_hObj);
}

bool URegion::createRectIndirect(const LPRECT lpRect)
{
    m_hObj = ::CreateRectRgnIndirect(lpRect);

    return (NULL != m_hObj);
}

bool URegion::createRoundRect(int left, int top, int right, int bottom, int w, int h)
{
    m_hObj = ::CreateRoundRectRgn(left, top, right, bottom, w, h);
    return (NULL != m_hObj);
}

bool URegion::createElliptic( int left, int top, int right, int bottom )
{
    m_hObj = ::CreateEllipticRgn(left, top, right, bottom);
    return (NULL != m_hObj);
}

bool URegion::createEllipticIndirect( const LPRECT lpRect )
{
    m_hObj = ::CreateEllipticRgnIndirect(lpRect);
    return (NULL != m_hObj);
}

bool URegion::createPolygon( const POINT *lppt, int cPoints, int fnPolyFillMode )
{
    m_hObj = ::CreatePolygonRgn(lppt, cPoints, fnPolyFillMode);
    return (NULL != m_hObj);
}

bool URegion::createPolyPolygon( const POINT *lppt, const INT *lpPolyCounts, int nCount, int fnPolyFillMode )
{
    m_hObj = ::CreatePolyPolygonRgn(lppt, lpPolyCounts, nCount, fnPolyFillMode);
    return (NULL != m_hObj);
}

bool URegion::ptInRgn( int x, int y )
{
    return TRUE == ::PtInRegion((HRGN)m_hObj, x, y);
}

bool URegion::rectInRgn( const LPRECT lpRect )
{
    return TRUE == ::RectInRegion((HRGN)m_hObj, lpRect);
}

int URegion::combine(HRGN hRgnSrc1, HRGN hRgnSrc2, int nMode)
{
    return ::CombineRgn((HRGN)m_hObj, hRgnSrc1, hRgnSrc2, nMode);
}

int URegion::combine( URegion &ur, int nMode )
{
    // This region must exist before CombineRgn is called.
    HRGN hRgn = CreateRectRgn (0, 0, 1, 1) ;
    int nRet = ::CombineRgn(hRgn, (HRGN)m_hObj, (HRGN)ur.m_hObj, nMode);
    m_hObj = hRgn;
    return nRet;
}

bool URegion::isEqual(HRGN hRgn)
{
    return TRUE == ::EqualRgn((HRGN)m_hObj, hRgn);
}

bool URegion::fill(HDC hdc, HBRUSH hBrush)
{
    return TRUE == ::FillRgn(hdc, (HRGN)m_hObj, hBrush);
}

bool URegion::paint( HDC hdc )
{
    return TRUE == ::PaintRgn(hdc, (HRGN)m_hObj);
}

int URegion::offset( int x, int y )
{
    return ::OffsetRgn((HRGN)m_hObj, x, y);
}

int URegion::getRgnBox( LPRECT lpRect )
{
    return ::GetRgnBox((HRGN)m_hObj, lpRect);
}

bool URegion::invert( HDC hdc )
{
    return TRUE == ::InvertRgn(hdc, (HRGN)m_hObj);
}

bool URegion::frame( HDC hdc, HBRUSH hbr, int nWidth, int nHeight )
{
    return TRUE == ::FrameRgn(hdc, (HRGN)m_hObj, hbr, nWidth, nHeight);
}

int URegion::getPolyFillMode( HDC hdc )
{
    return ::GetPolyFillMode(hdc);
}

int URegion::setPolyFillMode( HDC hdc, int nMode )
{
    return ::SetPolyFillMode(hdc, nMode);
}

DWORD URegion::getData( DWORD dwCount, LPRGNDATA lpRgnData )
{
    return ::GetRegionData((HRGN)m_hObj, dwCount, lpRgnData);
}

/***************************************************/
/*                David Gallardo Llopis               */
/*                                                    */
/*             Based on the code in the book           */
/*       PROGRAMACION AVANZADA EN WINDOWS 2000     */
/*             at McGraw-Hill (c) 2000             */
/*                       by                        */
/*        J. Pascual, F. Charte, M.J. Segarra,     */
/*            J.A. Clavijo, A. de Antonio.         */
/*                                                 */
/*  The code in this book is based on an original  */
/*       code by Jean-Edouard Lachand-Robert       */
/***************************************************/

HRGN URegion::bitmapRegion(HBITMAP hBitmap,COLORREF cTransparentColor,BOOL bIsTransparent)
{
    HRGN hRegion = NULL;

    // If the passed bitmap is NULL, go away!
    if(!hBitmap)
    {
        return NULL;
    }

    // We create a memory context for working with the bitmap
    // The memory context is compatible with the display context (screen)
    HDC hMemDC = ::CreateCompatibleDC(NULL);

    // If no context is created, go away, too!
    if(!hMemDC)
    {
        return NULL;
    }

    // Computation of the bitmap size
    BITMAP bmBitmap;

    ::GetObject(hBitmap, sizeof(bmBitmap), &bmBitmap);

    // In order to make the space for the region, we
    // create a bitmap with 32bit depth color and with the
    // size of the loaded bitmap!
    BITMAPINFOHEADER RGB32BITSBITMAPINFO =
    {
        sizeof(BITMAPINFOHEADER),
        bmBitmap.bmWidth,
        bmBitmap.bmHeight,
        1,
        32,
        BI_RGB,
        0,
        0,
        0,
        0,
        0
    };

    // Here is the pointer to the bitmap data
    VOID *pBits;

    // With the previous information, we create the new bitmap!
    HBITMAP hNewBitmap;
    hNewBitmap = ::CreateDIBSection(hMemDC,
                     (BITMAPINFO *)&RGB32BITSBITMAPINFO,
                     DIB_RGB_COLORS,
                     &pBits,
                     NULL,
                     0);

    // If the creation process succeded...
    if(hNewBitmap)
    {
        // We select the bitmap onto the created memory context
        // and then we store the previosly selected bitmap on this context!
        HBITMAP hPrevBmp = (HBITMAP)::SelectObject(hMemDC, hNewBitmap);

        // We create another device context compatible with the first!
        HDC hDC = ::CreateCompatibleDC(hMemDC);

        // If success...
        if(hDC)
        {
            // We compute the number of bytes per row that the bitmap contains, rounding to 32 bit-multiples
            BITMAP bmNewBitmap;

            ::GetObject(hNewBitmap, sizeof(bmNewBitmap), &bmNewBitmap);

            while(bmNewBitmap.bmWidthBytes % 4)
            {
                bmNewBitmap.bmWidthBytes++;
            }

            // Copy of the original bitmap on the memory context!
            HBITMAP  hPrevBmpOrg = (HBITMAP)::SelectObject(hDC, hBitmap);
            ::BitBlt(hMemDC, 0, 0, bmBitmap.bmWidth, bmBitmap.bmHeight, hDC, 0, 0, SRCCOPY);

            // In order to optimize the code, we don't call the GDI each time we
            // find a transparent pixel. We use a RGN_DATA structure were we store
            // consecutive rectangles, until we have a large amount of them and then we crete
            // the composed region with ExtCreateRgn(), combining it with the main region.
            // Then we begin again initializing the RGN_DATA structure and doing another
            // iteration, until the entire bitmap is analyzed.

            // Also, in order to not saturate the Windows API with calls for reserving
            // memory, we wait until NUMRECT rectangles are stores in order to claim
            // for another NUMRECT memory space!
            #define NUMRECT    100
            DWORD maxRect = NUMRECT;

            // We create the memory data
            HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect));
            RGNDATA *pData= (RGNDATA*)::GlobalLock(hData);
            pData->rdh.dwSize = sizeof(RGNDATAHEADER);
            pData->rdh.iType = RDH_RECTANGLES;
            pData->rdh.nCount = pData->rdh.nRgnSize = 0;
            ::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

            // We study each pixel on the bitmap...
            BYTE *Pixeles=(BYTE*) bmNewBitmap.bmBits+(bmNewBitmap.bmHeight-1)*bmNewBitmap.bmWidthBytes;

            // Main loop
            for(int Row=0;Row<bmBitmap.bmHeight;Row++)
            {
                // Horizontal loop
                for(int Column=0;Column<bmBitmap.bmWidth;Column++)
                {
                    // We optimized searching for adyacent transparent pixels!
                    int Xo=Column;
                    LONG *Pixel=(LONG*) Pixeles+Column;

                    while(Column<bmBitmap.bmWidth)
                    {
                        BOOL bInRange=FALSE;

                        // If the color is that indicated as transparent...
                        if(    GetRValue(*Pixel)==GetRValue(cTransparentColor) &&
                            GetGValue(*Pixel)==GetGValue(cTransparentColor) &&
                            GetBValue(*Pixel)==GetBValue(cTransparentColor) )
                            bInRange=TRUE;

                        if((bIsTransparent) && (bInRange))
                            break;

                        if((!bIsTransparent) && (!bInRange))
                            break;

                        Pixel++;
                        Column++;
                    } // while (Column < bm.bmWidth)

                    if(Column>Xo)
                    {
                        // We add the rectangle (Xo,Row),(Column,Row+1) to the region

                        // If the number of rectangles is greater then NUMRECT, we claim
                        // another pack of NUMRECT memory places!
                        if (pData->rdh.nCount>=maxRect)
                        {
                            GlobalUnlock(hData);
                            maxRect+=NUMRECT;
                            hData=GlobalReAlloc(hData,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),GMEM_MOVEABLE);
                            pData=(RGNDATA *)GlobalLock(hData);
                        }

                        RECT *pRect=(RECT*) &pData->Buffer;
                        SetRect(&pRect[pData->rdh.nCount],Xo,Row,Column,Row+1);

                        if(Xo<pData->rdh.rcBound.left)
                            pData->rdh.rcBound.left=Xo;

                        if(Row<pData->rdh.rcBound.top)
                            pData->rdh.rcBound.top=Row;

                        if(Column>pData->rdh.rcBound.right)
                            pData->rdh.rcBound.right=Column;

                        if(Row+1>pData->rdh.rcBound.bottom)
                            pData->rdh.rcBound.bottom=Row+1;

                        pData->rdh.nCount++;

                        // In Win95/08 there is a limitation on the maximum number of
                        // rectangles a RGN_DATA can store (aprox. 4500), so we call
                        // the API for a creation and combination with the main region
                        // each 2000 rectangles. This is a good optimization, because
                        // instead of calling the routines for combining for each new
                        // rectangle found, we call them every 2000 rectangles!!!
                        if(pData->rdh.nCount==2000)
                        {
                            HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRect),pData);
                            if (hNewRegion) {
                                // Si ya existe la región principal,sumamos la nueva,
                                // si no,entonces de momento la principal coincide con
                                // la nueva región.
                                if (hRegion) {
                                    CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);
                                    DeleteObject(hNewRegion);
                                } else
                                    hRegion=hNewRegion;


                            }
                            // Volvemos a comenzar la suma de rectángulos
                            pData->rdh.nCount=0;
                            SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
                        }

                    } // if (Column > Xo)
                } // for (int  Column ...)

                // Nueva Row. Lo del negativo se debe a que el bitmap est?invertido
                // verticalmente.
                Pixeles -= bmNewBitmap.bmWidthBytes;

            } // for (int Row...)

            // Una vez finalizado el proceso,procedemos a la fusión de la
            // región remanente desde la última fusión hasta el final
            HRGN hNewRegion = ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),pData);

            if(hNewRegion)
            {
                // If the main region does already exist, we add the new one,
                if(hRegion)
                {
                    CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);
                    DeleteObject(hNewRegion);
                }
                else
                    // if not, we consider the new one to be the main region at first!
                    hRegion=hNewRegion;
            }

            // We free the allocated memory and the rest of used ressources
            GlobalFree(hData);
            SelectObject(hDC,hPrevBmpOrg);
            DeleteDC(hDC);

        }// if (hDC)

        SelectObject(hMemDC,hPrevBmp);
        DeleteDC(hMemDC);
    } //if (hNewBitmap)

    setRgn(hRegion);

    return *this;
}
