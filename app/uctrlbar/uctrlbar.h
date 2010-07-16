/*
 * =====================================================================================
 *
 *       Filename:  usysadm.h
 *
 *    Description:  Class MyDialog
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:20:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_MYDIALOG_CTRLBAR_H
#define U_MYDIALOG_CTRLBAR_H

#include "ubasewindow.h"
#include "ucontrol.h"
#include "colors.h"
//#include "ubutton.h"
//#include "ustatic.h"
#include "udc.h"

#include "adt/uvector.h"

class UCtrlBar : public UBaseWindow
{
public:
    typedef huys::ADT::UVector<UControl *> UCtrls;

    UCtrlBar(UBaseWindow *pWndParent)
    : UBaseWindow(pWndParent)
    {   
        _initialize();
    }

    UCtrlBar(HWND hParent)
        : UBaseWindow(hParent)
    {
        _initialize();
    }

    ~UCtrlBar()
    {
        UCtrls::iterator it = m_ctrls.begin();
        for (; it!=m_ctrls.end(); ++it)
        {
            delete *it;
        }
    }

    BOOL createControls()
    {
        UCtrls::iterator it = m_ctrls.begin();
        for (; it!=m_ctrls.end(); ++it)
        {
            (*it)->setPos(10, 10, 100, 40);
            (*it)->create();
        }

        return TRUE;
    }

//     BOOL onEraseBkgnd(HDC hdc)
//     {
//         USmartDC dc(hdc);
//         
//         huys::URectL rect;
//         
//         this->getClientRect(rect);
//         HPEN hOldPen = (HPEN)dc.selectObj(::GetStockObject(NULL_PEN));
//         dc.setBrushColor(huys::burlywood);
//         dc.rectangle(rect); 
//         dc.selectObj(hOldPen);
// 
//         return TRUE;
//    

    int getCount() const
    {
        return m_nCount;
    }

    void addCtrl(UControl * ctrl)
    {
        m_ctrls.push_back(ctrl);
    }

    //virtual UControl * create(HWND hParent, UINT id, const TCHAR *lpTitle, LPRECT lpRect) = 0;
protected:
    UCtrls m_ctrls;
    int m_nCount;

private:
    void _initialize()
    {
        setMenu(0);
        setWndClassName(_T("HUYS_CTRLBAR_WINDOW_CLASS"));
        //setTitle(_T("DOCK"));
        
        addStyles(WS_CHILD);
        //setExStyles(WS_EX_TOOLWINDOW);
    }
};

#endif // U_MYDIALOG_CTRLS_H

