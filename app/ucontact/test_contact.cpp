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
   : UBaseWindow(NULL, ::GetModuleHandle(NULL)),
     m_pContactList(0),
     m_pBack(0),
     m_pEdtName(0),
     m_pCboSex(0),
     m_pEdtAddr(0)
   {
        this->setTitle(_T("UContact Test 0.0.1"));
   }

   ~UMyWindow()
   {
           CHECK_PTR(m_pContactList);
        CHECK_PTR(m_pBack);
        CHECK_PTR(m_pEdtName);
        CHECK_PTR(m_pCboSex);
        CHECK_PTR(m_pEdtAddr);
   }

   BOOL onCreate()
   {
       UStatic us1(this, "Name:");
          us1.setPos(20, 20, 60, 20);
       us1.create();

       m_pEdtName = new UEdit(this, ID_ED_NAME);
       m_pEdtName->setPos(80, 20, 100, 25);
       m_pEdtName->create();

       UStatic us2(this, "Sex:");
       us2.setPos(200, 20, 60, 25);
       us2.create();

       m_pCboSex = new UComboBox(this, ID_CB_SEX);
       m_pCboSex->setStyles(CBS_DROPDOWN);
       m_pCboSex->setPos(240, 20, 100, 25);
       m_pCboSex->create();
       m_pCboSex->addText(_T("F"));
       m_pCboSex->addText(_T("M"));

       UStatic us3(this, "Address:");
       us3.setPos(360, 20, 60, 25);
       us3.create();

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
    UListView *m_pContactList;
    UBitmap *m_pBack;
    UEdit *m_pEdtName;
    UComboBox *m_pCboSex;
    UEdit *m_pEdtAddr;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    UMyWindow *pWnd = new UMyWindow;
    app.setMainWindow(pWnd);
    app.init(hInstance);
    pWnd->setIconBig(IDI_APP);
    return app.run();
}

