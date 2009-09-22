#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ubutton.h"
#include "umci.h"

class UMyWindow : public UBaseWindow
{
    enum {
        IDC_BN_PLAYMIDI = 122
    };
public:
    UMyWindow(HINSTANCE hInst)
    : UBaseWindow(NULL, hInst)
    {
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        UButton ubn(*this, IDC_BN_PLAYMIDI, getInstance());
        ubn.create();

        RECT rc = { 100, 100, 200, 200};
        ubn.setPosition(&rc);

        ubn.setWindowText(_T("PlayMidi"));

        return UBaseWindow::onCreate();
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_PLAYMIDI:
            return onBnPlayMidi();
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    BOOL onDestroy()
    {
        return UBaseWindow::onDestroy();
    }

    void onDraw(HDC hdc)
    {

    }

    BOOL onMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        if (MM_MCINOTIFY == uMessage)
        {
            return onPlayOver();
        }
        return UBaseWindow::onMessage(uMessage, wParam, lParam);
    }
private:
    BOOL onBnPlayMidi()
    {
        // UMCI::playMIDIFile(*this, "sample.mid");
        UMCI::playMIDIFile(*this, "sample2.mid");
        return FALSE;
    }

    BOOL onPlayOver()
    {
        this->showMsg(_T("Over!"));
    }
};


class UMyApp : public UWinApp
{
public:
    bool init( HINSTANCE hInst /*= NULL*/ )
    {
        m_pMainWindow = new UMyWindow(hInst);
        m_pMainWindow->create();

        return true;
    }
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UMyApp app;

    app.init(hInstance);

    return app.run();
}

