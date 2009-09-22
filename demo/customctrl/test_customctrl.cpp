#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <cassert>

#include "udialogx.h"
#include "udlgapp.h"
#include "ucustomctrl.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UCTCTRL = 11112
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pCTCtrl(0)
    {
    }

    ~UDialogExt()
    {
        CHECK_PTR(m_pCTCtrl);
    }

    BOOL onInit()
    {
        m_pCTCtrl = new UCustomCtrl(m_hDlg, ID_UCTCTRL, m_hInst);
        m_pCTCtrl->setPos(20, 20, 260, 260);
        m_pCTCtrl->create();
        return TRUE;
    }
private:
    UCustomCtrl *m_pCTCtrl;
};

UDLGAPP_T(UDialogExt, IDD_TEST);
