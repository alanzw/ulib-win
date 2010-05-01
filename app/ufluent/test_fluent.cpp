 #include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <string>

#include "udialogx.h"
#include "udlgapp.h"
#include "uedit.h"
#include "uthread.h"
#include "uicon.h"
#include "umsg.h"

#include "ufluentman.h"
#include "ugambitman.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

typedef huys::ADT::UStringAnsi TString;

using huys::UDialogBox;

class UDialogFluent : public UDialogBox
{
    enum {
        IDC_EDIT_VIEW = 1111
    };
public:
    UDialogFluent(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}
    

    BOOL onInit()
    {

        m_ico.loadIconEx(m_hInst, IDI_FLUENT);
        this->setDlgIconBig(m_ico.getHICON());

       // m_pBack = new UBitmap(IDB_BACKGROUND, m_hInst);
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        rc.left += 20;
        rc.right -= 20;
        rc.top += 20;
        rc.bottom -= 200;

        m_pEdit = new UEdit(m_hDlg, IDC_EDIT_VIEW, m_hInst);
        m_pEdit->setRect(&rc);
        m_pEdit->setStyles(WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN);
        m_pEdit->create();

        this->addLines("Hello Fluent Guys!\r\n");

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {

        if (message == UFM_FINISHED)
        {
            return onFluentExit();
        }

        if (message == UGM_FINISHED)
        {
            return onGambitExit();
        }

        if (message == UFM_UPDATE)
        {
            return onFluentUpdate((char *)lParam);
        }
        
        if (message == 51111)
        {
            //addLines("-->51111.\r\n");
        }

        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        return result;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BN_GAMBIT:
            return onBnGambit();
        case IDC_BN_FLUENT:
            return onBnFluent();
        case IDC_BN_EXIT:
            return UDialogBox::onCancel();
        case IDC_BN_SENDCMD:
            return onBnSendCmd();
        case IDOK:
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
//    UBitmap *m_pBack;
    huys::ADT::UAutoPtr<UEdit> m_pEdit;

    UIcon m_ico;
    //
    //std::string m_strBuf;
    TString m_strBuf;
    //
    UFluentMan m_ufm;
    UGambitMan m_ugm;
private:
    void addLines(const char *sLines)
    {
        m_strBuf += sLines;
        m_pEdit->setText(m_strBuf);
        m_pEdit->scrollLineDown();
    }

    BOOL onBnGambit()
    {
        addLines("-->Start Gambit.\r\n");
        m_ugm.setTargetWnd(m_hDlg);
        m_ugm.startExe();
        return FALSE;
    }

    BOOL onBnFluent()
    {
        addLines("-->Start Fluent.\r\n");
        m_ufm.setTargetWnd(m_hDlg);
        m_ufm.startExe();
        return FALSE;
    }

    BOOL onFluentExit()
    {
        m_ufm.clearup();
        addLines("-->Fluent exit.\r\n");
        return FALSE;
    }

    BOOL onGambitExit()
    {
        addLines("-->Gambit exit.\r\n");
        //::MessageBox(m_hDlg, "xx", "xx", MB_OK);
        return FALSE;
    }

    BOOL onFluentUpdate(char * result)
    {
        addLines("======================================================\r\n");
        addLines(result);
        addLines("======================================================\r\n");
        return FALSE;
    }
    
    BOOL onBnSendCmd()
    {
       m_ufm.breakCalc();
       m_ufm.sendCmd("(+ 1 1)");
       //Sleep(1000);
       //m_ufm.getResult();
       m_ufm.subclassWnd(m_hDlg);
       //m_ufm.addMenu();
       SetWindowPos(m_hDlg, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
       m_ufm.focusWnd();
       Sleep(500);
       m_ufm.fetchNewResult();
       
    }
};

UDLGAPP_T(UDialogFluent, IDD_FLUENT);

