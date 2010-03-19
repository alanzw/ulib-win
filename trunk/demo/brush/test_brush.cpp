#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "udialogx.h"
#include "ubutton.h"
#include "udlgapp.h"
#include "udc.h"

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
            //return CaptureAnImage(m_hDlg, _T("window.bmp"));
                return FALSE;
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual void onDraw(HDC hdc)
    {
        HBRUSH hBrush = (HBRUSH)::GetStockObject(BLACK_BRUSH);
        UDC udc;
        udc.attach(hdc);

        RECT rc = {10, 10, 60, 110};
        udc.fillRect(&rc, hBrush);

        udc.setBKColor(::GetSysColor(COLOR_BTNFACE));
        udc.textOut(rc.left + 15, rc.bottom+10, "1", 1);

        HBRUSH hBrushWhite = (HBRUSH)::GetStockObject(WHITE_BRUSH);

        rc.left += 100;
        rc.right += 100;
        udc.frameRect(&rc, hBrush);
        //udc.invertRect(&rc);
        udc.textOut(rc.left + 15, rc.bottom+10, "2", 1);

        RECT rcClient;
        ::GetClientRect(m_hDlg, &rcClient);

        udc.setViewportOrg(rcClient.left, rcClient.bottom);

        udc.setMapMode(MM_HIENGLISH);

        //SetViewportOrgEx(hdc, 100, 100, NULL);
        udc.setPenColor(huys::red);
        udc.setBrushColor(huys::aliceblue);

        RECT rc2 = {2500, 2100, 3000, 3100};
        //udc.fillRect(&rc2, hBrush);
        udc.rectangle(&rc2);
        udc.textOut(rc2.left + 150, rc2.top - 100, "3", 1);

        udc.setPenColor(huys::aqua);
        udc.setBrushColor(huys::xpblue);

        rc2.left += 1000;
        rc2.right += 1000;
        udc.rectangle(&rc2);
        //udc.invertRect(&rc2);
        udc.textOut(rc2.left + 150, rc2.top - 100, "4", 1);

        udc.dettach();
    }
private:
    huys::ADT::UAutoPtr<UButton> m_pBtn;
};

UDLGAPP_T(UDialogExt, IDD_TEST);
