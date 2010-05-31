#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "umsg.h"

#include "uedit.h"
#include "ustatic.h"
#include "ubutton.h"
#include "ucombobox.h"

#include "adt/uautoptr.h"
#include "aui/aui_label.h"

//#include "upython.h"

const char * g_InterpList[] = {
    "python",
    "perl",
    "tcl",
    "ruby",
    "lua",
    NULL
};

using huys::UDialogBox;

class UDialogScript : public UDialogBox
{
    enum {
        IDC_EDIT_CMD     = 10001,
        IDC_BUTTON_CHECK = 10002,
        IDC_COMBO_INTERP = 10003
    };
public:
    UDialogScript(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}


    virtual BOOL onInit()
    {
        huys::URectI rect;
        
        ::GetClientRect(m_hDlg, rect);
    
        m_pCbInterp = new UComboBox(m_hDlg, IDC_COMBO_INTERP, m_hInst);
        m_pCbInterp->setPos(50, 10, 100, 65);
        m_pCbInterp->create();

        for(int i = 0; g_InterpList[i] != NULL; ++i)
        {
            m_pCbInterp->addText(g_InterpList[i]);
        }

        m_pStaticTitle = new AUI::UTransLabel(m_hDlg, IDC_STATIC, m_hInst);
        m_pStaticTitle->setPos(180, 10, 100, 20);
        m_pStaticTitle->setText(_T("Command"));
        m_pStaticTitle->create();

        m_pEditCmd = new UEdit(m_hDlg, IDC_EDIT_CMD, m_hInst);
        m_pEditCmd->setPos(rect.left()+40, rect.top()+50, rect.width()-80, 150);
        m_pEditCmd->create();

        m_pButtonCheck = new UPushButton(m_hDlg, IDC_BUTTON_CHECK, m_hInst);
        m_pButtonCheck->setPos(450, rect.top()+260, 100, 60);
        m_pButtonCheck->create();
        m_pButtonCheck->setWindowText(_T("check"));

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
        case IDC_BUTTON_CHECK:
            return onBnCheck();
        default:
            return FALSE;
        }
    }

    BOOL onBnCheck()
    {
        //huys::UPython upy;

        //if (upy.isReady())
        {
            showMsg(_T("Python is OK"), _T("Info"), m_hDlg);
        }

        return FALSE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        HBRUSH hBrushBK = ::CreateSolidBrush(huys::aliceblue);
        switch (message)
        {
        case WM_CTLCOLORDLG:
            return (BOOL)hBrushBK;
        case WM_CTLCOLORSTATIC:
            //return UDialogBox::onCtrlColor(wParam, lParam);
            return (BOOL)hBrushBK;
        }
        return result;
    }
private:
    huys::ADT::UAutoPtr<UEdit> m_pEditCmd;
    AUI::UTransLabelP m_pStaticTitle;
    huys::ADT::UAutoPtr<UPushButton> m_pButtonCheck;
    huys::ADT::UAutoPtr<UComboBox> m_pCbInterp;

};

UDLGAPP_T(UDialogScript, IDD_SCRIPT);
