#ifndef U_AUI_ABOUT_H
#define U_AUI_ABOUT_H

#include "ubasewindow.h"
#include "ubutton.h"

#include "uicon.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

#include "aui/aui_button.h"

#define  UABOUTDIALOG_CLASSNAME "UABOUTDLG_{8E613694-04D1-4813-80A6-51EE5DBA3D6F}"

namespace AUI
{

class UAboutDialog : public UBaseWindow
{
public:
    UAboutDialog(HWND hParent)
    : UBaseWindow(hParent)
    {
        huys::URectL rect;
        ::GetWindowRect(hParent, rect);
        rect.inflate(-200, -50);
        setRect(rect);

        setTitle(_T("About"));
        setWndClassName(UABOUTDIALOG_CLASSNAME);

        _hModule = NULL;
    }

    UAboutDialog(UBaseWindow *pWndParent)
        : UBaseWindow(pWndParent)
    {
        huys::URectL rect;
        pWndParent->getWindowRect(rect);
        rect.inflate(-200, -50);
        setRect(rect);

        setTitle(_T("About"));
        setWndClassName(UABOUTDIALOG_CLASSNAME);

        _hModule = NULL;
    }

    BOOL onCreate()
    {
        setIconBig(IDI_HELP);

        huys::URectL rect;
        this->getClientRect(rect);

        UGroupBox groupbox(this, -1);
        groupbox.setPos(30, 150, 330, 100);
        groupbox.create();

        _buttonOK = new UButton(this, IDOK);
        _buttonOK->setPos(rect.right()-100, rect.bottom()-100, 50, 50);
        _buttonOK->create();
        _buttonOK->setWindowText(_T("OK"));

        return TRUE;
    }

    BOOL onClose()
    {
        return this->hide();
    }

    void onDraw(HDC hdc)
    {
        USmartDC dc(hdc);
        
        UIcon icon;

        if (NULL == _hModule)
        {
            _hModule = ::GetModuleHandle(NULL);
        }

        icon.loadImage(_hModule, IDI_HELP, 128, 128);
        
        icon.drawEx(dc, 20, 20, 128, 128);

        
        dc.setBKMode(TRANSPARENT);
        dc.textOutEx(200, 100, _header);
        dc.textOutEx(60, 200, _description);
        dc.textOutEx(100, 300, _copyright);
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch(LOWORD(wParam))
        {
        case IDOK:
            return onClose();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setBKBrush((HBRUSH)::GetStockObject(LTGRAY_BRUSH));
        return UBaseWindow::onPreRegisterWindowClass(uwc);
    }

    void setHeader(const char *text)
    {
        _header = text;
    }

    void setDescription(const char *desc)
    {
        _description = desc;
    }

    void setCopyright(const char *copy)
    {
        _copyright = copy;
    }

    void setModule(HINSTANCE hModule)
    {
        _hModule = hModule;
    }
private:
    huys::ADT::UStringAnsi _header;
    huys::ADT::UStringAnsi _description;
    huys::ADT::UStringAnsi _copyright;

    HINSTANCE _hModule;

    AUI::UButtonP _buttonOK;
};

typedef huys::ADT::UAutoPtr<UAboutDialog> UAboutDialogP;

}; // namespace AUI

#endif // U_AUI_ABOUT_H
