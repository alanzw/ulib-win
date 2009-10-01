#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubitmap.h"

using huys::UDialogBox;

class UDialogViewer : public UDialogBox
{
public:
    UDialogViewer(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    ~UDialogViewer()
    {
        CHECK_PTR(m_pBack);
    }

    BOOL onInit()
    {
        m_pBack = new UBitmap(IDB_BACKGROUND, m_hInst);
        return TRUE;
    }

    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        m_pBack->showStretch(hdc, rc);
        return TRUE;
    }
private:
    UBitmap *m_pBack;
};

UDLGAPP_T(UDialogViewer, IDD_VIEWER);

