#include "resource.h"
#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"
#include "ubasewindow.h"

#include "aui/aui.h"

#include "ushell.h"

class UMyWindow : public UBaseWindow
{
    enum {
        ID_BN_EXEC = 1111
    };
public:
    UMyWindow()
        : UBaseWindow(NULL, GetModuleHandle(NULL))
    {
        this->setTitle(_T("UShell Demo"));
        this->setPos(0, 0, 800, 600);
    }

    /* virtual */  BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc)
    {
        uwc.setBKBrush((HBRUSH)::GetStockObject(GRAY_BRUSH));
        return TRUE;
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_HELP);

        _bnExec = new UButton(this, ID_BN_EXEC);
        _bnExec->setPos(100, 100, 100, 50);
        _bnExec->create();
        _bnExec->setWindowText(_T("notepad"));

        return UBaseWindow::onCreate();
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }

    /* virtual */ BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case ID_BN_EXEC:
            return onBnExec();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }
private:
    AUI::UButtonP _bnExec;

private:
    BOOL onBnExec()
    {
        UShell::execute(NULL, "notepad");
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
