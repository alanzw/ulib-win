#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>

#include "udialogx.h"
#include "uimagelist.h"
#include "udlgapp.h"

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
public:
    MyDialog(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        setDlgIconBig(IDI_APP);
        return TRUE;
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        UImageList uiml(1, 32, 32);
        uiml.addIcon(IDI_APP, m_hInst);
        uiml.drawIcon(0, m_hDlg, 20, 20);

        UImageList uiml2(IDB_TOOLBAR1, m_hInst);
        for (int i=0; i<uiml2.getIconNum(); i++)
        {
            uiml2.drawIcon(i, m_hDlg, 50+i*30, 50);
        }
    }
};

UDLGAPP_T(MyDialog, IDD_DIALOG1);

