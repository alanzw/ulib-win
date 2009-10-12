/*
 * =====================================================================================
 *
 *       Filename:  test_sysadm.cpp
 *
 *    Description:  A little kit for system admin
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:17:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udlgapp.h"
#include "udialogx.h"
#include "uctrls.h"
#include "ucollapse.h"

using huys::UDialogBox;

class UDialogCtrls : public UDialogBox
{
public:
    UDialogCtrls(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pCtrlFactory(0),
        m_pgbox(0),
        m_pButton(0)
    {}

    ~UDialogCtrls()
    {
        CHECK_PTR(m_pCtrlFactory);
        CHECK_PTR(m_pgbox);
        CHECK_PTR(m_pButton);
    }

    BOOL onInit()
    {
        // Sub Controls should be created before UCollapseGroupBox
        m_pButton = new UButton(m_hDlg, 2222, m_hInst);
        m_pButton->setPos(250, 250, 30, 30);
        m_pButton->create();
        m_pButton->setWindowText(_T("S"));

        m_pgbox = new UCollapseGroupBox(m_hDlg, 1111, m_hInst);
        m_pgbox->setPos(200, 200, 100, 100);
        m_pgbox->create();
        m_pgbox->setTitle("GBox");

        m_pgbox->addSubCtrl(*m_pButton);

        ucgbox.subclass(IDC_CGBOX, m_hDlg);
        ucgbox.setTitle("GBox");
        ucgbox.addSubCtrl(::GetDlgItem(m_hDlg, IDC_BUTTON1));
        ucgbox.addSubCtrl(::GetDlgItem(m_hDlg, IDC_EDIT1));
        return TRUE;
    }

    BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        if (0 == m_pCtrlFactory)
        {
            m_pCtrlFactory = new UButtonFactory;
            RECT rc = {100, 100, 200, 200};
            m_pCtrlFactory->create(m_hDlg, 333, TEXT("Button"), &rc);
        }
        return FALSE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        if (0 == m_pCtrlFactory)
        {
            m_pCtrlFactory = new UStaticFactory;
            RECT rc = {100, 100, 200, 200};
            m_pCtrlFactory->create(m_hDlg, 333, TEXT("Static"), &rc);
        }
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch ( LOWORD(wParam) )
        {
        case 2222:
            {
                showMsg(_T("S"), _T("info"), m_hDlg);
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UCtrlFactory *m_pCtrlFactory;
    UCollapseGroupBox ucgbox;
    UCollapseGroupBox *m_pgbox;
    UButton *m_pButton;
};

UDLGAPP_T(UDialogCtrls, IDD_CTRLS);


