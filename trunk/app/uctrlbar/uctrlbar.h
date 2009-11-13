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

#include "udialogx.h"
#include "umsg.h"
#include "ucontrol.h"
#include "colors.h"
//#include "ubutton.h"
//#include "ustatic.h"


//#include "adt/uvector.h"

class UCtrlBar : public UControl
{
public:
    //typedef huys::ADT::UVector<UControl *> UCtrls;

    UCtrlBar()
    {}

    virtual ~UCtrlBar()
    {
    }

    //virtual UControl * create(HWND hParent, UINT id, const TCHAR *lpTitle, LPRECT lpRect) = 0;
protected:
    //UCtrls m_ctrls;
};

using huys::UDialogBox;

class UDialogCtrlBar : public UDialogBox
{
public:
    UDialogCtrlBar(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    ~UDialogCtrlBar()
    {

    }

    BOOL onInit()
    {
        return TRUE;
    }

    BOOL onRButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch ( LOWORD(wParam) )
        {
        case 333:
            {
                ;
            }
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);
        HBRUSH hBrushBK = ::CreateSolidBrush(huys::xpblue);
        switch (message)
        {
        case WM_CTLCOLORDLG:
            return (BOOL)hBrushBK;
        }
        return result;
    }
private:
    UCtrlBar *m_pCtrlBar;
};

#endif // U_MYDIALOG_CTRLS_H

