/*
 * =====================================================================================
 *
 *       Filename:  test_sysadm.cpp
 *
 *    Description:  A little kit for system admin
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:17:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "umsg.h"
#include "udlgapp.h"

#include "uctrlbar.h"


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


UDLGAPP_T(UDialogCtrlBar, IDD_CTRLBAR);

