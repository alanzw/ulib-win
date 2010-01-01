#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "ulib.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uprogressbar.h"
#include "ubutton.h"

#include "colors.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_BN_OK = 1123
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pUBtnOK);
    }
    
    virtual BOOL onInit()
    {
        m_pUBtnOK = new UPushButton(m_hDlg, ID_BN_OK, m_hInst);
        m_pUBtnOK->setPos(100, 100, 100, 100);
        m_pUBtnOK->create();
        return TRUE;
    }
    
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
            case ID_BN_OK:
                {
                    return TRUE;
                }
            default:
                return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UPushButton *m_pUBtnOK;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

