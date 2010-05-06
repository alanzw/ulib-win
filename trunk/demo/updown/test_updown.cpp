#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "uupdownctrl.h"
#include "udlgapp.h"
#include "uedit.h"
#include "umsg.h"
#include "adt/uautoptr.h"

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
        : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_puud = new UUpDownCtrl(m_hDlg, ID_UUPDOWNCTRL, m_hInst);
        m_puud->setStyles(UDS_ALIGNRIGHT|UDS_SETBUDDYINT);
        m_puud->create();
        m_puud->setRange(100, 1);

        m_pEdit = new UEdit(m_hDlg, ID_ET_BUDDY, m_hInst);
        m_pEdit->setPos(50, 50, 200, 40);
        m_pEdit->create();

        m_puud->setBuddy(*m_pEdit);

        return TRUE;
    }

    /* virtual */ BOOL onNotify(WPARAM wParam, LPARAM lParam)
    {
        if (UDN_DELTAPOS == (((LPNMHDR)lParam)->code))
        {
            return onDeltaPos();
        }

        return UDialogBox::onNotify(wParam, lParam);
    }

private:
    huys::ADT::UAutoPtr<UUpDownCtrl> m_puud;
    huys::ADT::UAutoPtr<UEdit> m_pEdit;
private:
    BOOL onDeltaPos()
    {
        return FALSE;
    }
};


UDLGAPP_T(MyDialog, IDD_TEST);
