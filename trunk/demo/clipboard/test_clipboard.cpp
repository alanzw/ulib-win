#include "resource.h"

#include <windows.h>
#include <tchar.h>


#include "udialogx.h"
#include "udlgapp.h"
#include "uedit.h"
#include "ubutton.h"
#include "umsg.h"
#include "adt/uautoptr.h"

#include "uclipboard.h"

using huys::UDialogBox;

//
class MyDialog : public UDialogBox
{
public:
    MyDialog(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    enum {
        IDC_MBN = 1333,
        IDC_EDIT_ORIGIN = 1444,
        IDC_EDIT_CLIP = 1445
    };

    BOOL onInit()
    {
        huys::URectL rect(20, 20, 150, 220);

        m_pEditOrigin = new UEdit(m_hDlg, IDC_EDIT_ORIGIN, m_hInst);
        m_pEditOrigin->create();
        m_pEditOrigin->setPosition(rect);

        rect.deflate(150, 20, -150, 50);
        m_pButton = new UButton(m_hDlg, IDC_MBN, m_hInst);
        m_pButton->create();
        m_pButton->setPosition(rect);
        m_pButton->setWindowText(_T("Copy&&Paste"));

        rect.set(320, 20, 150, 200);
        m_pEditClip = new UEdit(m_hDlg, IDC_EDIT_CLIP, m_hInst);
        m_pEditClip->create();
        m_pEditClip->setPosition(rect);

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
private:
    huys::ADT::UAutoPtr<UEdit> m_pEditOrigin;
    huys::ADT::UAutoPtr<UEdit> m_pEditClip;
    huys::ADT::UAutoPtr<UButton> m_pButton;
};


UDLGAPP_T(MyDialog, IDD_TEST);

