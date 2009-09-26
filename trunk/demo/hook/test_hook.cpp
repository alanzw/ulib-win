#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "umsg.h"

using huys::UDialogBox;

HHOOK hhookTest;
HWND g_hwnd = NULL;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        return 1;
    }
    //showMsg("Hook!");

    char *str= new char[256];
    PMOUSEHOOKSTRUCT pmh = (PMOUSEHOOKSTRUCT)lParam;

    sprintf(str, "Mouse: x=%d, y=%d", pmh->pt.x, pmh->pt.y);

    ::SendMessage(GetDlgItem(g_hwnd, IDC_EDIT_INFO),
        WM_SETTEXT, (WPARAM)256, (LPARAM)str);
    delete[] str;

    CallNextHookEx(hhookTest, nCode, wParam, lParam);
    return 0;

}

void startHook()
{
    hhookTest = SetWindowsHookEx(WH_MOUSE, MouseProc, ::GetModuleHandle(NULL), 0);
}

void stopHook()
{
    if (hhookTest)
    {
        UnhookWindowsHookEx(hhookTest);
    }
    hhookTest = NULL;
}


class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    BOOL onInit()
    {
        this->sendChildMsg(IDC_RADIO_UNHOOK, (UINT)BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
        g_hwnd = m_hDlg;
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_RADIO_HOOK:
            startHook();
            //showMsg("Start Hook!");
            return TRUE;
        case IDC_RADIO_UNHOOK:
            stopHook();
            //showMsg("Stop HOOK!");
            return this->sendChildMsg(IDC_EDIT_INFO, WM_SETTEXT, (WPARAM)256, (LPARAM)"NULL!");
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);
