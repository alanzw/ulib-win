#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#include "ubtnst.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_BN_ST = 1002
    };
public:
    UMyWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL))
    {
        this->setTitle(_T("UBtnST Test 0.0.1"));
    }

    ~UMyWindow()
    {
        CHECK_PTR(m_pBnSt);
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        m_pBnSt = new UButtonST(this, ID_BN_ST);
        m_pBnSt->setPos(20, 20, 100, 100);
        m_pBnSt->create();
        m_pBnSt->setWindowText(_T("BtnST"));

        return UBaseWindow::onCreate();
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case ID_BN_ST:
            return onBnSt();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    UButtonST *m_pBnSt;

private:
    BOOL onBnSt()
    {
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

