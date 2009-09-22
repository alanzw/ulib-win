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


#ifndef U_MYDIALOG_SYSADM_H
#define U_MYDIALOG_SYSADM_H

#include "udialogx.h"

#include "umsg.h"
#include "ubutton.h"

using huys::UDialogBox;

class UDialogSysAdm : public UDialogBox
{
    enum {
        IDC_BN_CHANGE_WALLPAPER = 1400
    };
public:
    UDialogSysAdm(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    ~UDialogSysAdm()
    {
        CHECK_PTR(m_pBnWallpaper);
    }

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
    UPushButton *m_pBnWallpaper;

private:
    BOOL onBnChangeWallpaper()
    {
        showMsg(_T("Wallpaper"), _T("info"), m_hDlg);
        return FALSE;
    }
};

#endif // U_MYDIALOG_SYSADM_H

