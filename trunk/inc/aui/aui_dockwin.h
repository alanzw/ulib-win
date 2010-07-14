#ifndef U_AUI_DOCKWIN_H
#define U_AUI_DOCKWIN_H

#include "ubasewindow.h"
#include "ulayout.h"

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
        RECT rc;
        ::GetClientRect(getParent(), &rc);
        rc.left = rc.right - 200;
        setRect(&rc);
        setMenu(0);
        setWndClassName(_T("HUYS_DOCK_WINDOW_CLASS"));
        setTitle(_T("DOCK"));

        addStyles(WS_CHILD);
        setExStyles(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_APPWINDOW);
    }

    BOOL onCreate()
    {
        _label = new AUI::UTransLabel(this, _T("Properties"));
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
        RECT rc;
        ::GetClientRect(getParent(), &rc);
        rc.left = rc.right - 200;
        this->moveWindow(&rc);

        this->getClientRect(&rc);

        rc.top += 50;

        huys::layout::UVBoxLayout layout;
        layout.setRect(&rc);
        layout.addControl(_label);
        layout.addControl(_entry1);
        layout.addControl(_entry2);
        layout.go();

        return FALSE;
    }

    void onDraw(HDC hdc)
    {
        HBRUSH hbTitle = ::CreateSolidBrush(huys::xpblue);
        RECT rcWindow = {0};
        this->getClientRect(&rcWindow);

        RECT rcTitle = {
            rcWindow.left,
            rcWindow.top,
            rcWindow.right,
            rcWindow.top + 20
        };

        ::FillRect( hdc, &rcTitle, hbTitle);
        ::SetTextColor( hdc, huys::white );
        ::SetBkColor( hdc, huys::xpblue );
        LPCTSTR m_sTitle = _T("dock");
        ::TextOut( hdc,
                   rcWindow.left+5,
                   rcWindow.top+2,
                   m_sTitle,
                   lstrlen(m_sTitle));

        ::TextOut( hdc,
                   rcWindow.right-20,
                   rcWindow.top+2,
                   "X",
                   1);

    }
private:
    AUI::UTransLabelP _label;
    AUI::UIntEntryP _entry1;
    AUI::UEntryP _entry2;
};

typedef huys::ADT::UAutoPtr<UDockWindow> UDockWindowP;

}; // namespace AUI

#endif // U_AUI_DOCKWIN_H
