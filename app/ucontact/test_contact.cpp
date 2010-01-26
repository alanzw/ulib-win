#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ustatic.h"
#include "ulistview.h"
#include "ubitmap.h"
#include "uedit.h"
#include "ucombobox.h"

#include "adt/uautoptr.h"

class UTransStatic : public UStatic
{
public:
    UTransStatic(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
    }
    
    UTransStatic(UBaseWindow *pWndParent, UINT nID)
    : UStatic(pWndParent, nID)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::createEx(WS_EX_TRANSPARENT, _T("STATIC"), m_pText);
        this->subclassProc();
        return  bRet;
    }

    BOOL onMessage( UINT message, WPARAM wParam, LPARAM lParam )
    {
        BOOL bRet = UStatic::onMessage(message, wParam, lParam);

        if (WM_SETTEXT == message)
        {
            onSetText();
        }

        return bRet;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        return (BOOL)(HBRUSH)::GetStockObject(HOLLOW_BRUSH);
    }
private:
    void onSetText()
    {
        RECT rc;
        ::GetWindowRect(m_hSelf, &rc);
        ::ScreenToClient(m_hParent,(LPPOINT) &rc);
		//::ScreenToClient(m_hParent,(LPPOINT) (&rc+1));
        ::InvalidateRect(m_hParent, &rc, TRUE);
        ::UpdateWindow(m_hParent);
    }
};

class UMyWindow : public UBaseWindow
{
    enum {
        ID_LISTCTRL = 11002,
        ID_ED_NAME  = 11003,
        ID_CB_SEX   = 11004,
        ID_ED_ADDR  = 11005
    };
public:
   UMyWindow()
   : UBaseWindow(NULL, ::GetModuleHandle(NULL))
   {
        this->setTitle(_T("UContact Test 0.0.1"));
   }

   BOOL onCreate()
   {
        us[0] = new UTransStatic(this, 1333);
        us[0]->setPos(20, 20, 50, 20);
        us[0]->create();
        us[0]->setWindowText("Name:");

       m_pEdtName = new UEdit(this, ID_ED_NAME);
       m_pEdtName->setPos(80, 20, 100, 25);
       m_pEdtName->create();

       us[1] = new UTransStatic(this, 2333);
       us[1]->setPos(200, 20, 30, 25);
       us[1]->create();
       us[1]->setWindowText("Sex:");

       m_pCboSex = new UComboBox(this, ID_CB_SEX);
       m_pCboSex->setStyles(CBS_DROPDOWN);
       m_pCboSex->setPos(240, 20, 100, 25);
       m_pCboSex->create();
       m_pCboSex->addText(_T("F"));
       m_pCboSex->addText(_T("M"));

       us[2] = new UTransStatic(this, 3333);
       us[2]->setPos(360, 20, 60, 25);
       us[2]->create();
       us[2]->setWindowText("Address:");

       m_pEdtAddr= new UEdit(this, ID_ED_ADDR);
       m_pEdtAddr->setPos(440, 20, 200, 25);
       m_pEdtAddr->create();

       m_pContactList = new UListView(this, ID_LISTCTRL);
       m_pContactList->setPos(18, 50, 622, 300);
       m_pContactList->setStyles(WS_BORDER | LVS_REPORT);
       m_pContactList->create();

       m_pContactList->addColTextWidth(0, "name", 150);
       m_pContactList->addColTextWidth(1, "sex", 50);
       m_pContactList->addColTextWidth(2, "address", 300);

       m_pContactList->addItemTextImage(0, "fox", 0);
       m_pContactList->setItemText(0, 1, _T("f"));
       m_pContactList->setItemText(0, 2, _T("Another world"));

       m_pBack = new UBitmap(IDB_BACKGROUND, getInstance());

       return UBaseWindow::onCreate();
   }

    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc;
        this->getClientRect(&rc);
        m_pBack->showStretch(hdc, rc);
        return TRUE;
    }
private:
    huys::ADT::UAutoPtr<UListView> m_pContactList;
    huys::ADT::UAutoPtr<UBitmap> m_pBack;
    huys::ADT::UAutoPtr<UEdit> m_pEdtName;
    huys::ADT::UAutoPtr<UComboBox> m_pCboSex;
    huys::ADT::UAutoPtr<UEdit> m_pEdtAddr;
    
    huys::ADT::UAutoPtr<UTransStatic> us[3];

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

