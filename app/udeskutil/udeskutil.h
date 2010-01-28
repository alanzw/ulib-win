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


#ifndef U_MYDIALOG_DESKTOP_UTIL_H
#define U_MYDIALOG_DESKTOP_UTIL_H

#include "udialogx.h"

#include "umsg.h"
#include "ubutton.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogDeskUtil : public UDialogBox
{
    enum {
        IDC_BN_CHANGE_WALLPAPER = 1400
    };
public:
    UDialogDeskUtil(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_pBnWallpaper = new UPushButton(m_hDlg, IDC_BN_CHANGE_WALLPAPER, m_hInst);
        m_pBnWallpaper->setPos(20, 20, 200, 50);
        m_pBnWallpaper->create();
        m_pBnWallpaper->setWindowText(_T("Change Wallpaper"));

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch ( LOWORD(wParam) )
        {
        case IDC_BN_CHANGE_WALLPAPER:
            return onBnChangeWallpaper();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UPushButton> m_pBnWallpaper;

private:
    BOOL onBnChangeWallpaper()
    {
        //showMsg(_T("Wallpaper"), _T("info"), m_hDlg);
        HWND hProgman = ::FindWindow(_T("Progman"), _T("Program Manager"));

        HWND hShellDll_DefView = ::GetWindow(hProgman, GW_CHILD);

        HWND hWnd = ::GetWindow(hShellDll_DefView, GW_CHILD);

        //HWND hListHeader = ::GetWindow(hWnd, GW_CHILD);

        //::FindWindow(_T("SysListView32"), _T("FolderView"));

        if (NULL != hWnd)
        {
            //showMsg(_T("Get it!"), _T("info"), m_hDlg);
            return FALSE;
        }


        return TRUE;
    }
};

#endif // U_MYDIALOG_SYSADM_H

