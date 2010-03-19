#include "resource.h"

#define  WINVER 0x0501

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <assert.h>

#include "udialogx.h"
#include "umenu.h"
#include "udlgapp.h"
#include "umsg.h"
#include "ubitmap.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDM_HELP = 3333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        umn.createPopup();

        umn.append(IDM_HELP, _T("&help"));

        MENUINFO mi = {0};
        mi.cbSize = sizeof(MENUINFO);
        mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS | MIIM_BITMAP;
        mi.dwStyle = MNS_AUTODISMISS;
        mi.hbrBack = (HBRUSH)::GetStockObject(GRAY_BRUSH);

        HMENU hMenu = ::GetMenu(m_hDlg);

        ::SetMenuInfo(hMenu, &mi);

        ubmp[0].loadFromResource(IDB_ICON1, m_hInst, 32, 32);
        ubmp[1].loadFromResource(IDB_ICON2, m_hInst, 32, 32);
        ubmp[2].loadFromResource(IDB_ICON3, m_hInst, 32, 32);

        ::SetMenuItemBitmaps(hMenu, IDM_EXIT, MF_BYCOMMAND, ubmp[0], ubmp[1]);

        umn.addSeparator();

        umn.append(IDM_ICON1, ubmp[0]);
        umn.append(IDM_ICON2, ubmp[1]);
        umn.append(IDM_ICON3, ubmp[2]);

        umn.addSeparator();

        upop.createPopup();
        upop.append(3335, _T("&quit"));

        umn.addSubMenu(upop, _T("&popup"));

        umn.checkItemByPos(0);

        return TRUE;
    }

    virtual BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        POINT pt;
        ::GetCursorPos(&pt);
        umn.attach(m_hDlg);
        //umn.trackPopup(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
        umn.trackPopup(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y);
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_HELP:
            showMsg("&Help!");
            break;
        case 3335:
        case IDM_EXIT:
            return this->onCancel();
        case IDM_COPY:
            showMsg("Not implement yet");
            break;
        case IDM_ABOUT:
            showMsg("About");
            break;
        case IDM_ICON1:
            showMsg("Icon1");
            break;
        case IDM_ICON2:
            showMsg("Icon2");
            break;
        case IDM_ICON3:
            showMsg("Icon3");
            break;
        default:
            ;
        }
        return UDialogBox::onCommand(wParam, lParam);
    }
private:
    UMenu umn;
    UMenu upop;

    UBitmap ubmp[3];
};

UDLGAPP_T(UDialogExt, IDD_TEST);

