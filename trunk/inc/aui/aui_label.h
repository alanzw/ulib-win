#ifndef U_AUI_LABEL_H
#define U_AUI_LABEL_H

#include "ustatic.h"
#include "adt/uautoptr.h"

namespace AUI
{

class UTransLabel : public UStatic
{
public:
    UTransLabel(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UStatic(hParent, nResource, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
    }

    UTransLabel(UBaseWindow *pWndParent, LPCTSTR lpText)
    : UStatic(pWndParent, lpText)
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_CENTER | SS_CENTERIMAGE;
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
        return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
    }
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
};


typedef huys::ADT::UAutoPtr<UTransLabel> UTransLabelP; 

}; // namespace AUI

#endif // U_AUI_LABEL_H
