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

#include "ustatic.h"

#include "adt/uautoptr.h"

#include "aui/aui_label.h"

#include "uctrlbar.h"


using huys::UDialogBox;

class UDialogCtrlBar : public UDialogBox
{
public:
    UDialogCtrlBar(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        this->setDlgIconBig(IDI_CTRLBAR);

        m_pCtrlBar = new UCtrlBar(m_hDlg);

        m_pCtrlBar->setPos(50, 50, 600, 100);      

        m_pCtrlBar->create();

        //UStatic * _label = new AUI::UTransLabel(m_pCtrlBar,_T("hello"));
        //_label->setPos(0, 0, 100, 50);

        m_pCtrlBar->addCtrl(new AUI::UTransLabel(m_pCtrlBar,_T("hello")));

        m_pCtrlBar->createControls();
        
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
    huys::ADT::UAutoPtr<UCtrlBar> m_pCtrlBar;

    //AUI::UTransLabelP m_label;
};


UDLGAPP_T(UDialogCtrlBar, IDD_CTRLBAR);

