#include "resource.h"

#define _WIN32_IE 0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "ubutton.h"
#include "uedit.h"
#include "ulistview.h"
#include "adt/uautoptr.h"

#include "ufilesplit.h"

class UMyWindow : public UBaseWindow
{
    enum {
		ID_BUTTON_ADD   = 11001,
        ID_BUTTON_GO    = 11002,
        ID_TIMER_CLOCK = 11003,
		ID_LISTVIEW_FILES = 11004,
		ID_FILE_OUTPUT   = 11005
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UFileSplit 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(0, 0, 600, 400);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_APP);
       this->setTimer(ID_TIMER_CLOCK, 1000);

		m_lv = new UListView(this, ID_LISTVIEW_FILES);
		m_lv->setStyles(LVS_REPORT | LVS_EDITLABELS);
		m_lv->setPos(50, 50, 300, 200);
		m_lv->create();

		m_lv->setExStylesListView(LVS_EX_GRIDLINES);

		m_lv->addColTextWidth(0, "index", 50);
		m_lv->addColTextWidth(1, "filename", 100);
		m_lv->addColTextWidth(2, "other", 150);

		m_lv->addItemTextImage(0, "1", 0);
		m_lv->setItemText(0, 1, _T("a.rar"));
		m_lv->addItemTextImage(1, "2", 0);
		m_lv->setItemText(1, 1, _T("b.rar"));
		m_lv->addItemTextImage(2, "3", 0);
		m_lv->setItemText(2, 1, _T("c.rar"));

		m_add = new UButton(this, ID_BUTTON_ADD);
		m_add->setPos(400, 60, 100, 50);
		m_add->create();
		m_add->setWindowText(_T("Add Files"));

        m_go = new UButton(this, ID_BUTTON_GO);
        m_go->setPos(400, 140, 100, 50);
        m_go->create();
        m_go->setWindowText(_T("Go"));

		m_output = new UEdit(this, ID_FILE_OUTPUT);
		m_output->setStyles(ES_MULTILINE);
		m_output->setPos(50, 270, 300, 50);
		m_output->create();

       return UBaseWindow::onCreate();
   }

   BOOL onClose()
   {
        this->killTimer(ID_TIMER_CLOCK);
        return UBaseWindow::onClose();
   }

    //
    virtual void onDraw(HDC hdc)
    {

    }
    //
    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc = {0};
        ::GetClientRect(*this, &rc);
        huys::URectangle urc(rc);
        urc.setFilledColor(huys::black);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
        return TRUE;
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if (ID_TIMER_CLOCK == wParam)
        {
            go_update();

            return FALSE;
        }
        return UBaseWindow::onTimer(wParam, lParam);
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_EXIT:
            return UBaseWindow::onClose();
        case ID_BUTTON_GO:
            return onBnGo();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
	huys::ADT::UAutoPtr<UButton> m_add;
    huys::ADT::UAutoPtr<UButton> m_go;
	huys::ADT::UAutoPtr<UListView> m_lv;
	huys::ADT::UAutoPtr<UEdit> m_output;
private:
    //
    void go_update()
    {
    }

    BOOL onBnGo()
    {

        return FALSE;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

