#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ucommondialog.h"
#include "ubutton.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_BUTTON_COLOR = 1021,
        IDC_BUTTON_FILE  = 1022,
        IDC_BUTTON_FONT  = 1023,
        IDC_BUTTON_FIND  = 1024
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        m_pButtonColor = new UButton(m_hDlg, IDC_BUTTON_COLOR, m_hInst);
        m_pButtonColor->create();

        RECT rc = { 50, 20, 250, 70};
        m_pButtonColor->setPosition(&rc);
        m_pButtonColor->setWindowText(_T("Choose Color"));

        m_pButtonFile = new UButton(m_hDlg, IDC_BUTTON_FILE, m_hInst);
        m_pButtonFile->create();

        rc.top += 100;
        rc.bottom += 100;
        m_pButtonFile->setPosition(&rc);
        m_pButtonFile->setWindowText(_T("Choose File"));

        m_pButtonFont = new UButton(m_hDlg, IDC_BUTTON_FONT, m_hInst);
        m_pButtonFont->create();

        rc.top += 100;
        rc.bottom += 100;
        m_pButtonFont->setPosition(&rc);
        m_pButtonFont->setWindowText(_T("Choose Font"));

        rc.left += 250;
        rc.right += 250;
        m_pButtonFind = new UButton(m_hDlg, IDC_BUTTON_FIND, m_hInst);
        m_pButtonFind->setRect(&rc);
        m_pButtonFind->create();
        m_pButtonFind->setWindowText(_T("Find"));

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        return result;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BUTTON_COLOR:
            return onBnColor();
        case IDC_BUTTON_FILE:
            return onBnFile();
        case IDC_BUTTON_FONT:
            return onBnFont();
        case IDC_BUTTON_FIND:
            return onBnFind();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
protected:
    BOOL onBnColor()
    {
        UCommonDialog::UColorDialog colordlg(m_hDlg);
        return colordlg.choose();
    }
    BOOL onBnFile()
    {
        UCommonDialog::UFileDialog filedlg(m_hDlg);
        return filedlg.open();
    }

    BOOL onBnFont()
    {
        UCommonDialog::UFontDialog fontdlg(m_hDlg);
        return fontdlg.choose();
    }

    BOOL onBnFind()
    {
        UCommonDialog::UFindReplaceDialog dlg(m_hDlg);
        return dlg.go(_T("hello"));
    }
private:
    huys::ADT::UAutoPtr<UButton> m_pButtonColor;
    huys::ADT::UAutoPtr<UButton> m_pButtonFile;
    huys::ADT::UAutoPtr<UButton> m_pButtonFont;
    huys::ADT::UAutoPtr<UButton> m_pButtonFind;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

