#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "uedit.h"
#include "udlgapp.h"
#include "ucmdline.h"

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
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        m_pEdit = new UEdit(m_hDlg, IDC_ED_CMDLINE, m_hInst);
        //m_pEdit->setPos(20, 20, 240, 80);
        m_pEdit->setStyles( ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN |
                              ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL);
        m_pEdit->create();
        m_pEdit->setPosition(&rc);
        UCmdLine ucmdline;
        int nArgs  = 0;
        LPTSTR *sArgv = ucmdline.CommandLineToArgv(::GetCommandLine(), &nArgs);

        for (int i=0; i<nArgs; ++i)
        {
            m_pEdit->addLine(sArgv[i]);
        }

        return TRUE;
    }
private:
    UEdit *m_pEdit;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


