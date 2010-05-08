#ifndef U_AUI_ABOUT_H
#define U_AUI_ABOUT_H

#include "ubasewindow.h"
#include "ubutton.h"
#include "uicon.h"
#include "adt/uautoptr.h"
#include "adt/ustring.h"

#define  UABOUTDIALOG_CLASSNAME "UABOUTDLG_{8E613694-04D1-4813-80A6-51EE5DBA3D6F}"

namespace AUI
{

class UAboutDialog : public UBaseWindow
{
public:
    UAboutDialog(HWND hWnd)
    : UBaseWindow(hWnd)
    {
        RECT rc;
        ::GetWindowRect(hWnd, &rc);
        ::InflateRect(&rc, -200, -50);
        setRect(&rc);
        setTitle(_T("About"));
        setWndClassName(UABOUTDIALOG_CLASSNAME);
    }

    UAboutDialog(UBaseWindow *pWnd)
        : UBaseWindow(pWnd)
    {
        RECT rc;
        ::GetWindowRect(*pWnd, &rc);
        ::InflateRect(&rc, -200, -50);
        setRect(&rc);
        setTitle(_T("About"));
        setWndClassName(UABOUTDIALOG_CLASSNAME);
    }

    BOOL onCreate()
    {
        setIconBig(IDI_HELP);
        UGroupBox groupbox(this, -1);
        groupbox.setPos(30, 150, 330, 100);
        groupbox.create();

        return TRUE;
    }

    BOOL onClose()
    {
        return this->hide();
    }

    void onDraw(HDC hdc)
    {
        UIcon icon;
        icon.loadImage(::GetModuleHandle(NULL), IDI_HELP, 128, 128);
        icon.drawEx(hdc, 20, 20, 128, 128);
        ::SetBkMode(hdc, TRANSPARENT);
        ::TextOut(hdc, 200, 100, _header, _header.length());
        ::TextOut(hdc, 60, 200, _description, _description.length());
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
private:
    huys::ADT::UStringAnsi _header;
    huys::ADT::UStringAnsi _description;
};

typedef huys::ADT::UAutoPtr<UAboutDialog> UAboutDialogP;

}; // namespace AUI

#endif // U_AUI_ABOUT_H
