#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"

#include "adt/uautoptr.h"

#include <ocidl.h>
#include <olectl.h>
#include <shlwapi.h>

#include "upicture.h"

#include "ContactListBox.h"

using huys::UDialogBox;

class UDialogOICQ : public UDialogBox
{
    enum {
        ID_LB_CONTACT = 1234
    };
public:
    UDialogOICQ(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    virtual BOOL onInit()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);

        setDlgIconBig(::LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_OICQ)));

        m_pLBContact = new UContactListBox(m_hDlg, ID_LB_CONTACT, m_hInst);
        m_pLBContact->setPos(20, 130, 300, 400);
        m_pLBContact->create();

        pic[0].load("1.gif");
        pic[1].load("2.gif");
        pic[2].load("3.gif");

        m_pLBContact->addItem("pencil", &pic[0]);
        m_pLBContact->addItem("crayon", &pic[1]);
        m_pLBContact->addItem("marker", &pic[2]);

        m_pLBContact->update();

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

    virtual void onDraw(HDC hdc)
    {
        RECT rc = {40, 40, 80, 80};
        pic[0].show(hdc, &rc);
        rc.left += 50;
        rc.right += 50;
        pic[1].show(hdc, &rc);
        rc.left += 50;
        rc.right += 50;
        pic[2].show(hdc, &rc);
        
        rc.top = 580;
        rc.bottom = rc.top + 40;
        pic[2].show(hdc, &rc);
        rc.left -= 50;
        rc.right -= 50;
        pic[1].show(hdc, &rc);
        rc.left -= 50;
        rc.right -= 50;
        pic[0].show(hdc, &rc);
    }
private:
    huys::ADT::UAutoPtr<UContactListBox> m_pLBContact;

    UPicture pic[3];
};

UDLGAPP_T(UDialogOICQ, IDD_OICQ);

