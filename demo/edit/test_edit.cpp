#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "uedit.h"
#include "udlgapp.h"
#include "colors.h"
#include "ubutton.h"

#include "ueditex.h"

#include "adt/uautoptr.h"

#if !defined(_MSC_VER) || _MSC_VER <= 1200
// ToolTip Icons possible wParam values for TTM_SETTITLE message
#define TTI_NONE                0
#define TTI_INFO                1
#define TTI_WARNING             2
#define TTI_ERROR               3
#endif

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_EDIT_UE = 3333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        m_pEditEx = new UEditEx(m_hDlg, IDC_EDIT_UE, m_hInst);
        m_pEditEx->setPos(50,95,400,180);
        m_pEditEx->setStyles( ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN |
                              ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL);
        m_pEditEx->create();
        m_pEditEx->setWindowText(_T("ÖÐ¹ú"));

        URadioButton ura;
        ura.fromID(m_hDlg, IDC_RADIO_NORMAL);
        ura.check();

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_RADIO_TOPMOST:
            return this->setTopMost();
        case IDC_RADIO_NORMAL:
            return this->setNoTopMost();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        m_pEditEx->showBalloonTip(L"hello", L"boooooo!", TTI_INFO);


        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UEditEx> m_pEditEx;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

