#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "ubutton.h"
#include "udlgapp.h"
#include "udc.h"
#include "ubrush.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_UBUTTON = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_pBtn = new UButton(m_hDlg, ID_UBUTTON, m_hInst);
        m_pBtn->setPos(300, 200, 100, 100);
        m_pBtn->create();
        m_pBtn->setWindowText(_T("Capture"));
        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case ID_UBUTTON:
            {
                return FALSE;
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual void onDraw(HDC hdc)
    {
        brush.getStockBrush(BLACK_BRUSH);

        USmartDC udc(hdc);

		huys::URectL rect(10, 10, 60, 110);
        udc.fillRect(rect, brush);

        udc.setBKColor(::GetSysColor(COLOR_BTNFACE));
        udc.textOut(rect.left() + 15, rect.bottom()+10, "1", 1);

        brush.getStockBrush(WHITE_BRUSH);

        rect.offset(100,0);
        udc.frameRect(rect, brush);
        //udc.invertRect(&rc);
        udc.textOut(rect.left() + 15, rect.bottom()+10, "2", 1);

		huys::URectL rectClient;
        ::GetClientRect(m_hDlg, rectClient);

        udc.setViewportOrg(rectClient.left(), rectClient.bottom());

        udc.setMapMode(MM_HIENGLISH);

        //SetViewportOrgEx(hdc, 100, 100, NULL);
        udc.setPenColor(huys::red);
        udc.setBrushColor(huys::aliceblue);

		huys::URectL  rc2(2500, 2100, 3000, 3100);
        //udc.fillRect(&rc2, hBrush);
        udc.rectangle(rc2);
        udc.textOut(rc2.left() + 150, rc2.top() - 100, "3", 1);

        udc.setPenColor(huys::aqua);
        udc.setBrushColor(huys::xpblue);

        rc2.offset(1000,0);
        udc.rectangle(rc2);
        //udc.invertRect(&rc2);
        udc.textOut(rc2.left() + 150, rc2.top() - 100, "4", 1);

    }
private:
    huys::ADT::UAutoPtr<UButton> m_pBtn;
	UBrush brush;
};

UDLGAPP_T(UDialogExt, IDD_TEST);
