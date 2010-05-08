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

#include <shlobj.h>

#include "udialogx.h"

#include "umsg.h"
#include "ubutton.h"
#include "adt/uautoptr.h"
#include "com/ucom.h"



using huys::UDialogBox;

class UDialogSysAdm : public UDialogBox
{
    enum {
        IDC_BN_CHANGE_WALLPAPER = 1400,
        IDC_BN_SHUTDOWN         = 1401,
        IDC_BN_SHOWDESKTOP      = 1402,
        IDC_BN_CONTROLPANNEL    = 1403
    };
public:
    UDialogSysAdm(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          _shellobj(CLSID_Shell)
    {}

    BOOL onInit()
    {

        m_pBnWallpaper = new UPushButton(m_hDlg, IDC_BN_CHANGE_WALLPAPER, m_hInst);
        m_pBnWallpaper->setPos(20, 20, 200, 50);
        m_pBnWallpaper->create();
        m_pBnWallpaper->setWindowText(_T("Change Wallpaper"));

        m_pBnShutdown = new UPushButton(m_hDlg, IDC_BN_SHUTDOWN, m_hInst);
        m_pBnShutdown->setPos(300, 20, 200, 50);
        m_pBnShutdown->create();
        m_pBnShutdown->setWindowText(_T("Shutdown"));

        m_pBnShowDesktop = new UPushButton(m_hDlg, IDC_BN_SHOWDESKTOP, m_hInst);
        m_pBnShowDesktop->setPos(20, 100, 200, 50);
        m_pBnShowDesktop->create();
        m_pBnShowDesktop->setWindowText(_T("Show Desktop"));

        m_pBnControlPannel = new UPushButton(m_hDlg, IDC_BN_CONTROLPANNEL, m_hInst);
        m_pBnControlPannel->setPos(300, 100, 200, 50);
        m_pBnControlPannel->create();
        m_pBnControlPannel->setWindowText(_T("Control Pannel"));

        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch ( LOWORD(wParam) )
        {
        case IDC_BN_CHANGE_WALLPAPER:
            return onBnChangeWallpaper();
        case IDC_BN_SHUTDOWN:
            return onBnShutdown();
        case IDC_BN_SHOWDESKTOP:
            return onBnShowDesktop();
        case IDC_BN_CONTROLPANNEL:
            return onBnControlPannel();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    huys::ADT::UAutoPtr<UPushButton> m_pBnWallpaper;
    huys::ADT::UAutoPtr<UPushButton> m_pBnShutdown;
    huys::ADT::UAutoPtr<UPushButton> m_pBnShowDesktop;
    huys::ADT::UAutoPtr<UPushButton> m_pBnControlPannel;

    huys::UCOM::UDispObject<IShellDispatch> _shellobj;
private:
    BOOL onBnChangeWallpaper()
    {
        //showMsg(_T("Wallpaper"), _T("info"), m_hDlg);
        _shellobj->FileRun();
        return FALSE;
    }

    BOOL onBnShutdown()
    {
        _shellobj->ShutdownWindows();
        return FALSE;
    }

    BOOL onBnShowDesktop()
    {
//         HRESULT hr;   
//         IShellDispatch *pShellDisp = NULL;   
//         CoInitialize(NULL); 
//         hr = CoCreateInstance(CLSID_Shell,NULL,CLSCTX_SERVER,IID_IDispatch,(LPVOID *) &pShellDisp   );   
//         if(FAILED(hr))   
//         {   
//             CoUninitialize();   
//             return FALSE;   
//         }   
//         pShellDisp->MinimizeAll();   
//         pShellDisp->Release();   
//         ::CoUninitialize();
        static bool bShowDesktop = false;
        if (!bShowDesktop)
        {
            _shellobj->MinimizeAll();
            m_pBnShowDesktop->setWindowText(_T("Restore"));
        }
        else
        {
            _shellobj->UndoMinimizeALL();
            m_pBnShowDesktop->setWindowText(_T("Show Desktop"));
        }

        bShowDesktop = !bShowDesktop;
        return FALSE;
    }

    BOOL onBnControlPannel()
    {
        _shellobj->ControlPanelItem(L"inetcpl.cpl");
        return FALSE;
    }
};

#endif // U_MYDIALOG_SYSADM_H

