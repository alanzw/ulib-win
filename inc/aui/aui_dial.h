#ifndef U_AUI_DIAL_H
#define U_AUI_DIAL_H

#include "ustatic.h"
#include "udc.h"
#include "adt/uautoptr.h"
#include "colors.h"

namespace AUI
{

class UDial : public UStatic
{
public:
    UDial(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UStatic(hParent, nResource, hInst)
    {
        init();
    }
    
    UDial(UBaseWindow *pWndParent, LPCTSTR lpText = _T(""))
    : UStatic(pWndParent, lpText)
    {
        init();
    }

    UDial(UBaseWindow *pWndParent, UINT nResource)
    : UStatic(pWndParent, nResource)
    {
        init();
    }
    
    /* virtual */ BOOL create()
    {
        return UStatic::create() && this->subclassProc();
    }
private:
    void init()
    {
        m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_NOTIFY;        
    }
    
};

}; // namespace AUI

#endif // U_AUI_DIAL_H
