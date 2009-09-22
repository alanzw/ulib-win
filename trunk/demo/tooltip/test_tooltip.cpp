#include "resource.h"

#define _WIN32_IE 0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "utooltip.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "umsg.h"

#include "uctrlmsg.h"

using huys::UDialogBox;

//
class MyDialog : public UDialogBox
{
    enum {
        ID_TOOLTIP_CTRL = 0,
        ID_BN_TEST      = 10116
    };
public:
    MyDialog(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pttp(0),
          m_pbtn(0),
          m_bShow(FALSE)
    {}

    ~MyDialog()
    {
        CHECK_PTR(m_pttp);
        CHECK_PTR(m_pbtn);
    }

    BOOL onInit()
    {
        m_pbtn = new UButton(m_hDlg, ID_BN_TEST, m_hInst);
        RECT rc = {200, 200, 380, 250};
        m_pbtn->create();
        m_pbtn->setPosition(&rc);
        m_pbtn->setWindowText(_T("Show Tooltip"));

        m_pttp = new UToolTip(m_hDlg, ID_TOOLTIP_CTRL, m_hInst);
        m_pttp->setStyles(WS_CHILD);
        m_pttp->create();
        m_pttp->setTitle(_T("Hello ToolTip"));
        m_pttp->setText(_T("Here we go!"));
        m_pttp->setBKColor(huys::green);
        m_pttp->setTextColor(huys::red);
        m_pttp->deactivate();

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_NOTIFY)
        {
            switch (((LPNMHDR)lParam)->code)
            {
            case UDN_DELTAPOS:
                ::MessageBox(m_hDlg, "xx", "DeltaPos", MB_OK);
                return 0;
            default:
                break;
            }
        }

        return result;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case ID_BN_TEST:
            return onBnTest();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

private:
    UToolTip *m_pttp;
    UButton *m_pbtn;

    BOOL m_bShow;
private:
    BOOL onBnTest()
    {
        if (!m_bShow)
        {
        //showMsg(_T("xxx"));
        RECT rc;
        ::GetWindowRect(m_hDlg, &rc);
        //::ClientToScreen(m_hDlg, (LPPOINT)&rc);
        m_pttp->setXY((WORD)0, (WORD)0);
        m_pttp->showTip();
        m_bShow = TRUE;

        m_pbtn->setWindowText(_T("Hide Tooltip"));
        }
        else
        {
            m_pttp->deactivate();
            m_bShow = FALSE;
            m_pbtn->setWindowText(_T("Show Tooltip"));
        }
        return FALSE;
    }
};

UDLGAPP_T(MyDialog, IDD_TEST);
