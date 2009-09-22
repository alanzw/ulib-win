//#define _WIN32_WINNT 0x0501
//#define _WIN32_IE    0x0500

#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

#include <cassert>

#include "udialogx.h"
#include "ucomboboxex.h"
#include "ucombobox.h"
#include "udlgapp.h"
#include "uimagelist.h"

#define HU_VERIFY assert

char buf[256] = "cccc";

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UCOMBOBOX = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pCombo(0),
      m_uiml(1, 32, 32)
    {
        m_uiml.addIcon(IDI_APP, m_hInst);
    }

    ~UDialogExt()
    {
        CHECK_PTR(m_pCombo);
    }

    BOOL onInit()
    {
        m_pCombo = new UComboBoxEx(m_hDlg, ID_UCOMBOBOX, m_hInst);
        m_pCombo->setStyles(CBS_DROPDOWN);
        m_pCombo->setPos(20, 20, 240, 240);
        m_pCombo->create();

        HU_VERIFY(m_pCombo->addItem(0, buf, 256));
        buf[0] = 'x';
        HU_VERIFY(m_pCombo->addItem(1, buf, 256));

        m_pCombo->setCurSel(0);
        m_pCombo->setImageList(m_uiml);
        // Set size of control to make sure it's displayed correctly now
        // that the image list is set.
        ::SetWindowPos(m_pCombo->getCombo(),NULL,0,0,240,240,SWP_NOACTIVATE);
        return TRUE;
    }
private:
    UComboBoxEx *m_pCombo;
    UImageList m_uiml;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


