#include "resource.h"

#define  WINVER   0x0501

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubitmap.h"
#include "umsg.h"

using huys::UDialogBox;

void DrawAlphaBlend (HWND hWnd, HDC hdcwnd)
{
    HDC hdc;               // handle of the DC we will create
    BLENDFUNCTION bf;      // structure for alpha blending
    HBITMAP hbitmap;       // bitmap handle
    BITMAPINFO bmi;        // bitmap header
    VOID *pvBits;          // pointer to DIB section
    ULONG   ulWindowWidth, ulWindowHeight;      // window width/height
    ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height
    RECT    rt;            // used for getting window dimensions
    UINT32   x,y;          // stepping variables
    UCHAR ubAlpha;         // used for doing transparent gradient
    UCHAR ubRed;
    UCHAR ubGreen;
    UCHAR ubBlue;
    float fAlphaFactor;    // used to do premultiply

    // get window dimensions
    GetClientRect(hWnd, &rt);

    // calculate window width/height
    ulWindowWidth = rt.right - rt.left;
    ulWindowHeight = rt.bottom - rt.top;

    // make sure we have at least some window size
    if ((!ulWindowWidth) || (!ulWindowHeight))
        return;

    // divide the window into 3 horizontal areas
    ulWindowHeight = ulWindowHeight / 3;

    // create a DC for our bitmap -- the source DC for AlphaBlend
    hdc = CreateCompatibleDC(hdcwnd);

    // zero the memory for the bitmap info
    ZeroMemory(&bmi, sizeof(BITMAPINFO));

    // setup bitmap info
    // set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas.
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth - (ulWindowWidth/5)*2;
    bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight - (ulWindowHeight/5)*2;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

    // create our DIB section and select the bitmap into the dc
    hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
    SelectObject(hdc, hbitmap);

    // in top window area, constant alpha = 50%, but no source alpha
    // the color format for each pixel is 0xaarrggbb
    // set all pixels to blue and set source alpha to zero
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
            ((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x000000ff;

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 0x7f;  // half of 0xff = 50% transparency
    bf.AlphaFormat = 0;             // ignore source alpha channel

    if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5,
                    ulBitmapWidth, ulBitmapHeight,
                    hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
        return;                     // alpha blend failed

    // in middle window area, constant alpha = 100% (disabled), source
    // alpha is 0 in middle of bitmap and opaque in rest of bitmap
    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
            if ((x > (int)(ulBitmapWidth/5)) && (x < (ulBitmapWidth-ulBitmapWidth/5)) &&
                (y > (int)(ulBitmapHeight/5)) && (y < (ulBitmapHeight-ulBitmapHeight/5)))
                //in middle of bitmap: source alpha = 0 (transparent).
                // This means multiply each color component by 0x00.
                // Thus, after AlphaBlend, we have a, 0x00 * r,
                // 0x00 * g,and 0x00 * b (which is 0x00000000)
                // for now, set all pixels to red
                ((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x00ff0000;
            else
                // in the rest of bitmap, source alpha = 0xff (opaque)
                // and set all pixels to blue
                ((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0xff0000ff;
            //endif;

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha
    bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha)

    if (!AlphaBlend(hdcwnd, ulWindowWidth/5, ulWindowHeight/5+ulWindowHeight, ulBitmapWidth, ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
        return;

    // bottom window area, use constant alpha = 75% and a changing
    // source alpha. Create a gradient effect using source alpha, and
    // then fade it even more with constant alpha
    ubRed = 0x00;
    ubGreen = 0x00;
    ubBlue = 0xff;

    for (y = 0; y < ulBitmapHeight; y++)
        for (x = 0; x < ulBitmapWidth; x++)
        {
            // for a simple gradient, base the alpha value on the x
            // value of the pixel
            ubAlpha = (UCHAR)((float)x / (float)ulBitmapWidth * 255);
            //calculate the factor by which we multiply each component
            fAlphaFactor = (float)ubAlpha / (float)0xff;
            // multiply each pixel by fAlphaFactor, so each component
            // is less than or equal to the alpha value.
            ((UINT32 *)pvBits)[x + y * ulBitmapWidth]
                = (ubAlpha << 24) |                       //0xaa000000
                 ((UCHAR)(ubRed * fAlphaFactor) << 16) |  //0x00rr0000
                 ((UCHAR)(ubGreen * fAlphaFactor) << 8) | //0x0000gg00
                 ((UCHAR)(ubBlue   * fAlphaFactor));      //0x000000bb
        }

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;   // use source alpha
    bf.SourceConstantAlpha = 0xbf;   // use constant alpha, with
                                     // 75% opaqueness

    AlphaBlend(hdcwnd, ulWindowWidth/5,
               ulWindowHeight/5+2*ulWindowHeight, ulBitmapWidth,
               ulBitmapHeight, hdc, 0, 0, ulBitmapWidth,
               ulBitmapHeight, bf);

    // do cleanup
    DeleteObject(hbitmap);
    DeleteDC(hdc);

}

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}


    BOOL onInit()
    {
        m_ubmBack.loadFromResource(IDB_BACKGROUND, m_hInst);

    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        HDC hdc;
        PAINTSTRUCT ps;
        RECT rc = {100, 20, 200, 120};
        RECT rcClose = {0};
        RECT rcMin = {0};
        RECT rcMax = {0};
        RECT rcRes = {0};
        static UBitmap ubm(IDB_EXIT_NORMAL, m_hInst);
        static UBitmap ubmo(IDB_EXIT_FOCUS, m_hInst);
        static UBitmap ubmm(IDB_MIN_NORMAL, m_hInst);
        static UBitmap ubmmo(IDB_MIN_FOCUS, m_hInst);
        static UBitmap ubmmm(IDB_MAX_NORMAL, m_hInst);
        static UBitmap ubmmmo(IDB_MAX_FOCUS, m_hInst);
        static UBitmap ubmres(IDB_RESTORE_NORMAL, m_hInst);
        static UBitmap ubmreso(IDB_RESTORE_FOCUS, m_hInst);
        static UBitmap ubmres_dea(IDB_RESTORE_DEACTIVE, m_hInst);

        if (message == WM_PAINT)
        {
            hdc = BeginPaint( m_hDlg, &ps );
            ubm.show(hdc, rc);
            EndPaint( m_hDlg, &ps );
        }

        if (    message == WM_SYSCOMMAND
                || message == WM_NCPAINT
                || message == WM_INITMENUPOPUP
                || message == WM_NCACTIVATE
                || message == WM_SETTEXT
                || message == WM_WINDOWPOSCHANGING
                || message == WM_SIZE
                || message == WM_SIZING
                || message == WM_NCHITTEST
                || message == WM_NCCALCSIZE
                || message == WM_NCLBUTTONUP
                || message == WM_NCLBUTTONDOWN
                || message == WM_NCLBUTTONDBLCLK
                || message == WM_NCRBUTTONUP
                || message == WM_NCRBUTTONDOWN
                || message == WM_NCMOUSEMOVE)
        {
            //hdc = GetWindowDC(m_hDlg);
            //get rectangle of window, and calculate windows
            RECT WindowRect;
            GetWindowRect(m_hDlg, &WindowRect);
            rcClose.left = WindowRect.right - WindowRect.left - ubm.getWidth()-5;
            rcClose.right = WindowRect.right - WindowRect.left-5;
            rcClose.top = 5;
            rcClose.bottom = 30;
            //ubm.show(hdc, rcClose);
            //ReleaseDC(m_hDlg, hdc);
            draw(ubm, rcClose);

            rcMin.left = rcClose.left - ubm.getWidth() - 5;
            rcMin.right = rcClose.left - 5;
            rcMin.top = rcClose.top;
            rcMin.bottom = rcClose.bottom;

            draw(ubmm, rcMin);

            rcMax.left = rcMin.left - ubm.getWidth() - 5;
            rcMax.right = rcMin.left - 5;
            rcMax.top = rcMin.top;
            rcMax.bottom = rcMin.bottom;

            draw(ubmmm, rcMax);

            rcRes.left = rcMax.left - ubm.getWidth() - 5;
            rcRes.right = rcMax.left - 5;
            rcRes.top = rcMax.top;
            rcRes.bottom = rcMax.bottom;

            draw(ubmres, rcRes);

        //if ( message == WM_NCLBUTTONDOWN )
        //{
            //get rectangle of window, and calculate windows
        //    RECT WindowRect;
        //    GetWindowRect(m_hDlg, &WindowRect);
            RECT rcButtonClose =
            {
                WindowRect.left + rcClose.left,
                WindowRect.top + rcClose.top,
                WindowRect.left + rcClose.right,
                WindowRect.top + rcClose.bottom
            };
            //::ClientToScreen(m_hDlg, (LPPOINT)&rcButtonClose);
            //::ClientToScreen(m_hDlg, ((LPPOINT)&rcButtonClose)+1);

            POINT pt = {
                GET_X_LPARAM(lParam),
                GET_Y_LPARAM(lParam)
            };

            //::ClientToScreen(m_hDlg, (LPPOINT)&rcButtonClose);

           //showMsgFormat("Info", "pt: ( %d, %d )", pt.x, pt.y);
           //showMsgFormat("Info", "RECT: ( %d, %d ), ( %d, %d )", rcButtonClose.left,
           //        rcButtonClose.top, rcButtonClose.right, rcButtonClose.bottom );
           if (PtInRect(&rcButtonClose, pt))
           {
                //this->onCancel();
                //showMsgFormat("Info", "pt: ( %d, %d )", pt.x, pt.y);
                draw(ubmo, rcClose);
                if (message == WM_NCLBUTTONDOWN)
                    this->onCancel();
           }

           RECT rcButtonMin =
           {
                WindowRect.left + rcMin.left,
                WindowRect.top + rcMin.top,
                WindowRect.left + rcMin.right,
                WindowRect.top + rcMin.bottom
           };
           if (PtInRect(&rcButtonMin, pt))
           {
                //this->onCancel();
                //showMsgFormat("Info", "pt: ( %d, %d )", pt.x, pt.y);
                draw(ubmmo, rcMin);
                if (message == WM_NCLBUTTONDOWN)
                    ::ShowWindow(m_hDlg, SW_MINIMIZE);
           }

           RECT rcButtonMax =
           {
                WindowRect.left + rcMax.left,
                WindowRect.top + rcMax.top,
                WindowRect.left + rcMax.right,
                WindowRect.top + rcMax.bottom
           };
           if (PtInRect(&rcButtonMax, pt))
           {
                //this->onCancel();
                //showMsgFormat("Info", "pt: ( %d, %d )", pt.x, pt.y);
                draw(ubmmmo, rcMax);
                if (message == WM_NCLBUTTONDOWN)
                    ::ShowWindow(m_hDlg, SW_MAXIMIZE);
           }

           RECT rcButtonRes =
           {
                WindowRect.left + rcRes.left,
                WindowRect.top + rcRes.top,
                WindowRect.left + rcRes.right,
                WindowRect.top + rcRes.bottom
           };
           if (PtInRect(&rcButtonRes, pt))
           {
                //this->onCancel();
                //showMsgFormat("Info", "pt: ( %d, %d )", pt.x, pt.y);
                draw(ubmreso, rcRes);
                if (message == WM_NCLBUTTONDOWN)
                    ::ShowWindow(m_hDlg, SW_RESTORE);
           }
    }

        return result;
    }

    void onDraw(HDC hdc)
    {
        DrawAlphaBlend(m_hDlg, hdc);

        RECT rc = {20, 400, 320, 600};
        //m_ubmBack.showExtend(hdc, &rc, 20, 0);
    }

/*
    BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        m_ubmBack.showStretch(hdc, rc);
        return TRUE;
    }
*/
private:
    UBitmap m_ubmBack;
private:
    void draw(UBitmap &ubmz, RECT &rc)
    {
        HDC hdc;
        hdc = GetWindowDC(m_hDlg);
        ubmz.show(hdc, rc);
        ReleaseDC(m_hDlg, hdc);
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);
