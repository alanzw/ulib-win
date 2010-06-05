#ifndef U_AUI_LABEL_H
#define U_AUI_LABEL_H

#include <cassert>

#include "ustatic.h"
#include "ubitmap.h"
#include "udc.h"
#include "adt/uautoptr.h"
#include "colors.h"

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

    UTransLabel(UBaseWindow *pWndParent, LPCTSTR lpText)
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
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
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
    : UStatic(hParent, IDC_STATIC, ::GetModuleHandle(NULL))
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

typedef huys::ADT::UAutoPtr<UTransLabel> UTransLabelP;
typedef huys::ADT::UAutoPtr<UBitmapLabel> UBitmapLabelP;

}; // namespace AUI

#endif // U_AUI_LABEL_H
