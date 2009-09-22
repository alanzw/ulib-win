#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "usocket.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "colors.h"

using huys::UDialogBox;

class UDialogNetChat : public UDialogBox
{
public:
    UDialogNetChat(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);
        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        HBRUSH hBrushBK = ::CreateSolidBrush(huys::aliceblue);
        switch (message)
        {
        case WM_CTLCOLORDLG:
            return (BOOL)hBrushBK;
        }
        return result;
    }
};

UDLGAPP_T(UDialogNetChat, IDD_NETCHAT);
