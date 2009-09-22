#include "resource.h"

#include <windows.h>
#include <tchar.h>


#include "udialogx.h"
#include "udlgapp.h"
#include "uedit.h"
#include "ubutton.h"
#include "umsg.h"

#include "uclipboard.h"

using huys::UDialogBox;

//
class MyDialog : public UDialogBox
{
public:
    MyDialog(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    ~MyDialog()
    {
        CHECK_PTR(m_pEditOrigin);
        CHECK_PTR(m_pEditClip);
        CHECK_PTR(m_pButton);
    }

    enum {
        IDC_MBN = 1333,
        IDC_EDIT_ORIGIN = 1444,
        IDC_EDIT_CLIP = 1445
    };

    BOOL onInit()
    {
        RECT rc = { 20, 20, 150, 220 };

        m_pEditOrigin = new UEdit(m_hDlg, IDC_EDIT_ORIGIN, m_hInst);
        m_pEditOrigin->create();
        m_pEditOrigin->setPosition(&rc);

        rc.left += 150;
        rc.right += 150;
        rc.top += 20;
        rc.bottom -= 50;
        m_pButton = new UButton(m_hDlg, IDC_MBN, m_hInst);
        m_pButton->create();
        m_pButton->setPosition(&rc);
        m_pButton->setWindowText(_T("Copy&&Paste"));

        rc.left = 320;
        rc.right = rc.left + 150;
        rc.top = 20;
        rc.bottom = 220;
        m_pEditClip = new UEdit(m_hDlg, IDC_EDIT_CLIP, m_hInst);
        m_pEditClip->create();
        m_pEditClip->setPosition(&rc);

    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
            case IDC_MBN:
            {
                //showMsg(_T("You Press me!"));
                TCHAR buf[256];
                m_pEditOrigin->getText(buf);
                //showMsg(buf);
                UClipBoard::copyToClipborad(buf);

                //
                TCHAR buf2[256];
                UClipBoard::getFromClipboard(buf2);
                //showMsg(buf2);
                m_pEditClip->setText(buf2);
            }
        }

        return FALSE;
    }
protected:
private:
    UEdit *m_pEditOrigin;
    UEdit *m_pEditClip;
    UButton *m_pButton;
};


UDLGAPP_T(MyDialog, IDD_TEST);

