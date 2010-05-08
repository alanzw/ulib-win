#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <commctrl.h>

#include "udialogx.h"
#include "urichedit.h"
#include "udlgapp.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_RICHEDIT  = 10001
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        m_pRichEdit = new URichEdit(m_hDlg, IDC_RICHEDIT, m_hInst);

        m_pRichEdit->setStyles(ES_MULTILINE|ES_WANTRETURN|WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP);

        m_pRichEdit->create();

        RECT rcClient;
        ::GetClientRect(m_hDlg, &rcClient);

        rcClient.top += 20;
        rcClient.bottom -= 50;
        rcClient.left += 10;
        rcClient.right -= 10;

        m_pRichEdit->setPosition(&rcClient);

        return TRUE;
    }
private:
    huys::ADT::UAutoPtr<URichEdit> m_pRichEdit;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

