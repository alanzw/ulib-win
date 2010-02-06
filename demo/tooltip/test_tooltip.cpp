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

#include "adt/uautoptr.h"

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
          m_bShow(FALSE)
    {}

    BOOL onInit()
    {
        m_pbtn = new UButton(m_hDlg, ID_BN_TEST, m_hInst);
        m_pbtn->setPos(200, 200, 180, 50);
        m_pbtn->create();
        m_pbtn->setWindowText(_T("Show Tooltip"));

        m_pttp = new UToolTip(m_hDlg, ID_TOOLTIP_CTRL, m_hInst);
        m_pttp->setStyles(WS_CHILD);
        m_pttp->create();
        m_pttp->setTitle(_T("Hello ToolTip"));
        m_pttp->setText(_T("Here we go!"));
        m_pttp->setBKColor(huys::xpblue);
        m_pttp->setTextColor(huys::red);
        m_pttp->deactivate();

        return TRUE;
    }
    
    BOOL onNotify(WPARAM wParam, LPARAM lParam)
    {
        switch (((LPNMHDR)lParam)->code)
        {
        case UDN_DELTAPOS:
            ::MessageBox(m_hDlg, "xx", "DeltaPos", MB_OK);
            return 0;
        default:
            return UDialogBox::onNotify(wParam, lParam);
        }
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
    huys::ADT::UAutoPtr<UToolTip> m_pttp;
    huys::ADT::UAutoPtr<UButton> m_pbtn;

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
