#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <cassert>

#include "udialogx.h"
#include "udlgapp.h"
#include "ucustomctrl.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UCTCTRL = 11112
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {
    }

    BOOL onInit()
    {
        m_pCTCtrl = new UCustomCtrl(m_hDlg, ID_UCTCTRL, m_hInst);
        m_pCTCtrl->setStyles(WS_BORDER);
        m_pCTCtrl->setPos(20, 20, 260, 260);
        m_pCTCtrl->create();
        return TRUE;
    }
private:
    huys::ADT::UAutoPtr<UCustomCtrl> m_pCTCtrl;
};

UDLGAPP_T(UDialogExt, IDD_TEST);
