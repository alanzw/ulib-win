#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>

#include "udialogx.h"
#include "udlgapp.h"
#include "uanimation.h"

const UINT IDC_ANIME = 10001;

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pAnime(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pAnime);
    }

    virtual BOOL onInit()
    {
        m_pAnime = new UAnimation(m_hDlg, IDC_ANIME, m_hInst);

        m_pAnime->setTransparent();

        RECT rc = {100, 100, 200, 200};

        m_pAnime->create();
        m_pAnime->setPosition(&rc);
        m_pAnime->show();
        //m_pAnime->open(IDR_AVI_FILECOPY);
        m_pAnime->open(_T("1.avi"));
        //m_pAnime->subclassProc();

        return TRUE;
    }

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::PostMessage(m_hDlg, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)lParam);
        return FALSE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BN_PAUSE:
            m_pAnime->pause();
            return TRUE;
        case IDC_BN_PLAY:
            m_pAnime->play();
            //m_pAnime->disable();
            //m_pAnime->enable();
            return TRUE;
        case IDC_BN_STOP:
            m_pAnime->stop();
            return TRUE;
        case IDC_BN_CHANGE:
            m_pAnime->open(IDR_AVI_FILECOPY);
            ::EnableWindow(::GetDlgItem(m_hDlg, IDC_BN_CHANGE), FALSE);
            return TRUE;
        case IDOK :
            return TRUE;
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }
private:
    UAnimation *m_pAnime;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

