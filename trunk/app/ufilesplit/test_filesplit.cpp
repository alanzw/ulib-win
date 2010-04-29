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
#include "ustatic.h"
#include "uedit.h"
#include "ulistview.h"
#include "ucombobox.h"
#include "adt/uautoptr.h"
#include "adt/ustring.h"
#include "ucommondialog.h"
#include "umsg.h"

#include "aui/aui_label.h"
#include "aui/aui_button.h"

#include "ufilesplit.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_RADIOBTN_SPLIT = 10110,
        ID_RADIOBTN_MERGE = 10111,
        ID_BUTTON_ADD     = 11001,
        ID_BUTTON_GO      = 11002,
        ID_TIMER_CLOCK    = 11003,
        ID_LISTVIEW_FILES = 11004,
        ID_FILE_OUTPUT    = 11005,
        ID_FILE_SIZE      = 11006,
        ID_FILE_UNIT      = 11009,
        ID_BUTTON_SELECT  = 11007
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, GetModuleHandle(NULL))
   {
        this->setTitle(_T("UFileSplit 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(0, 0, 800, 600);
   }

   BOOL onCreate()
   {
       this->setIconBig(IDI_HELP);
       this->setTimer(ID_TIMER_CLOCK, 1000);

       m_rbSplit = new AUI::UTransRadioButton(this, ID_RADIOBTN_SPLIT);
       m_rbSplit->setPos(120, 20, 100, 20);
       m_rbSplit->create();
       m_rbSplit->setWindowText(_T("Split"));

       m_rbMerge = new AUI::UTransRadioButton(this, ID_RADIOBTN_MERGE);
       m_rbMerge->setPos(240, 20, 100, 20);
       m_rbMerge->create();
       m_rbMerge->setWindowText(_T("Merge"));

       m_lv = new UListView(this, ID_LISTVIEW_FILES);
       m_lv->setStyles(LVS_REPORT | LVS_EDITLABELS);
       m_lv->setPos(50, 50, 500, 400);
       m_lv->create();

       m_lv->setExStylesListView(LVS_EX_GRIDLINES);

       m_lv->addColTextWidth(0, "Index", 50);
       m_lv->addColTextWidth(1, "Filename", 300);
       m_lv->addColTextWidth(2, "Other", 100);

       m_add = new UButton(this, ID_BUTTON_ADD);
       m_add->setPos(580, 60, 100, 50);
       m_add->create();
       m_add->setWindowText(_T("Add Files"));

       m_size = new UComboBox(this, ID_FILE_SIZE);
       m_size->setStyles(CBS_DROPDOWN);
       m_size->setPos(580, 140, 60, 200);
       m_size->create();
       m_size->addText(_T("1"));
       m_size->addText(_T("10"));
       m_size->addText(_T("100"));
       m_size->addText(_T("1000"));
       m_size->setCurSel(0);

       m_unit = new UComboBox(this, ID_FILE_UNIT);
       m_unit->setStyles(CBS_DROPDOWNLIST);
       m_unit->setPos(650, 140, 40, 50);
       m_unit->create();
       m_unit->addText(_T("K"));
       m_unit->addText(_T("M"));
       m_unit->addText(_T("G"));
       m_unit->setCurSel(0);

       m_go = new UButton(this, ID_BUTTON_GO);
       m_go->setPos(580, 240, 100, 50);
       m_go->create();
       m_go->setWindowText(_T("Go"));

       m_output = new UEdit(this, ID_FILE_OUTPUT);
       m_output->setStyles(ES_MULTILINE);
       m_output->setPos(50, 470, 350, 50);
       m_output->create();

       m_select = new UButton(this, ID_BUTTON_SELECT);
       m_select->setPos(420, 470, 60, 50);
       m_select->create();
       m_select->setWindowText(_T("Select"));

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

   virtual BOOL onEraseBkgnd(HDC hdc)
   {
       RECT rc = {0};
       ::GetClientRect(*this, &rc);
       huys::URectangle urc(rc);
       urc.setFilledColor(huys::gray);
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
        case ID_BUTTON_ADD:
            return onBnAdd();
        case ID_BUTTON_GO:
            return onBnGo();
        case ID_BUTTON_SELECT:
            return onBnSelect();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }


private:
    huys::ADT::UAutoPtr<UButton> m_add;
    huys::ADT::UAutoPtr<UButton> m_go;
    huys::ADT::UAutoPtr<UListView> m_lv;
    huys::ADT::UAutoPtr<UEdit> m_output;
    huys::ADT::UAutoPtr<UButton> m_select;
    huys::ADT::UAutoPtr<UComboBox> m_size;
    huys::ADT::UAutoPtr<UComboBox> m_unit;
    huys::ADT::UStringAnsi m_sOutputFile;
    huys::ADT::UAutoPtr<URadioButton> m_rbSplit;
    huys::ADT::UAutoPtr<URadioButton> m_rbMerge;
private:
    //
    void go_update()
    {
    }

    BOOL onBnAdd()
    {
        UCommonDialog::UFileDialog filedlg(*this);
        filedlg.setFlags(OFN_ALLOWMULTISELECT|OFN_EXPLORER);
        if (filedlg.open())
        {
            const TCHAR * pbuff = filedlg.getName();
            huys::ADT::UStringAnsi filename = pbuff;
            huys::ADT::UStringAnsi tmp(MAX_PATH);

            if (filename.length() > filedlg.getFileOffset())
            {
                int nCount = m_lv->getItemCount();
                m_lv->addItemTextImage(nCount, tmp.format("%d", nCount+1), 0);
                m_lv->setItemText(nCount, 1, filename);
            }
            else
            {
                pbuff += filedlg.getFileOffset();

                while ('\0' != pbuff[0])
                {
                    int nCount = m_lv->getItemCount();
                    m_lv->addItemTextImage(nCount, tmp.format("%d", nCount+1), 0);
                    m_lv->setItemText(nCount, 1, tmp.format("%s\\%s", filename.c_str(), pbuff));

                    pbuff += lstrlen(pbuff)+1;
                }
            }
        }

        return FALSE;
    }

    BOOL onBnGo()
    {
        huys::ADT::UStringAnsi fin_name(MAX_PATH);
        huys::ADT::UStringAnsi fout_name(MAX_PATH);



        m_output->getWindowText(fout_name, MAX_PATH);

        UFileSplit::split(fout_name, 1000, 100, fout_name);


        return FALSE;
    }

    BOOL onBnSelect()
    {
        UCommonDialog::UFileDialog filedlg(*this);
        filedlg.setFlags(OFN_EXPLORER);
        if (filedlg.open())
        {
            m_sOutputFile = filedlg.getName();

            m_output->setWindowText(m_sOutputFile);
        }
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

