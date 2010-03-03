#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubitmap.h"
#include "ulistbox.h"
#include "umsg.h"
#include "ustatic.h"

#include "ucommondialog.h"

#include "upicture.h"

#include "adt/uautoptr.h"
#include "adt/ustring.h"

using huys::UDialogBox;

class UDialogViewer : public UDialogBox
{
public:
    UDialogViewer(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID) {}

    BOOL onInit()
    {
        m_pBack = new UBitmap(IDB_BACKGROUND, m_hInst);

        addFilenames();

        return TRUE;
    }

    virtual BOOL onEraseBkgnd(HDC hdc)
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        m_pBack->showStretch(hdc, rc);
        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        if (IDC_LIST_IMAGES == LOWORD(wParam) && LBN_SELCHANGE == HIWORD(wParam))
        {
            return this->onSelChange();
        }

        if (IDC_BN_LOAD == LOWORD(wParam))
        {
            return this->onBnLoad();
        }

        return UDialogBox::onCommand(wParam, lParam);
    }
private:
    huys::ADT::UAutoPtr<UBitmap> m_pBack;
    UPicture pic;
    UBitmap bmp;

    BOOL onSelChange()
    {
        char buf[256] = {0};
        UListBox ulb;
        ulb.fromID(m_hDlg, IDC_LIST_IMAGES);
        ulb.getText(ulb.getCurSel(), buf);
        //showMsg(buf, "info", m_hDlg);

        showPicture(buf);

        return FALSE;
    }

    void showPicture(LPCTSTR sPath)
    {
        pic.load(sPath);
        HDC hdc = ::GetDC(m_hDlg);
        RECT rc;
        UStatic ustatic;
        ustatic.fromID(m_hDlg, IDC_STATIC_IMAGE);
        ustatic.getClientRect(&rc);
        ustatic.clientToDialog(m_hDlg, &rc);
        pic.show(hdc, &rc);
    }

    BOOL addFilenames()
    {
        UListBox ulb;
        ulb.fromID(m_hDlg, IDC_LIST_IMAGES);
        //ulb.addString("cc.bmp")
        //char dir[MAX_PATH+6] = "F:\\ulib-win\\app\\uviewer\\*";
        char cdir[MAX_PATH];
        char dirs[2*MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, cdir);
        //int length = strlen(dir);
        ZeroMemory(dirs, sizeof(dirs));
        //strcpy(dirs, cdir);
        //strcat(dirs, "\\*.bmp|*.jpg");
        wsprintf(dirs, "%s\\%s", cdir, "*.bmp");

        WIN32_FIND_DATA findData;
        HANDLE hFind = INVALID_HANDLE_VALUE;

        hFind = FindFirstFile(dirs, &findData);
        if ( INVALID_HANDLE_VALUE == hFind)
        {
            showMsg("xxx", "info", m_hDlg);
            return FALSE;
        }

        // Check if it is a directory
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //showMsg("xxx2", "info", m_hDlg);
            //return FALSE;
        }
        ulb.addString(findData.cFileName);
        while (0 != FindNextFile(hFind, &findData))
        {
            //printf("\t%s\n", findData.cFileName);
            ulb.addString(findData.cFileName);
        }

        FindClose(hFind);
        return TRUE;
    }

    BOOL onBnLoad()
    {
        UCommonDialog::UFileDialog filedlg(m_hDlg);
        filedlg.setFilter(_T("All\0*.*\0Bitmap\0*.bmp\0PNG\0*.png\0"));
        if (filedlg.open())
        {
            //UListBox ulb;
            //ulb.fromID(m_hDlg, IDC_LIST_IMAGES);
            //ulb.addString(filedlg.getName());
            showPicture(filedlg.getName());
        }

        return TRUE;
    }
};

UDLGAPP_T(UDialogViewer, IDD_VIEWER);

