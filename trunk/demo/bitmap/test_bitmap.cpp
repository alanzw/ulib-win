#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubitmap.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}


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
