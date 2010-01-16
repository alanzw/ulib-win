#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "colors.h"
#include "udialogx.h"
#include "udlgapp.h"

#include "ContactListBox.h"

#include <ocidl.h>
#include <olectl.h>
#include <shlwapi.h>

#include "upicture.h"

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

        m_pLBContact = new UContactListBox(m_hDlg, ID_LB_CONTACT, m_hInst);
        m_pLBContact->setPos(20,130,300,400);
        m_pLBContact->create();

        pic.load("1.gif");
        
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
        pic.show(hdc, &rc);
	}
private:
    UContactListBox *m_pLBContact;
    
    UPicture pic;
};

UDLGAPP_T(UDialogOICQ, IDD_OICQ);

