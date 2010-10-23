#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "uedit.h"
#include "udlgapp.h"
#include "ucmdline.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_ED_CMDLINE = 11111
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        UCmdLine ucmdline;
        
        int nArg  = 0;
        LPTSTR *sArgv;
        
        huys::URectL rect;
        ::GetClientRect(m_hDlg, rect);
        
        m_pEdit = new UEdit(m_hDlg, IDC_ED_CMDLINE, m_hInst);
        m_pEdit->setRect(rect);
        m_pEdit->setStyles( ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN |
                              ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL);
        m_pEdit->create();
        
        sArgv= ucmdline.CommandLineToArgv(::GetCommandLine(), &nArg);

        for (int i=0; i<nArg; ++i)
        {
            m_pEdit->addLine(sArgv[i]);
        }

        return TRUE;
    }
private:
    huys::ADT::UAutoPtr<UEdit> m_pEdit;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


