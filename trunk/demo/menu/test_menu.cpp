#include "resource.h"

#define  WINVER 0x0501
#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <assert.h>

#include "udialogx.h"
#include "umenu.h"
#include "udlgapp.h"
#include "umsg.h"
#include "ubitmap.h"

#include "adt/ustring.h"

#include "aui/aui_label.h"
#include "aui/aui_menubar.h"

#include "mymenu.h"

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


        _label = new AUI::UTransLabel(m_hDlg, -1, m_hInst);
        _label->setPos(50, 50, 250, 100);
        _label->setStyles(SS_LEFT|SS_NOTIFY|SS_SUNKEN);
        _label->create();

        MENUBARINFO mbi = {0};
        mbi.cbSize = sizeof(MENUBARINFO);
        ::GetMenuBarInfo(m_hDlg, OBJID_MENU, 0, &mbi);

        huys::ADT::UStringAnsi text(256);
        char buffer[256];

        int idx = 0;
        while (0 != ::GetMenuString(mbi.hMenu, idx, buffer, 256, MF_BYPOSITION))
        {
            text << buffer;

            text << "\r\n";
            
            ++idx;
        }

        _label->setWindowText(text);


        _menubar = new AUI::UMenuBar(m_hDlg);
        _menubar->setPos(50, 200, 350, 35);
        _menubar->setMenuName(m_hInst, MAKEINTRESOURCE(IDR_MAINMENU));
        _menubar->create();

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

        if (STN_CLICKED == HIWORD(wParam) && _label->getHWND() == (HWND)lParam)
        {
            MENUBARINFO mbi = {0};
            mbi.cbSize = sizeof(MENUBARINFO);
            ::GetMenuBarInfo(m_hDlg, OBJID_MENU, 0, &mbi);

            HMENU hMenu = ::GetSubMenu(mbi.hMenu, 0);

            POINT pt;
            ::GetCursorPos(&pt);
            ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
                0, m_hDlg, 0);

        }

        return UDialogBox::onCommand(wParam, lParam);
    }
private:
    UMenu umn;
    UMenu upop;

    UBitmap ubmp[3];

    AUI::UTransLabelP _label;
    AUI::UMenuBarP _menubar;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

