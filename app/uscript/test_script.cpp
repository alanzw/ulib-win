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
#include "uconsole.h"
#include "adt/uautoptr.h"
#include "aui/aui_label.h"
#include "adt/ustring.h"
#include "upython.h"
#include "ulua.h"

typedef huys::ADT::UStringAnsi TString;

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
        IDC_COMBO_INTERP = 10003,
        IDC_EDIT_OUTPUT  = 10004
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
        m_pCbInterp->setPos(40, 10, 100, 65);
        m_pCbInterp->create();

        for(int i = 0; g_InterpList[i] != NULL; ++i)
        {
            m_pCbInterp->addText(g_InterpList[i]);
        }

        m_pStaticTitle = new AUI::UTransLabel(m_hDlg, IDC_STATIC, m_hInst);
        m_pStaticTitle->setPos(40, 50, 100, 20);
        m_pStaticTitle->setText(_T("Command"));
        m_pStaticTitle->create();

        m_pEditCmd = new UEdit(m_hDlg, IDC_EDIT_CMD, m_hInst);
        m_pEditCmd->setPos(rect.left()+40, rect.top()+ 90, rect.width()-80, 150);
        m_pEditCmd->create();

        m_pButtonCheck = new UPushButton(m_hDlg, IDC_BUTTON_CHECK, m_hInst);
        m_pButtonCheck->setPos(450, rect.top()+260, 100, 60);
        m_pButtonCheck->create();
        m_pButtonCheck->setWindowText(_T("check"));
        
        m_pEditOutput = new UEdit(m_hDlg, IDC_EDIT_OUTPUT, m_hInst);
        m_pEditOutput->setPos(rect.left()+40, rect.top()+ 340, rect.width()-80, 150);
        m_pEditOutput->create(); 

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
        int nSel = m_pCbInterp->getCurSel();
        if (-1 == nSel)
        {
            showMsg(_T("Select your interpreter!!"), _T("Error"), m_hDlg);
            return FALSE;
        }
        
        switch (nSel)
        {
        case 0:
        {
            AllocConsole();
            huys::UPython upy;

            if (upy.isReady())
            {
                
                //HANDLE hTemp = ::GetStdHandle(STD_OUTPUT_HANDLE);
                //showMsg(_T("Python is OK"), _T("Info"), m_hDlg);
                upy.doString("print 'HelloWorld!'");
                UConsole::PrintStdout("Press any key.\n");
 /*               
                char szBuffer[256];
                TString text;
                DWORD m_nReadNum;
                while (::ReadFile(hTemp, szBuffer, 100, &m_nReadNum, NULL))
                {
                    szBuffer[m_nReadNum] = '\0';
                    text << szBuffer;
                }
                
                m_pEditOutput->setWindowText(text);
*/                
                //FreeConsole();
                
            }
            break;
        }
        case 4:
        {
            
            break;
        }
        default:
        {
            showMsg(_T("Not implemented yet!!!"), _T("Info"), m_hDlg);
        }
        }
        return FALSE;
    }
    
    BOOL onCtrlColorDlg(HDC hdc)
    {
        static HBRUSH hBrushBK = ::CreateSolidBrush(huys::aliceblue);
        return (BOOL)hBrushBK;
    }
private:
    huys::ADT::UAutoPtr<UEdit> m_pEditCmd;
    AUI::UTransLabelP m_pStaticTitle;
    huys::ADT::UAutoPtr<UPushButton> m_pButtonCheck;
    huys::ADT::UAutoPtr<UComboBox> m_pCbInterp;
    huys::ADT::UAutoPtr<UEdit> m_pEditOutput;

};

UDLGAPP_T(UDialogScript, IDD_SCRIPT);
