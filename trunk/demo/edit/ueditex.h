/*
 * =====================================================================================
 *
 *       Filename:  ueditex.h
 *
 *    Description:  Class UEditEx
 *
 *        Version:  1.0
 *        Created:  2009/9/26 12:20:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_EDIT_EX_H
#define U_EDIT_EX_H

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
        ::Rectangle(hdc, 0, 0, rc.right - rc.left + 1, rc.bottom - rc.top + 1);
        ::SetTextColor(hdc, huys::black);
        //::TextOut(hdc, 2, 2, str, strlen(str));
        ::DrawText(hdc, str, strlen(str), &rc, DT_EDITCONTROL);
        ::SelectObject(hdc, hOldBrush);
        ::SelectObject(hdc, hOldPen);
    }
private:
    HBRUSH m_hbrush;
    huys::Color m_clr;
};

#endif // U_EDIT_EX_H

