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

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        switch (message)
        {
        case WM_ERASEBKGND:
            {
                // UBitmap ubm(_T("bk.bmp"));
                // UBitmap ubm(IDB_BACKGROUND, m_hInst);
                HDC hdc = (HDC)wParam;
                RECT rc;
                ::GetClientRect(m_hDlg, &rc);
                //hdc = ::GetWindowDC(m_hDlg);
                m_pBack->showStretch(hdc, rc);
                //ReleaseDC(m_hDlg, hdc);
                return TRUE;
            }
        }
        return result;
    }
private:
    UBitmap *m_pBack;
};

UDLGAPP_T(UDialogViewer, IDD_VIEWER);

