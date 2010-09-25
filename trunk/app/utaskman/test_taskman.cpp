#define  _WIN32_IE 0x0500

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>

#include "umsg.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "ucommondialog.h"
#include "utabctrl.h"
#include "ustatusbar.h"

#include "umenu.h"
#include "uprocessman.h"

#include "adt/uautoptr.h"

#include "usys.h"

using huys::UDialogBox;

#include "utabchild.h"

LPCTSTR sTabTitle[] = {
    "Applications",
    "Processes",
    "Services",
    "Performance",
    "Networking",
    "Users"
};

class UDialogExt : public UDialogBox
{
    enum {
        IDX_APP = 0,
        IDX_PRC = 1,
        IDX_SVC = 2,
        IDX_PRF = 3,
        IDX_NET = 4,
        IDX_USR = 5
    };
    enum {
        IDC_STATUSBAR = 10001
    };
    enum {
        ID_TIMER_UPDATE = 101
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {
        ::ZeroMemory(&m_pTabChild, sizeof(m_pTabChild));
    }

    virtual BOOL onInit()
    {
        setDlgIconSmall(IDI_APP);

        if (!m_UMainMenu.getMenu(m_hDlg))
        {
            return FALSE;
        }

        HWND hTabTask;
        hTabTask = GetDlgItem(m_hDlg, IDC_TAB_TASK);
        TCITEM item;
        item.mask = TCIF_TEXT;
        item.pszText = (LPSTR)sTabTitle[IDX_APP];
        SendMessage(hTabTask, TCM_INSERTITEM, 0, (LPARAM)&item);

        item.pszText = (LPSTR)sTabTitle[IDX_PRC];
        SendMessage(hTabTask, TCM_INSERTITEM, 1, (LPARAM)&item);

        item.pszText = (LPSTR)sTabTitle[IDX_SVC];
        SendMessage(hTabTask, TCM_INSERTITEM, 2, (LPARAM)&item);

        item.pszText = (LPSTR)sTabTitle[IDX_PRF];
        SendMessage(hTabTask, TCM_INSERTITEM, 3, (LPARAM)&item);

        item.pszText = (LPSTR)sTabTitle[IDX_NET];
        SendMessage(hTabTask, TCM_INSERTITEM, 4, (LPARAM)&item);

        item.pszText = (LPSTR)sTabTitle[IDX_USR];
        SendMessage(hTabTask, TCM_INSERTITEM, 5, (LPARAM)&item);

        m_pTabChild[IDX_APP]= new UTabChildApp(m_hInst, IDD_SHEET_APP, hTabTask);
        m_pTabChild[IDX_APP]->create();
        m_pTabChild[IDX_APP]->show();

        m_pTabChild[IDX_PRC]= new UTabChildPrc(m_hInst, IDD_SHEET_PRC, hTabTask);
        m_pTabChild[IDX_PRC]->create();
        m_pTabChild[IDX_PRC]->hide();

        m_pTabChild[IDX_SVC]= new UTabChildSvc(m_hInst, IDD_SHEET_SVC, hTabTask);
        m_pTabChild[IDX_SVC]->create();
        m_pTabChild[IDX_SVC]->hide();

        m_pTabChild[IDX_PRF]= new UTabChildPrf(m_hInst, IDD_SHEET_PRF, hTabTask);
        m_pTabChild[IDX_PRF]->create();
        m_pTabChild[IDX_PRF]->hide();

        m_pTabChild[IDX_NET]= new UTabChildNetwork(m_hInst, IDD_SHEET_NET, hTabTask);
        m_pTabChild[IDX_NET]->create();
        m_pTabChild[IDX_NET]->hide();

        m_pTabChild[IDX_USR]= new UTabChildUser(m_hInst, IDD_SHEET_USR, hTabTask);
        m_pTabChild[IDX_USR]->create();
        m_pTabChild[IDX_USR]->hide();

        RECT rc;
        ::GetClientRect(hTabTask, &rc);

        //::ClientToScreen(m_hDlg, (LPPOINT)&rc);
        //::ScreenToClient(m_pTabCtrl->getHWND(), (LPPOINT)&rc);

        rc.top += 25;
        rc.left += 5;
        rc.right -= 5;
        rc.bottom -= 5;
        ::MoveWindow(m_pTabChild[IDX_APP]->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
        ::MoveWindow(m_pTabChild[IDX_PRC]->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
        ::MoveWindow(m_pTabChild[IDX_SVC]->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
        ::MoveWindow(m_pTabChild[IDX_PRF]->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
        ::MoveWindow(m_pTabChild[IDX_NET]->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
        ::MoveWindow(m_pTabChild[IDX_USR]->getHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);


        m_pStatusBar = new UStatusBar(m_hDlg, IDC_STATUSBAR, m_hInst);
        m_pStatusBar->setPos(0, 100, 300, 100);
        m_pStatusBar->create();

        m_pStatusBar->setMinHeight(16);

        int aWidths[] = { 100, 250, 450 };
        m_pStatusBar->setParts(3, aWidths);

        TCHAR buf[512];
        DWORD u1;
        DWORD u2;

        m_pStatusBar->setText(0, _T("Processes: "));

        wsprintf(buf, _T("CPU Usage:  %d%%"), USystem::getCPUUsage());
        m_pStatusBar->setText(1, buf);

        u1 = USystem::getPhysicMem();
        u2 = USystem::getPhysicMemAvail();
        wsprintf(buf, _T("Memory Usage: %ld M / %ld M"), u1-u2, u1);
        m_pStatusBar->setText(2, buf);

        setTimer(ID_TIMER_UPDATE, 1000);

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if ( WM_MOVE == message )
        {
            UTabCtrl tab;
            tab.fromID(m_hDlg, IDC_TAB_TASK);
            int nSel = tab.getCurSel();
            return m_pTabChild[nSel]->redraw();
        }

        return UDialogBox::DialogProc(message, wParam, lParam);
    }



    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDM_NEW:
            showCustomInfoMsg("New", m_hDlg);
            return TRUE;
        case IDM_ABOUT:
            showCustomMsgBox("UExplorer v0.0.1 By Huys", "About", m_hDlg);
            return TRUE;
        case IDM_BKCOLOR:
            return TRUE;
        case IDM_ALWAYS_ON_TOP:
            return onMenuTopMost();
        case IDM_EXIT:
            return UDialogBox::onClose();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL onNotify(WPARAM wParam, LPARAM lParam)
    {
        UINT code = ((LPNMHDR)lParam)->code;
        if (TCN_SELCHANGE == code)
        {
            return this->onTabSelChange();
        }
        return UDialogBox::onNotify(wParam, lParam);
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if (ID_TIMER_UPDATE == wParam)
        {
            go_update();

            return FALSE;
        }
        return UDialogBox::onTimer(wParam, lParam);
    }

    BOOL onClose()
    {
        this->killTimer(ID_TIMER_UPDATE);
        return UDialogBox::onClose();
    }
private:
    huys::ADT::UAutoPtr<UTabChild> m_pTabChild[6];
    huys::ADT::UAutoPtr<UStatusBar> m_pStatusBar;
    //
    UMenu m_UMainMenu;
private:
    BOOL onTabSelChange()
    {
        UTabCtrl tab;
        tab.fromID(m_hDlg, IDC_TAB_TASK);
        int nSel = tab.getCurSel();
        switch (nSel)
        {
        case IDX_APP:
            {
            m_pTabChild[IDX_APP]->show();
            m_pTabChild[IDX_PRC]->hide();
            m_pTabChild[IDX_SVC]->hide();
            m_pTabChild[IDX_PRF]->hide();
            m_pTabChild[IDX_NET]->hide();
            m_pTabChild[IDX_USR]->hide();
            m_pTabChild[IDX_APP]->setRedraw();
            return FALSE;
            }
        case IDX_PRC:
            {
            m_pTabChild[IDX_PRC]->show();
            m_pTabChild[IDX_APP]->hide();
            m_pTabChild[IDX_SVC]->hide();
            m_pTabChild[IDX_PRF]->hide();
            m_pTabChild[IDX_NET]->hide();
            m_pTabChild[IDX_USR]->hide();
            m_pTabChild[IDX_PRC]->setRedraw();
            return FALSE;
            }
        case IDX_SVC:
            {
            m_pTabChild[IDX_SVC]->show();
            m_pTabChild[IDX_PRC]->hide();
            m_pTabChild[IDX_APP]->hide();
            m_pTabChild[IDX_PRF]->hide();
            m_pTabChild[IDX_NET]->hide();
            m_pTabChild[IDX_USR]->hide();
            m_pTabChild[IDX_SVC]->setRedraw();
            return FALSE;
            }
        case IDX_PRF:
            {
            m_pTabChild[IDX_PRF]->show();
            m_pTabChild[IDX_PRC]->hide();
            m_pTabChild[IDX_APP]->hide();
            m_pTabChild[IDX_SVC]->hide();
            m_pTabChild[IDX_NET]->hide();
            m_pTabChild[IDX_USR]->hide();
            m_pTabChild[IDX_PRF]->setRedraw();
            return FALSE;
            }
        case IDX_NET:
            {
            m_pTabChild[IDX_NET]->show();
            m_pTabChild[IDX_PRC]->hide();
            m_pTabChild[IDX_APP]->hide();
            m_pTabChild[IDX_SVC]->hide();
            m_pTabChild[IDX_PRF]->hide();
            m_pTabChild[IDX_USR]->hide();
            m_pTabChild[IDX_NET]->setRedraw();
            return FALSE;
            }
        case IDX_USR:
            {
            m_pTabChild[IDX_USR]->show();
            m_pTabChild[IDX_PRC]->hide();
            m_pTabChild[IDX_APP]->hide();
            m_pTabChild[IDX_SVC]->hide();
            m_pTabChild[IDX_NET]->hide();
            m_pTabChild[IDX_PRF]->hide();
            m_pTabChild[IDX_USR]->setRedraw();
            return FALSE;
            }
        default:
            return FALSE;
        }
    }

    BOOL onMenuTopMost()
    {
        //HMENU hMenu = ::GetMenu(m_hDlg);
        //if (MF_CHECKED & ::GetMenuState(hMenu, IDM_ALWAYS_ON_TOP, MF_BYCOMMAND))
        //{
        //    CheckMenuItem(hMenu, IDM_ALWAYS_ON_TOP, MF_BYCOMMAND|MF_UNCHECKED);
        //    this->setNoTopMost();
        //}
        //else
        //{
        //    CheckMenuItem(hMenu, IDM_ALWAYS_ON_TOP, MF_BYCOMMAND|MF_CHECKED);
        //    this->setTopMost();
        //}

        if (m_UMainMenu.isItemChecked(IDM_ALWAYS_ON_TOP))
        {
            m_UMainMenu.uncheckItemByID(IDM_ALWAYS_ON_TOP);
            this->setNoTopMost();
        }
        else
        {
            m_UMainMenu.checkItemByID(IDM_ALWAYS_ON_TOP);
            this->setTopMost();
        }

        return FALSE;
    }

    void go_update()
    {
        TCHAR buf[512];
        DWORD u1;
        DWORD u2;


        m_pStatusBar->setText(0, _T("Processes: "));

        wsprintf(buf, _T("CPU Usage:  %u%%"), USystem::getCPUUsage());
        m_pStatusBar->setText(1, buf);

        u1 = USystem::getPhysicMem();
        u2 = USystem::getPhysicMemAvail();
        wsprintf(buf, _T("Memory Usage: %ld M / %ld M"), u1-u2, u1);
        m_pStatusBar->setText(2, buf);
    }
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);
