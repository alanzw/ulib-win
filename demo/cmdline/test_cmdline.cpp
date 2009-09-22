#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "uedit.h"
#include "udlgapp.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_ED_CMDLINE = 11111
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pEdit(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pEdit);
    }

    BOOL onInit()
    {
        m_pEdit = new UEdit(m_hDlg, IDC_ED_CMDLINE, m_hInst);
        m_pEdit->setPos(20, 20, 240, 80);
        m_pEdit->create();
        return TRUE;
    }
private:
    UEdit *m_pEdit;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


