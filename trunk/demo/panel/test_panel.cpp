#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "upanel.h"
#include "ubutton.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_Panel_GO = 1021,
        IDC_BN_FIRST  = 1022,
        IDC_BN_SECOND  = 1023
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pPanelGo(0),
          m_pBtnFirst(0),
          m_pBtnSecond(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pPanelGo);
        CHECK_PTR(m_pBtnFirst);
        CHECK_PTR(m_pBtnSecond);
    }

    virtual BOOL onInit()
    {
        m_pPanelGo = new UPanel(m_hDlg, IDC_Panel_GO, m_hInst);
        m_pPanelGo->create();

        RECT rc = { 20, 20, 380, 250};
        m_pPanelGo->setPosition(&rc);

        m_pBtnFirst = new UButton(m_hDlg, IDC_BN_FIRST, m_hInst);
        m_pBtnFirst->create();
        m_pBtnFirst->setWindowText(_T("first"));

        m_pBtnSecond = new UButton(m_hDlg, IDC_BN_SECOND, m_hInst);
        m_pBtnSecond->create();
        m_pBtnSecond->setWindowText(_T("second"));

        m_pPanelGo->addControl(m_pBtnFirst);
        m_pPanelGo->addControl(m_pBtnSecond);

        m_pPanelGo->redirectMsg(m_hDlg);

        return true;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_FIRST:
            return onBnFirst();
        case IDC_BN_SECOND:
            return onBnSecond();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

protected:
private:
    UPanel *m_pPanelGo;

    UButton *m_pBtnFirst;
    UButton *m_pBtnSecond;
private:
    BOOL onBnFirst()
    {
        showMsg(_T("first chance"), _T("command"), m_hDlg);
        return FALSE;
    }

    BOOL onBnSecond()
    {
        showMsg(_T("second chance"),_T("command"), m_hDlg);
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);

