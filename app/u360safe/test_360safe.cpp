#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "ubitmap.h"
#include "ubutton.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialog360 : public UDialogBox
{
private:
    enum {
        IDC_BN_PROTECT = 500,
        IDC_BN_STATE
    };
public:
    UDialog360(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
    m_ubmProtect (NULL),
    m_bnProtect(NULL),
    m_ubmState(NULL),
    m_bnState(NULL)
    {}

    ~UDialog360()
    {
        CHECK_PTR(m_ubmProtect);
        CHECK_PTR(m_bnProtect);
        CHECK_PTR(m_ubmState);
        CHECK_PTR(m_bnState);
        CHECK_PTR(m_ubmProtectFocus);
        CHECK_PTR(m_ubmStateFocus);
    }

    virtual BOOL onInit()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);

        m_ubmProtect = new UBitmap(IDB_PROTECT_NORMAL, m_hInst);
        m_ubmState = new UBitmap(IDB_STATE_NORMAL, m_hInst);

        m_ubmProtectFocus = new UBitmap(IDB_PROTECT_FOCUS, m_hInst);
        m_ubmStateFocus = new UBitmap(IDB_STATE_FOCUS, m_hInst);

        //RECT rc = {20, 20, 120, 120};

        //rc.right = rc.left + m_ubmProtect->getWidth();
        //rc.bottom = rc.top + m_ubmProtect->getHeight();

        //m_bnProtect = new UBitmapButton(m_hDlg, IDC_BN_PROTECT, m_hInst);
        //m_bnProtect->setStyles(BS_FLAT|BS_PUSHLIKE);
        //m_bnProtect->create();
        //m_bnProtect->setBitmap(m_ubmProtect->getHandle());
        //m_bnProtect->setPosition(&rc);


        //rc.left = 150;
        //rc.right = rc.left + m_ubmState->getWidth();
        //rc.bottom = rc.top + m_ubmState->getHeight();

        //m_bnState = new UBitmapButton(m_hDlg, IDC_BN_PROTECT, m_hInst);
        //m_bnState->setStyles(BS_FLAT|BS_PUSHLIKE);
        //m_bnState->create();
        //m_bnState->setBitmap(m_ubmState->getHandle());
        //m_bnState->setPosition(&rc);

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        HBRUSH hBrushBK = ::CreateSolidBrush(huys::aliceblue);

        RECT rc = {20, 20, 120, 120};

        rc.right = rc.left + m_ubmProtect->getWidth();
        rc.bottom = rc.top + m_ubmProtect->getHeight();

        RECT rc2 = {
            rc.right,
            rc.top,
            rc.right + m_ubmState->getWidth(),
            rc.bottom
        };

        switch (message)
        {
        case WM_CTLCOLORDLG:
            return (BOOL)hBrushBK;
        case WM_MOUSEMOVE:
            {
                long xPos = GET_X_LPARAM(lParam);
                long yPos = GET_Y_LPARAM(lParam);

                POINT pt = {xPos, yPos};

                if (PtInRect(&rc, pt))
                {
                    HDC hdc = ::GetDC(m_hDlg);
                    m_ubmProtectFocus->show(hdc, rc);
                }
                else if (PtInRect(&rc2, pt))
                {
                    HDC hdc = ::GetDC(m_hDlg);
                    m_ubmStateFocus->show(hdc, rc2);
                }
                else
                {
                    ::InvalidateRect(m_hDlg, 0, 0);
                }
            }
        }
        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hDlg, &ps);

        RECT rc = {20, 20, 120, 120};

        rc.right = rc.left + m_ubmProtect->getWidth();
        rc.bottom = rc.top + m_ubmProtect->getHeight();

        m_ubmProtect->show(hdc, rc);

        rc.left = rc.right;
        rc.right = rc.left + m_ubmState->getWidth();
        rc.bottom = rc.top + m_ubmState->getHeight();

        m_ubmState->show(hdc, rc);

        EndPaint(m_hDlg, &ps);
        return TRUE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        RECT rc = {20, 20, 120, 120};

        rc.right = rc.left + m_ubmProtect->getWidth();
        rc.bottom = rc.top + m_ubmProtect->getHeight();

        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);

        POINT pt = {xPos, yPos};

        if (PtInRect(&rc, pt))
        {
            showMsg(_T("Hi...."), _T("gotcha"), m_hDlg);
        }

        return FALSE;
    }
private:
    UBitmap *m_ubmProtect;
    UBitmap *m_ubmProtectFocus;
    UBitmapButton *m_bnProtect;
    UBitmap *m_ubmState;
    UBitmap *m_ubmStateFocus;
    UBitmapButton *m_bnState;
};

//BEGIN_DLGAPP
//  UDialog360 dlg(hInstance, IDD_360SAFE);
//  dlg.create();
//END_DLGAPP

UDLGAPP_T(UDialog360, IDD_360SAFE);
