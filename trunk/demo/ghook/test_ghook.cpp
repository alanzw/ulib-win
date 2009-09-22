#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <stdio.h>

#include "udialogx.h"
#include "umsg.h"
#include "udlgapp.h"
#include "uhook.h"

using huys::UDialogBox;

HMODULE g_hModule = NULL;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    ~UDialogExt()
    {
    }

    BOOL onInit()
    {
        SendMessage(GetDlgItem(m_hDlg, IDC_RADIO_UNHOOK), (UINT) BM_SETCHECK,  (WPARAM)BST_CHECKED, 0);
        return TRUE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        //::PostMessage(m_hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, lParam);
        //::PostMessage(m_hDlg, UWM_MOUSEMOVE, 0, 0);
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_RADIO_HOOK:
            return onRaHook();
        case IDC_RADIO_UNHOOK:
            return onRaUnhook();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {

        if (message == UWM_MOUSEMOVE)
        {
            ::SetWindowText(m_hDlg, "hooked!");
        }

        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        return result;

    }

private:

    BOOL onRaHook()
    {
        setMyHook(m_hDlg);
        //::PostMessage(m_hDlg, UWM_MOUSEMOVE, 0, 0);
        setValue(33);
        return TRUE;
    }

    BOOL onRaUnhook()
    {
        char str[256];
        clearMyHook();
        sprintf(str, "%d", getValue());
        ::SendMessage(GetDlgItem(m_hDlg, IDC_EDIT_INFO), WM_SETTEXT, (WPARAM)256, (LPARAM)str);
        ::SetWindowText(m_hDlg, "hook");
        return TRUE;
    }
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);
