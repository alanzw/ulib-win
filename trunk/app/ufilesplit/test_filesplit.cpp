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
#include "adt/uvector.h"
#include "uimagelist.h"

#include "uthread.h"

#include "aui/aui_label.h"
#include "aui/aui_button.h"
#include "aui/aui_aboutdlg.h"

#include "ufilesplit.h"

HMODULE hmod = ::LoadLibrary(_T("exchndl.dll"));

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
        ID_BUTTON_SELECT  = 11007,
        ID_BUTTON_CLEAR   = 11008,
        ID_COMBO_BUFFER   = 11010
    };

    enum USMode {
        MODE_SPLIT,
        MODE_MERGE
    };

public:
    UMyWindow()
        : UBaseWindow(NULL, GetModuleHandle(NULL))
    {
        this->setTitle(_T("UFileSplit 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
        this->setPos(0, 0, 800, 600);
        this->addExStyles(WS_EX_ACCEPTFILES);
        mode = MODE_SPLIT;
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_HELP);
        this->setTimer(ID_TIMER_CLOCK, 1000);

        m_rbSplit = new AUI::UTransRadioButton(this, ID_RADIOBTN_SPLIT);
        m_rbSplit->setPos(120, 20, 100, 20);
        m_rbSplit->create();
        m_rbSplit->setWindowText(_T("Split"));
        m_rbSplit->check();

        m_rbMerge = new AUI::UTransRadioButton(this, ID_RADIOBTN_MERGE);
        m_rbMerge->setPos(240, 20, 100, 20);
        m_rbMerge->create();
        m_rbMerge->setWindowText(_T("Merge"));

        m_lv = new UListView(this, ID_LISTVIEW_FILES);
        m_lv->setStyles(LVS_REPORT | LVS_EDITLABELS);
        m_lv->setPos(50, 50, 500, 400);
        m_lv->create();

        m_lv->setExStylesListView(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

        // Change ListView Item Height
        UImageList ml;
        ml.create(1,24,TRUE|ILC_COLOR32,1,0);
        m_lv->setImageListSmall(ml);

        m_lv->addColTextWidth(0, "Index", 50);
        m_lv->addColTextWidth(1, "Filename", 300);
        m_lv->addColTextWidth(2, "Other", 100);

        m_lv->disable();

        m_add = new UButton(this, ID_BUTTON_ADD);
        m_add->setPos(580, 60, 100, 50);
        m_add->create();
        m_add->setWindowText(_T("Add Files"));

        m_add->disable();

        m_clear = new UButton(this, ID_BUTTON_CLEAR);
        m_clear->setPos(580, 130, 100, 50);
        m_clear->create();
        m_clear->setWindowText(_T("Clear All"));

        m_lbBufferSize = new AUI::UTransLabel(this, _T("Buffer Size (Bytes):"));
        m_lbBufferSize->setStyles(ES_LEFT);
        m_lbBufferSize->setPos(580, 220, 140, 20);
        m_lbBufferSize->create();

        m_cbBuffer = new UComboBox(this, ID_COMBO_BUFFER);
        m_cbBuffer->setPos(580, 250, 100, 30);
        m_cbBuffer->create();
        m_cbBuffer->addText(_T("1000"));
        m_cbBuffer->setItemData(0, 1000);
        m_cbBuffer->addText(_T("10000"));
        m_cbBuffer->setItemData(1, 10000);
        m_cbBuffer->addText(_T("100000"));
        m_cbBuffer->setItemData(2, 100000);
        m_cbBuffer->setCurSel();

        m_lbSize = new AUI::UTransLabel(this, _T("Size:"));
        m_lbSize->setStyles(ES_LEFT);
        m_lbSize->setPos(580, 290, 100, 20);
        m_lbSize->create();

        m_size = new UComboBox(this, ID_FILE_SIZE);
        m_size->setStyles(CBS_DROPDOWN);
        m_size->setPos(580, 320, 60, 200);
        m_size->create();
        m_size->addText(_T("1"));
        m_size->addText(_T("10"));
        m_size->addText(_T("100"));
        m_size->addText(_T("1000"));
        m_size->setCurSel(0);

        m_unit = new UComboBox(this, ID_FILE_UNIT);
        m_unit->setStyles(CBS_DROPDOWNLIST);
        m_unit->setPos(650, 320, 40, 50);
        m_unit->create();
        m_unit->addText(_T("K"));
        m_unit->setItemData(0, 1000);
        m_unit->addText(_T("M"));
        m_unit->setItemData(1, 1000000);
        m_unit->addText(_T("G"));
        m_unit->setItemData(2, 1000000000);
        m_unit->setCurSel(0);

        m_go = new UButton(this, ID_BUTTON_GO);
        m_go->setPos(580, 360, 100, 50);
        m_go->create();
        m_go->setWindowText(_T("Go"));

        m_output = new UEdit(this, ID_FILE_OUTPUT);
        m_output->setStyles(ES_MULTILINE);
        m_output->setPos(120, 470, 350, 50);
        m_output->create();

        m_lbOutput = new AUI::UTransLabel(this, _T("Input Name"));
        m_lbOutput->setPos(50, 470, 50, 50);
        m_lbOutput->create();

        m_select = new UButton(this, ID_BUTTON_SELECT);
        m_select->setPos(480, 470, 60, 50);
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
        urc.setFilledColor(huys::skyblue);
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
        case IDM_ABOUT:
            return onMenuAbout();
        case ID_BUTTON_ADD:
            return onBnAdd();
        case ID_BUTTON_CLEAR:
            return onBnClear();
        case ID_BUTTON_GO:
            return onBnGo();
        case ID_BUTTON_SELECT:
            return onBnSelect();
        case ID_RADIOBTN_SPLIT:
            return onRaSplit();
        case ID_RADIOBTN_MERGE:
            return onRaMerge();
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

    BOOL onDropFiles(WPARAM wParam, LPARAM lParam)
    {
        HDROP hDropInfo = (HDROP)wParam;
        WORD wNumFilesDropped = ::DragQueryFile(hDropInfo, -1, NULL, 0);
        //showMsgFormat(_T("dropfile"), _T("%u"), wNumFilesDropped);
        huys::ADT::UString<char, MAX_PATH> filename;
        huys::ADT::UString<char, MAX_PATH> tmp;
        for (WORD w=0; w<wNumFilesDropped; ++w)
        {
            DragQueryFile(hDropInfo, w, filename, MAX_PATH);
            if (!fin_names.is_already_exist(filename))
            {
                fin_names.push_back(filename);

                int nCount = m_lv->getItemCount();
                m_lv->addItemTextImage(nCount, tmp.format("%d", nCount+1), 0);
                m_lv->setItemText(nCount, 1, filename);
            }
        }

        return FALSE;
    }

    void run()
    {

    DWORD dwBufSize = m_cbBuffer->getItemData(m_cbBuffer->getCurSel());

    if (MODE_SPLIT == mode)
    {
        DWORD dwEachSize;

        huys::ADT::UStringAnsi strSize;

        m_size->getWindowText(strSize, 10);

        dwEachSize = atoi(strSize) * m_unit->getItemData(m_unit->getCurSel());

        huys::ADT::UStringAnsi fin_name(MAX_PATH);
        huys::ADT::UStringAnsi fout_name(MAX_PATH);

        m_output->getWindowText(fin_name, MAX_PATH);

        fout_name = fin_name;

        UFileSplit::split(fin_name, dwEachSize, dwBufSize, fout_name);

    }
    else
    {

        huys::ADT::UString<char, MAX_PATH> fin_name(MAX_PATH);
        huys::ADT::UString<char, MAX_PATH> fout_name(MAX_PATH);

        m_output->getWindowText(fout_name, MAX_PATH);

        //huys::ADT::UVector<huys::ADT::UString<char, MAX_PATH> > fin_names;

        int n = m_lv->getItemCount();

        //for (int i=0; i<n; ++i)
        //{
        //    m_lv->getItemText(i, 1, fin_name, MAX_PATH);
        //    fin_name.update();
        //    fin_names.push_back(fin_name);
        //}
        UFileSplit::merge(fin_names, n, dwBufSize, fout_name);
        }

    }

    static DWORD WINAPI thread_routine(LPVOID lpThreadParameter)
    {
        UMyWindow *p = (UMyWindow *)lpThreadParameter;
        p->run();
        return 0;
    }
private:
    huys::ADT::UAutoPtr<UButton> m_add;
    huys::ADT::UAutoPtr<UButton> m_clear;
    huys::ADT::UAutoPtr<UButton> m_go;
    huys::ADT::UAutoPtr<UListView> m_lv;
    huys::ADT::UAutoPtr<UEdit> m_output;
    huys::ADT::UAutoPtr<UButton> m_select;
    huys::ADT::UAutoPtr<UComboBox> m_size;
    huys::ADT::UAutoPtr<UComboBox> m_unit;
    huys::ADT::UStringAnsi m_sOutputFile;
    AUI::UTransRadioButtonP m_rbSplit;
    AUI::UTransRadioButtonP m_rbMerge;
    AUI::UTransLabelP m_lbSize;
    AUI::UTransLabelP m_lbBufferSize;
    AUI::UTransLabelP m_lbOutput;
    huys::ADT::UAutoPtr<UComboBox> m_cbBuffer;

    USMode mode;

    huys::ADT::UVector<huys::ADT::UString<char, MAX_PATH> > fin_names;

    AUI::UAboutDialogP aboutDlg;
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
            huys::ADT::UString<char, MAX_PATH> filename = pbuff;
            huys::ADT::UString<char, MAX_PATH> tmp;
            huys::ADT::UString<char, MAX_PATH> str;

            if (filename.length() > filedlg.getFileOffset())
            {
                if (!fin_names.is_already_exist(filename))
                {
                    fin_names.push_back(filename);

                    int nCount = m_lv->getItemCount();
                    m_lv->addItemTextImage(nCount, tmp.format("%d", nCount+1), 0);
                    m_lv->setItemText(nCount, 1, filename);
                }
            }
            else
            {
                pbuff += filedlg.getFileOffset();

                while ('\0' != pbuff[0])
                {
                    str.format("%s\\%s", filename.c_str(), pbuff);
                    if (!fin_names.is_already_exist(str))
                    {
                          fin_names.push_back(str);
                          int nCount = m_lv->getItemCount();
                          m_lv->addItemTextImage(nCount, tmp.format("%d", nCount+1), 0);
                          m_lv->setItemText(nCount, 1, str);
                    }
                    pbuff += lstrlen(pbuff)+1;
                }
            }
        }

        return FALSE;
    }

    BOOL onBnClear()
    {
        m_lv->deleteAllItems();
        m_output->clear();
        fin_names.clear();

        return FALSE;
    }

    BOOL onBnGo()
    {
        UThread t(&thread_routine, this);
        t.create();

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

    BOOL onRaSplit()
    {
        m_lv->disable();
        m_add->disable();
        m_clear->disable();

        m_size->enable();
        m_unit->enable();

        mode = MODE_SPLIT;

        m_lbOutput->hide();
        m_lbOutput->setWindowText(_T("Input Name"));
        m_lbOutput->show();

        return FALSE;
    }

    BOOL onRaMerge()
    {
        m_lv->enable();
        m_add->enable();
        m_clear->enable();

        m_size->disable();
        m_unit->disable();

        mode = MODE_MERGE;

        m_lbOutput->hide();
        m_lbOutput->setWindowText(_T("Output Name"));
        m_lbOutput->show();

        return FALSE;
    }

    BOOL onMenuAbout()
    {
        //showMsg(_T("About UFileSplit 0.0.1"));

        if (aboutDlg == 0)
        {
            aboutDlg = new AUI::UAboutDialog(this);
            aboutDlg->setHeader("UFileSplit 0.0.1");
            aboutDlg->setDescription("UFileSplit is written by huys03@gmail.com");
            aboutDlg->setCopyright("Copyright (C) 2010 huys");
            aboutDlg->addStyles(WS_SYSMENU);
            aboutDlg->create();
        }
        else
        {
            aboutDlg->show();
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

