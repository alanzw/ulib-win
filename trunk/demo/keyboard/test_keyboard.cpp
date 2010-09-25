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
#include "ustatic.h"

#include "ukeybd.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
    enum {
        ID_EDIT_TEST = 1115,
        ID_BUTTON_TEST = 1116,
        ID_BUTTON_RESULT = 1117,
        ID_LABEL_CHAR     = 1118
    };
    
    typedef huys::ADT::UStringAnsi TString; 
public:
    MyDialog(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        RECT rc = {20, 20, 520, 220};

        m_pEdtTest = new UEdit(m_hDlg, ID_EDIT_TEST, m_hInst);
        m_pEdtTest->setRect(&rc);
        //m_pEdtTest->create();

        rc.top += 260;
        rc.bottom += 160;
        rc.left = 200;
        rc.right = rc.left + 150;
        m_pBtnTest = new UPushButton(m_hDlg, ID_BUTTON_TEST, m_hInst);
        m_pBtnTest->setRect(&rc);
        //m_pBtnTest->create();
        //m_pBtnTest->setWindowText(_T("NumLock On"));

        rc.left = 20;
        rc.right = 170;
        m_pBtnResult = new UPushButton(m_hDlg, ID_BUTTON_RESULT, m_hInst);
        m_pBtnResult->setRect(&rc);
        //m_pBtnResult->create();
        //m_pBtnResult->setWindowText(_T("NumLock Off"));
        
        m_pLabelChar = new UStatic(m_hDlg, ID_LABEL_CHAR, m_hInst);
        m_pLabelChar->setPos(400, 300, 100, 50);
        m_pLabelChar->setText(_T(""));
        m_pLabelChar->setStyles(WS_BORDER|SS_SUNKEN);
        m_pLabelChar->create();

        return TRUE;
    }
    
    BOOL onPreTranslateMessage(LPMSG lpMsg)
    {
        if (WM_CHAR == lpMsg->message)
        {
            this->onChar(lpMsg->wParam, lpMsg->lParam);
            return TRUE;
        }
    
        return UDialogBox::onPreTranslateMessage(lpMsg);
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
    
    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        m_pLabelChar->setWindowText(_T(""));
        m_pLabelChar->hide();
        m_pLabelChar->show();
        _buf.format("%ld", wParam);
        m_pLabelChar->setWindowText(_buf);
        
        return UDialogBox::onChar(wParam, lParam);
    }
private:
    huys::ADT::UAutoPtr<UPushButton> m_pBtnTest;
    huys::ADT::UAutoPtr<UPushButton> m_pBtnResult;
    huys::ADT::UAutoPtr<UEdit> m_pEdtTest;
    huys::ADT::UAutoPtr<UStatic> m_pLabelChar;
    
    TString _buf;
};

UDLGAPP_T(MyDialog, IDD_DIALOG1);

