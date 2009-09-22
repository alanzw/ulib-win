#include "resource.h"

#define _WIN32_IE    0x0500

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <commctrl.h>

#include "udialogx.h"
#include "uipaddress.h"
#include "udlgapp.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_IPADDRESS = 12001
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pIPAddr(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pIPAddr);
    }

    virtual BOOL onInit()
    {
        m_pIPAddr = new UIPAddress(m_hDlg, IDC_IPADDRESS, m_hInst);
        m_pIPAddr->setStyles(WS_CHILD|WS_TABSTOP);
        m_pIPAddr->create();

        RECT rcClient;
        ::GetClientRect(m_hDlg, &rcClient);

        rcClient.top += 20;
        rcClient.bottom -= 350;
        rcClient.left += 10;
        rcClient.right -= 10;

        m_pIPAddr->setPosition(&rcClient);

        m_pIPAddr->setAddress(127, 0, 0, 0);
        
        m_pIPAddr->setFocus(1);

		m_pIPAddr->clear();

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
            m_pIPAddr->setAddress(192,168,0,1);
            return FALSE;
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UIPAddress *m_pIPAddr;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

