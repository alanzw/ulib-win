#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialog.h"
#include "umsg.h"

#include <stdio.h>

HINSTANCE g_hInst;
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
    hhookTest = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hInst, 0);
}

void stopHook()
{
    if (hhookTest)
    {
        UnhookWindowsHookEx(hhookTest);
    }
    hhookTest = NULL;
}


BOOL CALLBACK MyDlgProc(HWND hDlg, UINT message,
                        WPARAM wParam, LPARAM lParam)
{
    if (!g_hwnd)
    {
        g_hwnd = hDlg;
    }
    switch (message)
    {
    case WM_INITDIALOG :
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UNHOOK),
            (UINT) BM_SETCHECK,  (WPARAM)BST_CHECKED, 0);
        break;
    case WM_LBUTTONDOWN:
        PostMessage(hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        break;
    case WM_COMMAND :
        switch (LOWORD (wParam))
        {
        case IDC_RADIO_HOOK:
            startHook();
            //showMsg("Start Hook!");
            return TRUE;
        case IDC_RADIO_UNHOOK:
            stopHook();
            //showMsg("Stop HOOK!");
            ::SendMessage(GetDlgItem(g_hwnd, IDC_EDIT_INFO),
                WM_SETTEXT, (WPARAM)256, (LPARAM)"NULL!");
            return TRUE;
        }
    }
    return UDialogBox::DefaultDlgProc(hDlg, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    g_hInst = hInstance;

    UDialogBox dialog(hInstance, IDD_DIALOG1, MyDlgProc);

    dialog.create();

    //g_hwnd = dialog.getHWND();


    return 0;
}

