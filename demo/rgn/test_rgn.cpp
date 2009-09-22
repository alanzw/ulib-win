#include "resource.h"

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT  0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uregion.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    ~UDialogExt()
    {
    }

    virtual BOOL onInit()
    {
        RECT rc;
        ::GetWindowRect(m_hDlg, &rc);
        rc.right -= rc.left;
        rc.bottom -= rc.top;
        rc.left = 0;
        rc.top = 0;
        m_urgn.createRoundRect(rc.left,rc.top,rc.right,rc.bottom,100,100);

        URegion ur2;
        ur2.createRoundRect(50, 50, 120, 120, 20, 20);

        m_urgn.combine(ur2, RGN_DIFF);

        this->setWindowRgn(m_urgn);
        return TRUE;
    }

    void onDraw(HDC hdc)
    {
        RECT rt;
        GetClientRect(m_hDlg, &rt);
        ::DrawText(hdc, "Hello World!", strlen("Hello World!"), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::PostMessage(m_hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return FALSE;
    }
private:
    URegion m_urgn;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

