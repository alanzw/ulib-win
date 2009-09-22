#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "umsg.h"

#include "usplashwnd.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_splash(hInst)
    {}

    ~UDialogExt()
    {
    }

    virtual BOOL onInit()
    {
        //m_splash.setBmp(_T("bk.bmp"));

        m_splash.setBmp(IDB_BACKGROUND, m_hInst);
        m_splash.showSplash(3000);

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
            {
                //m_splash.setExStyles(WS_EX_TOPMOST);
                m_splash.setParent(m_hDlg);
                m_splash.showByTimer(3000);
                return FALSE;
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:

    USplashWindow m_splash;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

