#ifndef U_AUI_LABEL_H
#define U_AUI_LABEL_H

#include <cassert>

#include "ustatic.h"
#include "ubitmap.h"
#include "udc.h"
#include "adt/uautoptr.h"
#include "colors.h"
#include "adt/ustring.h"

namespace AUI
{

class UTransLabel : public UStatic
{
public:
    UTransLabel(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        //m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
        m_clrText = huys::black;
    }

    UTransLabel(UBaseWindow *pWndParent, LPCTSTR lpText = _T(""))
    : UStatic(pWndParent, lpText)
    {
        m_dwStyles &= ~SS_SIMPLE;
        //m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
        m_clrText = huys::black;
    }

    UTransLabel(UBaseWindow *pWndParent, UINT nResource)
    : UStatic(pWndParent, nResource)
    {
        m_dwStyles &= ~SS_SIMPLE;
        //m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
        m_clrText = huys::black;
    }

    /* virtual */ BOOL create()
    {
        return UStatic::create() && this->subclassProc();
    }

    BOOL onMessage( UINT message, WPARAM wParam, LPARAM lParam )
    {
        BOOL bRet = UStatic::onMessage(message, wParam, lParam);

        if (WM_SETTEXT == message)
        {
            onSetText();
        }

        return bRet;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, m_clrText);
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }

    void setTextColor(huys::Color clr)
    { m_clrText = clr; }
private:
    void onSetText()
    {
        RECT rc;
        ::GetWindowRect(m_hSelf, &rc);
        ::ScreenToClient(m_hParent,(LPPOINT) &rc);
        //::ScreenToClient(m_hParent,(LPPOINT) (&rc+1));
        ::InvalidateRect(m_hParent, &rc, TRUE);
        ::UpdateWindow(m_hParent);
    }
private:
    huys::Color m_clrText;
};


class UBitmapLabel : public UStatic
{
public:
    UBitmapLabel(HWND hParent, UINT nBitmap)
    : UStatic(hParent, -1, ::GetModuleHandle(NULL))
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_NOTIFY;
        m_ubm.loadFromResource(nBitmap, m_hInstance);
        setText(NULL);
    }

    virtual BOOL create()
    {
        return  UStatic::create() && this->subclassProc();
    }

    void size2content()
    {
        assert(!m_ubm.isNull());
        ::SetWindowPos(m_hSelf, NULL, -1, -1, m_ubm.getWidth(), m_ubm.getHeight(),
            SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE);
    }

    BOOL onPaint()
    {
    //!! BeginPaint will eat previous control text drawing or other actions
        UPaintDC dc(m_hSelf);
        onDraw(dc);
        return FALSE;
    }
//private:
protected:
    virtual void onDraw(HDC hdc)
    {
        RECT rc;
        this->getClientRect(&rc);

        m_ubm.showStretch(hdc, rc);
    }

private:
    UBitmap m_ubm;
};

class ULinkLabel : public UStatic
{
    enum ULL_State {
        ULLS_HOVER = 0,
        ULLS_VISITED,
        ULLS_NORMAL
    };
public:
    ULinkLabel(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles |= SS_NOTIFY;

        _initialize();
    }

    ULinkLabel(UBaseWindow *pWndParent, LPCTSTR lpText)
        : UStatic(pWndParent, lpText)
    {
        m_dwStyles |= SS_NOTIFY;

        _initialize();
    }

    /* virtual */ BOOL create()
    {
        return UStatic::create() && this->subclassProc();
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        USmartDC dc((HDC)wParam);

        switch (_state)
        {
        case ULLS_HOVER:
            dc.setTextColor(_clrHover);
            break;
        case ULLS_VISITED:
            dc.setTextColor(_clrVisited);
            break;
        default:
            dc.setTextColor(_clrLink);
        }

        dc.setBKMode(TRANSPARENT);

        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }

    virtual BOOL onLButtonDown(WPARAM,LPARAM)
    {
        gotoUrl();
        return FALSE;
    }

    virtual BOOL onLButtonUp(WPARAM,LPARAM)
    {

        return TRUE;
    }

    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        LONG x = LOWORD(lParam);
        LONG y = HIWORD(lParam);

        if (_state == ULLS_HOVER)
        {
            huys::URectL rect;
            this->getClientRect(rect);

            if (!rect.PtrInRectEx(x, y))
            {
                _state = ULLS_NORMAL;
                ::ReleaseCapture();
                this->invalidate();
                return FALSE;
            }
            _state = ULLS_NORMAL;
        }

        if (_state == ULLS_NORMAL)
        {
            _state = ULLS_HOVER;
            this->invalidate();
            ::SetCapture(m_hSelf);
        }

        return TRUE;
    }
public:
    void setUrl(const char * url)
    {
        _url = url;
    }
private:
    huys::Color _clrLink;
    huys::Color _clrVisited;
    huys::Color _clrHover;

    ULL_State _state;

    huys::ADT::UStringAnsi _url;
private:
    void _initialize()
    {
        _clrLink = huys::blue;
        _clrHover = huys::xpblue;
        _clrVisited = huys::darkmagenta;

        _state = ULLS_NORMAL;
    }

    void gotoUrl()
    {
        ::ShellExecute(NULL, _T("open"), _url, NULL, NULL, SW_SHOWNORMAL);
    }
};

typedef huys::ADT::UAutoPtr<UTransLabel> UTransLabelP;
typedef huys::ADT::UAutoPtr<UBitmapLabel> UBitmapLabelP;
typedef huys::ADT::UAutoPtr<ULinkLabel> ULinkLabelP;

}; // namespace AUI

#endif // U_AUI_LABEL_H
