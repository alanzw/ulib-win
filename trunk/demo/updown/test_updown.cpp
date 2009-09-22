#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "uupdownctrl.h"
#include "udlgapp.h"
#include "uedit.h"

using huys::UDialogBox;

//
class MyDialog : public UDialogBox
{
    enum {
        ID_UUPDOWNCTRL = 11,
        ID_ET_BUDDY    = 222
    };
public:
    MyDialog(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_puud(0),
          m_pEdit(0)
    {}

    ~MyDialog()
    {
        CHECK_PTR(m_puud);
        CHECK_PTR(m_pEdit);
    }

    BOOL onInit()
    {
        m_puud = new UUpDownCtrl(m_hDlg, ID_UUPDOWNCTRL, m_hInst);
        //RECT rc = {200, 200, 230, 250};
        m_puud->setStyles(UDS_ALIGNRIGHT|UDS_SETBUDDYINT);
        m_puud->create();
        //m_puud->setPosition(&rc);
        m_puud->setRange(100, 1);
        
        m_pEdit = new UEdit(m_hDlg, ID_ET_BUDDY, m_hInst);
        m_pEdit->create();
        RECT rcEdit = {50, 50, 250, 100};
        m_pEdit->setPosition(&rcEdit);
        
        m_puud->setBuddy(*m_pEdit);
        
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
                return onDeltaPos();
            default:
                break;
            }
        }


        return result;
    }
private:
    UUpDownCtrl *m_puud;
    UEdit *m_pEdit;
private:
    BOOL onDeltaPos()
    {
        
        return FALSE;
    }
};


UDLGAPP_T(MyDialog, IDD_TEST);
