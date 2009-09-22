#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>

#include "udialogx.h"
#include "udlgapp.h"

#include "uedit.h"
#include "ubutton.h"
#include "umsg.h"

#include "ukeybd.h"

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
    enum {
        ID_EDIT_TEST = 1115,
        ID_BUTTON_TEST = 1116,
        ID_BUTTON_RESULT = 1117  
    };
public:
    MyDialog(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~MyDialog()
    {
        if (m_pEdtTest)
        {
            delete m_pEdtTest;
            m_pEdtTest = NULL;
        }

        if (m_pBtnTest)
        {
            delete m_pBtnTest;
            m_pBtnTest = NULL;
        }
    }

    virtual BOOL onInit() {
        RECT rc = {20, 20, 520, 220};

        m_pEdtTest = new UEdit(m_hDlg, ID_EDIT_TEST, m_hInst);
        m_pEdtTest->setRect(&rc);
        m_pEdtTest->create();

        rc.top += 260;
        rc.bottom += 160;
        rc.left = 200;
        rc.right = rc.left + 150;
        m_pBtnTest = new UPushButton(m_hDlg, ID_BUTTON_TEST, m_hInst);
        m_pBtnTest->setRect(&rc);
        m_pBtnTest->create();
        m_pBtnTest->setWindowText(_T("NumLock On"));

        rc.left = 20;
        rc.right = 170;
        m_pBtnResult = new UPushButton(m_hDlg, ID_BUTTON_RESULT, m_hInst);
        m_pBtnResult->setRect(&rc);
        m_pBtnResult->create();
        m_pBtnResult->setWindowText(_T("NumLock Off"));

        return TRUE;
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(wParam)
        {
        case ID_BUTTON_TEST:
            UKeyboard::setNumLock();
            return FALSE;
        case ID_BUTTON_RESULT:
            UKeyboard::setNumLock(FALSE);
            return FALSE;
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UPushButton *m_pBtnTest;
    UPushButton *m_pBtnResult;
    UEdit *m_pEdtTest;
};

UDLGAPP_T(MyDialog, IDD_DIALOG1);

