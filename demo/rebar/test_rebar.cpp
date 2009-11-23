#include "resource.h"

#define _WIN32_IE 0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "ustatic.h"
#include "udlgapp.h"
#include "urebar.h"
#include "utoolbar.h"
#include "uimagelist.h"
#include "uedit.h"
#include "ucombobox.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_REBAR   = 1222,
        IDC_TOOLBAR = 1223,
        IDC_EDIT    = 1224,
        IDC_COMBO   = 1225
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pRebar(0),
        m_pToolBar(0),
        m_pImageList(0),
        m_pCombo(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pRebar);
        CHECK_PTR(m_pToolBar);
        CHECK_PTR(m_pImageList);
        CHECK_PTR(m_pCombo);
    }

    virtual BOOL onInit()
    {
        m_pRebar = new URebar(m_hDlg, IDC_REBAR, m_hInst);
        m_pRebar->setStyles(WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
            RBS_VARHEIGHT | CCS_NODIVIDER | RBS_BANDBORDERS);
        m_pRebar->create();

        m_pRebar->initBandInfo();

        // The CCS_NORESIZE style may have to be added to the window style
        // when creating a toolbar that is to be a child of a rebar control
        m_pToolBar = new UToolBar(m_hDlg, IDC_TOOLBAR, m_hInst);

        m_pToolBar->setStyles(CCS_NORESIZE);
        m_pToolBar->create();

        m_pImageList = new UImageList(IDR_TOOLBAR1, m_hInst);
        m_pToolBar->setImageList(m_pImageList->getHandle());

        TBBUTTON tbButtons[3] =
        {
            { MAKELONG(0, 0), IDM_NEW, TBSTATE_ENABLED,
            0, {0}, 0, (INT_PTR)"New" },
            { MAKELONG(1, 0), IDM_OPEN, TBSTATE_ENABLED,
            0, {0}, 0, (INT_PTR)"Open"},
            { MAKELONG(2, 0), IDM_SAVE, 0,
            0, {0}, 0, (INT_PTR)"Save"}
        };
        m_pToolBar->addButtons(3, tbButtons);
        //m_pToolBar->autosize();
        m_pToolBar->show();

        DWORD dwBtnSize = m_pToolBar->getButtonSize();
        m_pRebar->addChild(m_pToolBar->getHWND(), _T(""), HIWORD(dwBtnSize)*3, (int)(LOWORD(dwBtnSize)*1.4), 0);

        m_pEdit = new UEdit(m_pRebar->getHWND(), IDC_EDIT, m_hInst);
        m_pEdit->create();

        RECT rc = {0, 0, 100, 25};
        m_pEdit->setPosition(&rc);
        m_pRebar->addChild(m_pEdit->getHWND(), _T("Name"), 0, rc.bottom-rc.top, 150);

        //rc.bottom += 20;
        m_pCombo = new UComboBox(m_pRebar->getHWND(), IDC_COMBO, m_hInst);
        m_pCombo->setStyles(CBS_AUTOHSCROLL| WS_CLIPCHILDREN | WS_CLIPSIBLINGS |WS_VSCROLL);
        m_pCombo->create();
        m_pCombo->setPosition(&rc);
        //rc.bottom -= 20;
        m_pRebar->addChild(m_pCombo->getHWND(), _T("Font"), 0, rc.bottom-rc.top, 260);
        m_pCombo->addText(_T("hello!"));
        m_pCombo->addText(_T("good!"));
        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch ( LOWORD (wParam) )
        {
        case IDC_BN_GO:
            return onBnGo();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    URebar *m_pRebar;
    UToolBar *m_pToolBar;
    UImageList *m_pImageList;
    UEdit *m_pEdit;
    UComboBox *m_pCombo;

private:
    BOOL onBnGo()
    {
        //m_pRebar->setBkColor(huys::blue);
        showMsg(_T("go"), _T("info"), m_hDlg);
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


