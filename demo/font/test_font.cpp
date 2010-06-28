#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "ufont.h"
#include "ucommondialog.h"
#include "adt/uautoptr.h"
#include "udc.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_BN_GO = 1021
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        //setDlgIconBig(IDI_GOOGLE);
        setDlgIconSmall(IDI_GOOGLE);
        
        m_pBnGo = new UButton(m_hDlg, IDC_BN_GO, m_hInst);
        m_pBnGo->create();
        m_pBnGo->setWindowText(_T("GO"));

        

        RECT rc = { 200, 100, 380, 150};
        m_pBnGo->setPosition(&rc);

        m_font.setFontFaceName(_T("Times New Roman"));
        m_font.setFontHeight(15);
        m_font.create();

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BN_GO:
            return onBnGo();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    void onDraw(HDC hdc)
    {
        USmartDC dc(hdc);

        m_font.select(dc);

        dc.setBKMode(TRANSPARENT);
        dc.textOut(200, 200, "Cross River", 11);

        dc.setTextColor(huys::burlywood);
        dc.textOut(200, 250, "Cross River", 11);
    }

    BOOL onBnGo()
    {
        ::InvalidateRect(m_hDlg, NULL, TRUE);

        UCommonDialog::UFontDialog fdlg(m_hDlg);

        fdlg.choose();

        UFont font;

        font.useLogFont(fdlg.logFont());

        //font.createFont();

        //HDC hdc = GetDC(m_hDlg);
        UPrivateDC dc(m_hDlg);
        
        
        font.select(dc);

        // Display the text string.
        dc.setBKMode(TRANSPARENT);
        const TCHAR *text = _T("A Fox Crosses A River!");
        font.textOut(dc, 100, 50, text, lstrlen(text));

        font.restore();

        //ReleaseDC(m_hDlg, hdc);
        return FALSE;
    }

protected:
private:
    huys::ADT::UAutoPtr<UButton> m_pBnGo;
    UFont m_font;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

