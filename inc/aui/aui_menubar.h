#ifndef U_AUI_MENUBAR_H
#define U_AUI_MENUBAR_H

#include "ubasewindow.h"
#include "ulayout.h"
#include "udc.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"
#include "aui/aui_label.h"
#include "aui/aui_entry.h"
#include "adt/uvector.h"

namespace AUI
{

class UMenuBar : public UBaseWindow
{
public:
    UMenuBar(UBaseWindow *pWndParent)
    : UBaseWindow(pWndParent)
    {
        setMenu(0);
        setWndClassName(_T("HUYS_MENUBAR_WINDOW_CLASS"));
        addStyles(WS_CHILD);

        _hMenu = 0;
        _nFocusedItem = -1;
    }
    
    UMenuBar(HWND hParent)
    : UBaseWindow(hParent)
    {
        setMenu(0);
        setWndClassName(_T("HUYS_MENUBAR_WINDOW_CLASS"));
        addStyles(WS_CHILD);
        
        _hMenu = 0;
        _nFocusedItem = -1;
    }

    BOOL onCreate()
    {
   
        if (0 != _hMenu)
        {
            char buffer[256];   
            int idx = 0;
            while (0 != ::GetMenuString(_hMenu, idx, buffer, 256, MF_BYPOSITION))
            {
                if('&' == buffer[0])
                {
                    buffer[0] = ' ';
                }
                _texts.push_back(buffer);
                
                ++idx;
            }
        }

        return UBaseWindow::onCreate();
    }

    BOOL onClose()
    {
        this->hide();
        return FALSE;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {

        return FALSE;
    }

    void onDraw(HDC hdc)
    {

    }

    BOOL onEraseBkgnd(HDC hdc)
    {
        USmartDC dc(hdc);
        huys::URectL rect;
        this->getClientRect(rect);
        dc.selectObj(::GetStockObject(NULL_PEN));
        dc.setBrushColor(huys::xpblue);
        dc.rectangle(rect);
        dc.setBKMode(TRANSPARENT);

        LOG_STRING("%d", _nFocusedItem);

        if (_nFocusedItem != -1)
        {
            huys::URectL rectItem = rect;
            rectItem.setWidth(50);
            rectItem.offset(50*_nFocusedItem, 0);

            dc.setBrushColor(huys::red);
            dc.rectangle(rectItem);
        }       
        
        for (int i=0; i<_texts.size(); ++i)
        {
            dc.textOutEx(5+i*50, 10, _texts.at(i));
        }
        return TRUE;
    }
    
    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        
        LONG x = LOWORD(lParam);
		LONG y = HIWORD(lParam);
        
        int nPos =  x/50;

        _nFocusedItem = nPos;
        
        HMENU hMenu = ::GetSubMenu(_hMenu, nPos);

        if (0 != hMenu)
        {
            huys::URectL rect;
            this->getWindowRect(rect);
            //rect.clientToScreen(getHandle());
            //rect.screenToClient(getParent());

            huys::URectL rectClient;
            this->getClientRect(rectClient);
            rectClient.setWidth(50);
            rectClient.offset(nPos*50, 0);

            this->invalidate(TRUE);
            
            POINT pt;
            ::GetCursorPos(&pt);
            ::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect.left()+nPos*50, rect.bottom(),
                0, getParent(), 0);
            _nFocusedItem = -1;
            this->invalidate(TRUE);

        }

//         if (-1 != _nFocusedItem)
//         {
//             TRACKMOUSEEVENT tme;
//             tme.cbSize = sizeof(TRACKMOUSEEVENT);
//             tme.dwFlags = TME_LEAVE;
//             tme.hwndTrack = getHandle();
//             
//             TrackMouseEvent(&tme);
//         }

        return UBaseWindow::onLButtonDown(wParam, lParam);
    }

//     BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
//     {
//         //_nFocusedItem = -1;
//         //this->invalidate(TRUE);
//         return UBaseWindow::onLButtonUp(wParam, lParam);
//     }

//     BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
//     {
//         return UBaseWindow::onMouseMove(wParam, lParam);
//     }

//      BOOL filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
//      {
//         if ( WM_MOUSEACTIVATE == uMessage )
//         {
//             _nFocusedItem = -1;
//             this->invalidate(TRUE);
//         }
//         
//         
//         return UBaseWindow::filterMessage(uMessage, wParam, lParam);
//     }

public:
    void setMenuName(HINSTANCE hInst, LPCTSTR lpMenuName)
    {
        _hMenu = ::LoadMenu(hInst, lpMenuName);
    }

//     BOOL onMouseLeave(WPARAM wParam, LPARAM lParam)
//     {
//         _nFocusedItem = -1;
//         this->invalidate(TRUE);
// 
//         return TRUE;
private:
    HMENU _hMenu;
    typedef huys::ADT::UStringAnsi UStringT;
    typedef huys::ADT::UVector<UStringT> TextArray;
    TextArray _texts;
    int _nFocusedItem;
};

typedef huys::ADT::UAutoPtr<UMenuBar> UMenuBarP;

}; // namespace AUI

#endif // U_AUI_MENUBAR_H
