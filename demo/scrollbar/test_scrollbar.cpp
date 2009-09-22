#include "resource.h"

#define _WIN32_IE 0x0300

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "uscrollbar.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {

        ID_SCROLLBAR = 13333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pScrollBar(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pScrollBar);
    }

    virtual BOOL onInit()
    {
            m_pScrollBar = new UScrollBar(m_hDlg, ID_SCROLLBAR, m_hInst);
            m_pScrollBar->setStyles(SBS_HORZ);
            m_pScrollBar->setPos(100, 100, 300, 50);
            m_pScrollBar->create();
        return TRUE;
    }

private:
    UScrollBar *m_pScrollBar;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

