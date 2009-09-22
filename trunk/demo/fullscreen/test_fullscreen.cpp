#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "ugldialog.h"
#include "udlgapp.h"
#include "usys.h"
#include "ubutton.h"

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
    enum {
        ID_BN_GOFULL = 1024
    };
public:
    MyDialog(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {

    }

    virtual BOOL onInit()
    {
        m_pBnGo = new UButton(m_hDlg, ID_BN_GOFULL, m_hInst);
        m_pBnGo->create();
        m_pBnGo->setWindowText(_T("Go!"));

        RECT rc = { 100, 100, 200, 200 };
        m_pBnGo->setPosition(&rc);
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case ID_BN_GOFULL:
            return onBnGoFull();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    BOOL onBnGoFull()
    {
        goFullScreen();
        return FALSE;
    }

    void goWindowed()
    {
        ChangeDisplaySettings(NULL, 0);
    }


    void goFullScreen()
    {
        USystem usys;

        LONG width = usys.getScreenWidth();
        LONG height = usys.getScreenHeight();

        DEVMODE dmScreenSettings;
        ::ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // setting display mode failed, switch to windowed
            ::MessageBox(NULL, "Display mode failed", NULL, MB_OK);
        }

        ::SetWindowText(m_hDlg, "");
        //this->modifyStyles(WS_POPUP);
        DWORD dwOldStyle = ::GetWindowLong(m_hDlg, GWL_STYLE);
        DWORD dwNewStyle = dwOldStyle^WS_CAPTION;
        ::SetWindowLong(m_hDlg, GWL_STYLE, dwNewStyle);
        this->modifyExStyles(WS_EX_TOPMOST);
        ::MoveWindow(m_hDlg, 0, 0, width, height, TRUE);
    }
private:
    UButton *m_pBnGo;
};

UDLGAPP_T(MyDialog, IDD_TEST);
