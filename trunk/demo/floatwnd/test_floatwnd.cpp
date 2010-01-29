#include "resource.h"

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT  0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uicon.h"
#include "ustatic.h"
#include "umsg.h"
#include "umenu.h"
#include "uregion.h"
#include "ubitmap.h"
#include "colors.h"

HRGN BitmapRegion(HBITMAP hBitmap,COLORREF cTransparentColor=RGB(0,0,0),BOOL bIsTransparent=TRUE);

using huys::UDialogBox;

class UIconStatic : public UStatic
{
public:
    UIconStatic(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_ICON | SS_REALSIZEIMAGE;
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
    }

    BOOL setIcon(UINT nID)
    {
        this->sendMsg(STM_SETICON, (WPARAM)LoadIcon(m_hInstance, MAKEINTRESOURCE(nID)));
        return TRUE;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);

        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }
};

class UDialogExt : public UDialogBox
{
    enum {
        IDC_ST_ICON = 1021
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pIconStatic(0)
    {

    }

    ~UDialogExt()
    {
        CHECK_PTR(m_pIconStatic);
    }

    BOOL create()
    {

        //m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);
        //this->setLong();
        ::CreateDialogParam(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc, (LPARAM)this);

        //assert(NULL != m_hDlg);

        this->show();

        return TRUE;
    }

    virtual BOOL onInit()
    {
        //m_pIconStatic = new UIconStatic(m_hDlg, IDC_ST_ICON, m_hInst);
        //m_pIconStatic->create();

        //RECT rc = {0, 0, 32, 32};
        //m_pIconStatic->setPosition(&rc);

        //m_pIconStatic->setIcon(IDI_APP);

        ::MoveWindow(m_hDlg, 0, 0, 500, 250, TRUE);
        ::SetWindowPos(m_hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        this->modifyExStyles(WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(m_hDlg, 0, 129,LWA_ALPHA);

        /*
          When you create a window, the taskbar examines the window's
          extended style to see if either the WS_EX_APPWINDOW (&H40000)
          or WS_EX_TOOLWINDOW (&H80) style is turned on. If WS_EX_APPWINDOW
          is turned on, the taskbar shows a button for the window, and if
          WS_EX_ TOOLWINDOW is turned on, the taskbar does not show a button
          for the window. A window should never have both of these extended styles.
          If the window doesn't have either of these styles, the taskbar decides to
          create a button if the window is unowned and does not create a button if
          the window is owned.
         */
        this->hide();
        this->modifyExStyles(WS_EX_TOOLWINDOW);
        this->show();

        //static UBitmap ubm(IDB_ICON, m_hInst);
        m_ubm.loadFromResource(IDB_ICON, m_hInst);
        m_urgn = BitmapRegion(m_ubm, RGB(0,0,0));
        this->setWindowRgn(m_urgn);

        return TRUE;
    }

    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        if (NULL != m_ubm.getHandle())
        {
            RECT rc;
            ::GetClientRect(m_hDlg, &rc);
            m_ubm.show(hdc, rc);
            return TRUE;
        }
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(LOWORD (wParam))
        {
        case IDM_CLOSE:
            {
                this->onCancel();
                ::PostQuitMessage (0);
            }
            return FALSE;
        case IDM_SAYHI:
            return this->onSayHi();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::PostMessage(m_hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return FALSE;
    }

    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam)
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);


        POINT point = {xPos, yPos};
        ClientToScreen(m_hDlg, &point);

        UMenu um;
        um.load(m_hInst, MAKEINTRESOURCE(IDR_MENU1));
        um.attach(m_hDlg);
        um.trackSubPopup(1, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);

        return FALSE;
    }

private:
    UIconStatic *m_pIconStatic;

    BOOL onSayHi()
    {
        showMsg(_T("Hi"), _T("Info"), m_hDlg);
        return FALSE;
    }

    URegion m_urgn;
    UBitmap m_ubm;
};

class UDialogHide : public UDialogBox
{
public:
    UDialogHide(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_dlg(m_hInst, IDD_TEST)
    {}

    BOOL create()
    {

        //m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);
        //this->setLong();
        ::CreateDialogParam(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc, (LPARAM)this);

        //assert(NULL != m_hDlg);

        //this->hide();

        if (!m_hParent)
        {
            return this->go();
        }
        else
        {
            return 0;
        }
    }

    virtual BOOL onInit()
    {
        this->hide();
        m_dlg.setParentH(m_hParent);
        m_dlg.create();
        return TRUE;
    }
private:
    UDialogExt m_dlg;
};

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

HRGN BitmapRegion(HBITMAP hBitmap,COLORREF cTransparentColor,BOOL bIsTransparent)
{
    // We create an empty region
    HRGN        hRegion=NULL;

    // If the passed bitmap is NULL, go away!
    if(!hBitmap)
        return hRegion;

    // We create a memory context for working with the bitmap
    // The memory context is compatible with the display context (screen)
    HDC            hMemDC=CreateCompatibleDC(NULL);

    // If no context is created, go away, too!
    if(!hMemDC)
        return hRegion;

    // Computation of the bitmap size
    BITMAP        bmBitmap;

    GetObject(hBitmap, sizeof(bmBitmap), &bmBitmap);

    // In order to make the space for the region, we
    // create a bitmap with 32bit depth color and with the
    // size of the loaded bitmap!
    BITMAPINFOHEADER RGB32BITSBITMAPINFO=
    {
        sizeof(BITMAPINFOHEADER),
        bmBitmap.bmWidth,
        bmBitmap.bmHeight,
        1,32,BI_RGB,0,0,0,0,0
    };

    // Here is the pointer to the bitmap data
    VOID        *pBits;

    // With the previous information, we create the new bitmap!
    HBITMAP        hNewBitmap;
    hNewBitmap=CreateDIBSection(hMemDC,
                                (BITMAPINFO *)&RGB32BITSBITMAPINFO,
                                DIB_RGB_COLORS,&pBits,NULL,0);

    // If the creation process succeded...
    if(hNewBitmap)
    {
        // We select the bitmap onto the created memory context
        // and then we store the previosly selected bitmap on this context!
        HBITMAP        hPrevBmp=(HBITMAP) SelectObject(hMemDC,hNewBitmap);

        // We create another device context compatible with the first!
        HDC            hDC=CreateCompatibleDC(hMemDC);

        // If success...
        if(hDC)
        {
            // We compute the number of bytes per row that the bitmap contains, rounding to 32 bit-multiples
            BITMAP        bmNewBitmap;

            GetObject(hNewBitmap,sizeof(bmNewBitmap),&bmNewBitmap);

            while(bmNewBitmap.bmWidthBytes % 4)
                bmNewBitmap.bmWidthBytes++;

            // Copy of the original bitmap on the memory context!
            HBITMAP        hPrevBmpOrg=(HBITMAP) SelectObject(hDC,hBitmap);
            BitBlt(hMemDC,0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,hDC,0,0,SRCCOPY);

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
            HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect));
            RGNDATA *pData=(RGNDATA*) GlobalLock(hData);
            pData->rdh.dwSize=sizeof(RGNDATAHEADER);
            pData->rdh.iType=RDH_RECTANGLES;
            pData->rdh.nCount=pData->rdh.nRgnSize=0;
            SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);

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
            HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),pData);

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

    return hRegion;
}

UDLGAPP_T(UDialogHide, IDD_TEST);
