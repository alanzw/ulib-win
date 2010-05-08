#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "umsg.h"
#include "ubutton.h"
#include "usplashwnd.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_BN_SHOWSPLASH = 111
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_splash(hInst)
    {}

    virtual BOOL onInit()
    {
        //m_splash.setBmp(_T("bk.bmp"));

        m_splash.setBmp(IDB_BACKGROUND, m_hInst);
        m_splash.showSplash(3000);

        _btn = new UButton(m_hDlg, ID_BN_SHOWSPLASH, m_hInst);
        _btn->setPos(100, 100, 100, 50);
        _btn->create();
        _btn->setWindowText(_T("show splash"));

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
        case ID_BN_SHOWSPLASH:
            {
                m_splash.setParent(m_hDlg);
                m_splash.showTransparent(huys::gray);
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:

    USplashWindow m_splash;

    huys::ADT::UAutoPtr<UButton> _btn;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

