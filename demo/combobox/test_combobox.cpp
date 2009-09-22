#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "ucombobox.h"
#include "udlgapp.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{   
    enum {
        ID_UCOMBOBOX = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pCombo(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pCombo);
    }

    BOOL onInit()
    {
        m_pCombo = new UComboBox(m_hDlg, ID_UCOMBOBOX, m_hInst);
        RECT rc = {20, 20, 160, 160};
        m_pCombo->create();
        m_pCombo->setPosition(&rc);
        m_pCombo->addText("hello");
        m_pCombo->addText("hello2");
        m_pCombo->setCurSel(0);
        return TRUE;
    }

private:
    UComboBox *m_pCombo;
};


UDLGAPP_T(UDialogExt, IDD_TEST);


