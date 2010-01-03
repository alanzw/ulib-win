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
        IDC_BN_PLAYMIDI = 122,
        IDC_BN_MUTE     = 123
    };
public:
    UMyWindow(HINSTANCE hInst=::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst)
    {
        setTitle(_T("mci"));
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        UButton ubn(*this, IDC_BN_PLAYMIDI, getInstance());
        ubn.create();

        RECT rc = { 100, 100, 200, 200};
        ubn.setPosition(&rc);

        ubn.setWindowText(_T("PlayMidi"));
        
        UButton ubnMute(this, IDC_BN_MUTE);
        ubnMute.setPos(400, 100, 200, 200);
        ubnMute.create();
        ubnMute.setWindowText(_T("Mute"));

        return UBaseWindow::onCreate();
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_PLAYMIDI:
            return onBnPlayMidi();
        case IDC_BN_MUTE:
            return onBnMute();
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
        return TRUE;
    }
    
    BOOL onBnMute()
    {
        //UMCI::openCDDriver(TRUE);
        mciSendString("Set cdaudio door open wait", NULL, 0, 0); 
        return TRUE;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UMyWindow);
    app.init(hInstance);
    return app.run();
}

