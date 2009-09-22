#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialog.h"

UDialogBox::UDialogBox(HINSTANCE hInst, UINT nID, DLGPROC lpDialogFunc, HWND hParent)
{
    m_hInst = hInst;
    m_nDialogID = nID;
    m_lpDialogFunc = lpDialogFunc;
    m_hParent = hParent;
}

UDialogBox::~UDialogBox()
{
    // EndDialog(m_hDlg, 0);
}

BOOL UDialogBox::create()
{
    //DialogBox (m_hInst, MAKEINTRESOURCE(m_nDialogID), NULL, m_lpDialogFunc);

    m_hDlg = CreateDialog(m_hInst, MAKEINTRESOURCE(m_nDialogID), m_hParent, m_lpDialogFunc);

#if (WINVER >= 0x0500)
    AnimateWindow(m_hDlg, 1000, AW_SLIDE|AW_HOR_POSITIVE);
    RECT rc;
    GetClientRect(m_hDlg, &rc);
    ::InvalidateRect(m_hDlg, &rc, FALSE);
#else
    ShowWindow(m_hDlg, SW_SHOW);
#endif

    BOOL bRet;
    MSG msg;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // Handle the error and possibly exit
        }
        else if (!IsWindow(m_hDlg) || !IsDialogMessage(m_hDlg, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return TRUE;
}

BOOL UDialogBox::hide()
{
    return ShowWindow(m_hDlg, SW_HIDE);
}

BOOL UDialogBox::show()
{
    return ShowWindow(m_hDlg, SW_SHOW);
}

HWND UDialogBox::getHWND()
{
    return m_hDlg;
}

BOOL UDialogBox::onCancel(HWND hDlg)
{
    //EndDialog (hDlg, 0) ;
    BOOL bExitProccess = FALSE;

    if (GetParent(hDlg)==NULL || GetParent(hDlg)==HWND_DESKTOP)
    {
        bExitProccess = TRUE;
    }

#if (WINVER >= 0x0500)
    AnimateWindow(hDlg, 1000, AW_BLEND|AW_HIDE);
#endif
    DestroyWindow(hDlg);
    //hDlg = NULL;

    if (bExitProccess)
    {
        ::PostQuitMessage (0);
    }

    return TRUE;
}

BOOL CALLBACK UDialogBox::DefaultDlgProc(HWND hDlg, UINT message,
                            WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG :
        return TRUE;
    case WM_COMMAND :
        switch (LOWORD (wParam))
        {
        case IDOK :
        case IDCANCEL :
            return UDialogBox::onCancel(hDlg) ;
        }
    }
    //return ::DefWindowProc(hDlg, message, wParam, lParam);
    return FALSE;
}

