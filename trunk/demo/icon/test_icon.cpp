#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uicon.h"
#include "ubutton.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_BN_GO = 1021
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pBnGo(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pBnGo);
    }

    virtual BOOL onInit()
    {
        m_pBnGo = new UButton(m_hDlg, IDC_BN_GO, m_hInst);
        m_pBnGo->create();
        m_pBnGo->setWindowText(_T("GO"));

        RECT rc = { 200, 100, 380, 150};
        m_pBnGo->setPosition(&rc);

        //
        m_ico.loadIconEx(m_hInst, IDI_APP);
        this->setDlgIconBig(m_ico.getHICON());
        
        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BN_GO:
            return onBnGo();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    BOOL onBnGo()
    {
        m_ico.draw(m_hDlg, 0, 0);
        return FALSE;
    }

protected:
private:
    UButton *m_pBnGo;
    UIcon m_ico;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

