#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>

#include "udialogx.h"
#include "udlgapp.h"
#include "uanimation.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        IDC_ANIME = 10001
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        m_pAnime = new UAnimation(m_hDlg, IDC_ANIME, m_hInst);
        m_pAnime->setPos(100, 100, 100, 100);
        m_pAnime->setTransparent();
        m_pAnime->create();
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
    huys::ADT::UAutoPtr<UAnimation> m_pAnime;
};

UDLGAPP_T(UDialogExt, IDD_DIALOG1);

