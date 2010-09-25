#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ugdi.h"
#include "colors.h"
#include "adt/uautoptr.h"

#include "aui/aui_button.h"
#include "aui/aui_label.h"
#include "aui/aui_entry.h"
#include "ulistview.h"
#include "ucombobox.h"
#include "ubooks.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_TERMINAL = 11002
    };
public:
    UMyWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("UBooks Test 0.0.1"));
        this->setMenu(MAKEINTRESOURCE(IDR_MENU_MAIN));
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        _gbBook = new AUI::UTransGroupBox(this, -1);
        _gbBook->setPos(100, 20, 400, 200);
        _gbBook->create();
        _gbBook->setWindowText(_T("Books"));

        _gbDetail = new AUI::UTransGroupBox(this, -1);
        _gbDetail->setPos(100, 250, 400, 200);
        _gbDetail->create();
        _gbDetail->setWindowText(_T("Detail"));

        m_pListCtrl = new UListView(this, -1);
        m_pListCtrl->setPos(110, 40, 380, 170);
        m_pListCtrl->setStyles(LVS_REPORT | LVS_EDITLABELS);
        m_pListCtrl->create();

        m_pListCtrl->addColTextWidth(0, _T("NO."), 40);
        m_pListCtrl->addColTextWidth(1, _T("Title"), 50);
        m_pListCtrl->addColTextWidth(2, _T("Author"), 50);
        m_pListCtrl->addColTextWidth(3, _T("Genre"), 50);
        m_pListCtrl->addColTextWidth(4, _T("Year"), 50);
        m_pListCtrl->addColTextWidth(5, _T("Rating"), 140);

        huys::URectL rect;
        rect.set(110, 280, 50, 20);

        _labelTitle = new AUI::UTransLabel(this, _T("Title:"));
        _labelTitle->setRect(rect);
        _labelTitle->create();

        rect.offsetY(30, 30);

        _labelAuthor= new AUI::UTransLabel(this, _T("Author:"));
        _labelAuthor->setRect(rect);
        _labelAuthor->create();

        rect.offsetY(30, 30);

        _labelGenre= new AUI::UTransLabel(this, _T("Genre:"));
        _labelGenre->setRect(rect);
        _labelGenre->create();

        rect.offsetY(30, 30);

        _labelYear= new AUI::UTransLabel(this, _T("Year:"));
        _labelYear->setRect(rect);
        _labelYear->create();

        rect.offsetY(30, 30);

        _labelRating= new AUI::UTransLabel(this, _T("Rating:"));
        _labelRating->setRect(rect);
        _labelRating->create();

        rect.set(160, 280, 330, 20);

        _entryTitle = new AUI::UEntry(this, -1);
        //_entryTitle->setStyles(WS_BORDER);
        _entryTitle->setRect(rect);
        _entryTitle->create();

        rect.offsetY(30, 60);

        _comboAuthor = new UComboBox(this, -1);
        _comboAuthor->setRect(rect);
        _comboAuthor->create();

        rect.offsetY(30, 60);

        _comboGenre = new UComboBox(this, -1);
        _comboGenre->setRect(rect);
        _comboGenre->create();

        rect.offsetY(30, 60);

        _comboYear = new UComboBox(this, -1);
        _comboYear->setRect(rect);
        _comboYear->create();

        rect.offsetY(30, 60);

        _comboRating = new UComboBox(this, -1);
        _comboRating->setRect(rect);
        _comboRating->create();

        return UBaseWindow::onCreate();
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
        urc.setFilledColor(huys::pink);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);
        return TRUE;
    }

    //
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDM_ABOUT:
            return onMenuAbout();
        case IDM_EXIT:
            return UBaseWindow::onClose();
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
    AUI::UTransGroupBoxP _gbBook;
    AUI::UTransGroupBoxP _gbDetail;
    huys::ADT::UAutoPtr<UListView> m_pListCtrl;
    AUI::UTransLabelP _labelTitle;
    AUI::UTransLabelP _labelAuthor;
    AUI::UTransLabelP _labelGenre;
    AUI::UTransLabelP _labelYear;
    AUI::UTransLabelP _labelRating;
    AUI::UEntryP _entryTitle;
    huys::ADT::UAutoPtr<UComboBox> _comboAuthor;
    huys::ADT::UAutoPtr<UComboBox> _comboGenre;
    huys::ADT::UAutoPtr<UComboBox> _comboYear;
    huys::ADT::UAutoPtr<UComboBox> _comboRating;
private:
    BOOL onMenuAbout()
    {
        this->showMsg(_T("UBooks v0.0.1"), _T("About"));
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

