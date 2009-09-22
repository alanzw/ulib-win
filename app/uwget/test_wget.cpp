#include "resource.h"

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>
#include <stdio.h>

#include "udialogx.h"
#include "uprocess.h"
#include "umsg.h"
#include "udlgapp.h"
#include "ucombobox.h"
#include "uwget.h"

using huys::UDialogBox;

class MyDialog : public UDialogBox
{
public:
    typedef UDialogBox Super;
    enum {
        IDD = IDD_DIALOG_APP
    };
public:
    MyDialog()
    : UDialogBox(::GetModuleHandle(NULL), IDD),
      m_pc(0),
      m_bRun(false)
    {
    }

    ~MyDialog()
    {
    }

    BOOL onInit()
    {
        //
        if (!m_wivk.isExeFileOk())
        {
            showMsg("wget is not available.\r\nPlease get the application 'wget.exe' First.");
            return FALSE;
        }

        HWND hCombo = GetDlgItem(m_hDlg, IDC_COMBO_PROXY);
        ComboBox_AddString(hCombo, "proxy_office");
        ComboBox_SetCurSel(hCombo, 0);
        ComboBox_AddString(hCombo, "proxy_home");

        HICON hicon = (HICON)LoadImage(m_hInst, // small class icon
            MAKEINTRESOURCE(IDI_SMALL),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON)*2,
            GetSystemMetrics(SM_CYSMICON)*2,
            LR_DEFAULTCOLOR);
        SendDlgItemMessage(m_hDlg, IDC_BUTTON_GO, (UINT)BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hicon);
        SendMessage(m_hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hicon);

        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BUTTON_GO:
            return this->onBnGo();
        case IDC_CHECKBOX_RECURSIVE:
            return this->onBnChkRecusive();
        case IDC_CHECKBOX_PROXY:
            return this->onBnChkProxy();
        case IDCANCEL :
            m_pc->deleteContent();
            return Super::onCancel() ;
        case IDM_SAVE:
            showMsg("Save.");
            return FALSE;
        case IDM_ABOUT:
            showMsg("About.");
            return FALSE;
        case IDOK:
            return this->onOK();
        default:
            return Super::onCommand(wParam, lParam);
        }
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message==UPM_LINE)
        {
            TCHAR *lpszText = (TCHAR *)wParam;
            return this->sendChildMsg(IDC_EDIT_COMMAND, WM_SETTEXT, 0, (LPARAM)lpszText);
        }

        return result;
    }

private:
    WgetInvoker m_wivk;
    UPipedProcess *m_pc;

    bool m_bRun;
private:
    BOOL onBnGo()
    {
        //showMsg("Not implemented yet.");
        if (m_wivk.isExeFileOk())
        {
            //showMsg("OK");
            if (m_pc && !m_bRun)
            {

                if (!m_pc->run())
                {
                    showMsg("Error start child!");
                    return FALSE;
                }
                m_bRun = true;
            }
        } else {
            showMsg("Please get the application 'wget.exe' First.");
        }
        return TRUE;
    }

    BOOL onOK()
    {
        char szItemName[80];
        char szProxy[40];
        char szParameter[256];
        char szOutput[1024];

        if (!GetDlgItemText(m_hDlg, IDC_EDIT_URL, szItemName, 80))
        {
            //*szItemName=0;
            showError("URL is blank,");
            return FALSE;
        }
        //showMsg(szItemName);
        //SetDlgItemText(hDlg, IDC_COMBO1, "cc");
        ComboBox_GetText(GetDlgItem(m_hDlg, IDC_COMBO_PROXY), szProxy, 40);
        //GetWindowText(GetDlgItem(hDlg, IDC_COMBO1), szProxy, 40);
        //ComboBox_SetText(hCombo, "xx");
        //GetDlgItemText(hDlg, IDC_COMBO1, szProxy, 40);

        if ( IsDlgButtonChecked(m_hDlg, IDC_CHECKBOX_CONTINUE) )
        {
            strcpy(szParameter, " -c ");
        }
        if ( IsDlgButtonChecked(m_hDlg, IDC_CHECKBOX_RECURSIVE) )
        {
            strcat(szParameter, " -r ");
        }

        //wsprintf(szOutput, "@set http_proxy=http://huys.3322.org:9928\r\n@set ftp_proxy=http://huys.3322.org:9928\r\n\r\nwget --proxy=on %s%s\r\n", szParameter, szItemName);
        wsprintf(szOutput, "wget %s", szItemName);
        SetDlgItemText(m_hDlg, IDC_EDIT_COMMAND, szOutput);
        //MessageBox(hDlg, szProxy, "info", MB_OK|MB_ICONINFORMATION);
        if (!m_pc && strlen(szOutput)>0)
        {
            m_pc = new UPipedProcess(szOutput, m_hDlg, (UINT)::GetCurrentProcessId());
        }

        return TRUE;
    }

    BOOL onBnChkRecusive()
    {
        if (IsDlgButtonChecked(m_hDlg, IDC_CHECKBOX_RECURSIVE))
        {
            m_wivk.setRecursive(true);
        }
        else
        {
            m_wivk.setRecursive(false);
        }

        return FALSE;
    }

    BOOL onBnChkProxy()
    {
        if (IsDlgButtonChecked(m_hDlg, IDC_CHECKBOX_PROXY))
        {
            ::EnableWindow(::GetDlgItem(m_hDlg, IDC_COMBO_PROXY), TRUE);
            m_wivk.setProxyUsed(true);
        }
        else
        {
            ::EnableWindow(::GetDlgItem(m_hDlg, IDC_COMBO_PROXY), FALSE);
            m_wivk.setProxyUsed(false);
        }

        return FALSE;
    }
};

UDLGAPP_T0(MyDialog);
