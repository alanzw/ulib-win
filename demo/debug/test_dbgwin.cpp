#include "resource.h"

#define _WIN32_IE 0x0400
#define WINVER    0x0500

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"

#include "ubasewindow.h"

#include "ulistbox.h"
#include "utoolbar.h"

#include "adt/uautoptr.h"

class UTraceWindow : public UBaseWindow
{
    enum {
        ID_LB_TRACE = 1111
    };

    enum {
        IDM_SAVE = 2000
    };
public:
    UTraceWindow(HWND hParent)
    : UBaseWindow(hParent)
    {
        RECT rc;
        ::GetWindowRect(getParent(), &rc);
        rc.left = rc.right - 200;
        rc.top = rc.bottom - 200;
        setRect(&rc);
        setMenu(0);
        setWndClassName(_T("HUYS_TRACE_WINDOW_CLASS"));
        setTitle(_T("Trace"));
        setStyles(WS_VISIBLE);
        setExStyles(WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
    }

    BOOL onCreate()
    {
        _pListBox = new UListBox(this, ID_LB_TRACE);
        RECT rc;
        this->getClientRect(&rc);
        _pListBox->setRect(&rc);
        _pListBox->setStyles(WS_VSCROLL);
        _pListBox->create();
        _pListBox->setItemHeight(25);

        return UBaseWindow::onCreate();
    }

    void addLine(LPCTSTR sLine)
    {
        _pListBox->addString(sLine);
    }

    BOOL onClose()
    {
        this->hide();
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UListBox> _pListBox;
};

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDM_OPEN = 2000,
        IDM_SAVE = 2001,
        IDM_PIN  = 2002,
        IDM_HELP = 2003
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        win = new UTraceWindow(m_hDlg);
        win->create();

        addToolbar();


        this->setDlgIconBig(::LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_EYE)));

        return TRUE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        win->addLine("Here it is!");
        return FALSE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_OPEN:
            return onMenuOpen();
        case IDM_SAVE:
            return onMenuSave();
        case IDM_PIN:
            return onMenuPin();
        case IDM_HELP:
            return onMenuHelp();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
protected:
private:
    void addToolbar()
    {
        toolbar = new UToolBar(m_hDlg, 0, m_hInst);
        toolbar->create();
        toolbar->addBitmap(1, IDB_OPEN);
        toolbar->addBitmap(1, IDB_SAVE);
        toolbar->addBitmap(1, IDB_PIN);
        toolbar->addBitmap(1, IDB_HELP);
        toolbar->addButton(0, IDM_OPEN, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->addButton(1, IDM_SAVE, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->addSeparator(4);
        toolbar->addButton(2, IDM_PIN, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->addSeparator(4);
        toolbar->addButton(3, IDM_HELP, TBSTATE_ENABLED, TBSTYLE_AUTOSIZE);
        toolbar->autosize();
    }

    BOOL onMenuOpen()
    {
        win->addLine(_T("On Menu Open!"));
        return FALSE;
    }

    BOOL onMenuSave()
    {
        win->addLine(_T("On Menu Save!"));
        return FALSE;
    }

    BOOL onMenuPin()
    {
        win->addLine(_T("On Menu Pin!"));
        BYTE state = toolbar->getState(IDM_PIN);
        if (state & TBSTATE_CHECKED)
        {
            this->setNoTopMost();
            toolbar->setState(IDM_PIN, state& ~TBSTATE_CHECKED);
            win->addLine(_T("Disable Topmost!"));
        }
        else
        {
            this->setTopMost();
            toolbar->setState(IDM_PIN, state|TBSTATE_CHECKED);
            win->addLine(_T("Enable Topmost!"));
        }

        return FALSE;
    }

    BOOL onMenuHelp()
    {
        win->addLine(_T("On Menu Help!"));
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UTraceWindow> win;
    huys::ADT::UAutoPtr<UToolBar> toolbar;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

