#ifndef U_AUI_DOCKWIN_H
#define U_AUI_DOCKWIN_H

#include "ubasewindow.h"
#include "ulayout.h"
#include "udc.h"

#include "adt/uautoptr.h"

#include "aui/aui_label.h"
#include "aui/aui_entry.h"

namespace AUI
{

class UDockWindow : public UBaseWindow
{
public:
    UDockWindow(UBaseWindow *pWndParent)
    : UBaseWindow(pWndParent)
    {
        huys::URectL rect;
        ::GetClientRect(getParent(), rect);
        rect.setLeft(rect.right()-200);
        setRect(rect);

        setMenu(0);
        setWndClassName(_T("HUYS_DOCK_WINDOW_CLASS"));
        setTitle(_T("DOCK"));

        addStyles(WS_CHILD);
        setExStyles(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_APPWINDOW);
    }

    BOOL onCreate()
    {
        _label = new AUI::UTransLabel(this, _T("Properties"));
		_label->setStyles(SS_CENTER|SS_CENTERIMAGE);
		_label->setTextColor(huys::blueviolet);
        _label->setPos(40, 40, 100, 100);
        _label->create();

        _entry1 = new AUI::UIntEntry(this, 1234);
        _entry1->create();

        _entry2 = new AUI::UEntry(this, 1235);
        _entry2->create();

        return UBaseWindow::onCreate();
    }

    BOOL onClose()
    {
        this->hide();
        return FALSE;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        huys::URectL rect;

        ::GetClientRect(getParent(), rect);
        rect.setLeft(rect.right()-200);
        this->moveWindow(rect);

        this->getClientRect(rect);

        rect.offsetY(50, 0);

        huys::layout::UVBoxLayout layout;
        layout.setRect(rect);
        layout.addControl(_label);
        layout.addControl(_entry1);
        layout.addControl(_entry2);
        layout.go();

        return FALSE;
    }

    void onDraw(HDC hdc)
    {
        USmartDC dc(hdc);

		HBRUSH hbTitle = ::CreateSolidBrush(huys::xpblue);
        
		huys::URectL rcWindow;
        this->getClientRect(rcWindow);

        huys::URectL rcTitle = rcWindow;
		rcTitle.setHeight(20);

		dc.fillRect(rcTitle, hbTitle);
		dc.setTextColor(huys::white);
		dc.setBKColor(huys::xpblue);

        dc.textOutEx(rcWindow.left()+5, rcWindow.top()+2, _T("dock"));
        dc.textOutEx(rcWindow.right()-20, rcWindow.top()+2, _T("X"));

    }
private:
    AUI::UTransLabelP _label;
    AUI::UIntEntryP _entry1;
    AUI::UEntryP _entry2;
};

typedef huys::ADT::UAutoPtr<UDockWindow> UDockWindowP;

}; // namespace AUI

#endif // U_AUI_DOCKWIN_H
