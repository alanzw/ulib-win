#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "uedit.h"
#include "udlgapp.h"
#include "colors.h"
#include "ubutton.h"

class UEditEx : public UEdit
{
public:
    UEditEx(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UEdit(hParent, nResource, hInst)
    {
        m_hbrush = NULL;
        m_clr = huys::tomato;
    }

    // Redirect the Control Procedure
    BOOL create()
    {
        UEdit::create();
        this->subclassProc();
        return TRUE;
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        //if (wParam <'0' || wParam >'9')
        //   return 0;
        //char code[10];
        //wsprintf(code, "0x%x -- %c", wParam, wParam);
        if (wParam >= '0' && wParam <= '9' || wParam == VK_RETURN ) return 0;
         //   ::MessageBox(m_hSelf, "xx", code, MB_OK);
        return TRUE;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hSelf, &ps);

        char str[256];
        this->getText(str);

        drawit(str, hdc);

        EndPaint(m_hSelf, &ps);
        return FALSE;
    }

    virtual BOOL onSetFocus()
    {
        m_clr = huys::tomato;
        ::InvalidateRect(m_hSelf, NULL, TRUE);
        return FALSE;
    }

    virtual BOOL onKillFocus()
    {
        m_clr = huys::white;
        ::InvalidateRect(m_hSelf, NULL, TRUE);
        return FALSE;
    }
private:
    BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdcChild = (HDC)wParam;
        SetTextColor(hdcChild, huys::black);
        SetBkColor(hdcChild, huys::tomato);
        return (BOOL)m_hbrush;
    }

    void drawit(char *str, HDC &hdc)
    {
        if (!m_hbrush)
        {
            m_hbrush = ::CreateSolidBrush(m_clr);
        }
        else
        {
            ::DeleteObject(m_hbrush);
            m_hbrush = ::CreateSolidBrush(m_clr);
        }

        ::SetBkMode(hdc, OPAQUE);
        ::SetBkColor(hdc, m_clr);
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, m_hbrush);
        HPEN hOldPen = (HPEN)::SelectObject(hdc, ::GetStockObject(NULL_PEN));
        RECT rc;
        ::GetClientRect(m_hSelf, &rc);
        //::ScreenToClient(m_hSelf, (LPPOINT)&rc);
        //rc.left += 5;
        //rc.top += 5;
        ::Rectangle(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);
        ::SetTextColor(hdc, huys::black);
        //::TextOut(hdc, 2, 2, str, strlen(str));
        ::DrawText(hdc, str, strlen(str), &rc, DT_EDITCONTROL);
        ::SelectObject(hdc, hOldBrush);
        ::SelectObject(hdc, hOldPen);
    }
    HBRUSH m_hbrush;
    huys::Color m_clr;
};


using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_EDIT_UE = 3333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          m_pEditEx(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pEditEx);
    }

    virtual BOOL onInit()
    {
        m_pEditEx = new UEditEx(m_hDlg, IDC_EDIT_UE, m_hInst);
        m_pEditEx->setPos(50,95,400,180);
        m_pEditEx->setStyles( ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN |
                              ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL);
        m_pEditEx->create();
        m_pEditEx->setWindowText(_T("ÖÐ¹ú"));
        
        URadioButton ura;
        ura.fromID(m_hDlg, IDC_RADIO_NORMAL);
        ura.check();

        return TRUE;
    }
    
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_RADIO_TOPMOST:
            return this->setTopMost();
        case IDC_RADIO_NORMAL:
            return this->setNoTopMost();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

private:
    UEditEx *m_pEditEx;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

