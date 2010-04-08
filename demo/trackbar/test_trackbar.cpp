#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include <cassert>

#include "udialogx.h"
#include "utrackbar.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "udllman.h"
#include "adt/uautoptr.h"

#define HU_VERIFY assert

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_TRACK_BAR = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_pTrackBar = new UTrackBar(m_hDlg, ID_TRACK_BAR, m_hInst);
        m_pTrackBar->setStyles(TBS_ENABLESELRANGE);
        m_pTrackBar->setPos(20, 20, 200, 240);
        m_pTrackBar->create();
        
        m_pTrackBar->setRange(0, 255);
        m_pTrackBar->setRangeSize(20);
        m_pTrackBar->setTicFreq(20);
        m_pTrackBar->setTPos(200);
        return TRUE;
    }
    
    
    BOOL DialogProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_HSCROLL == nMessage)
        {
            return onHScroll();
        }
        return UDialogBox::DialogProc(nMessage, wParam, lParam);
    }
private:
    huys::ADT::UAutoPtr<UTrackBar> m_pTrackBar;
private:
    BOOL onHScroll()
    {
        LRESULT res = SendMessage(m_pTrackBar, TBM_GETPOS, 0, 0);
        //::SetLayeredWindowAttributes(m_hDlg, RGB(255, 0, 0), (BYTE) res, LWA_COLORKEY | LWA_ALPHA);
#if defined(_MSC_VER) && (_MSC_VER > 1200)
        this->modifyExStyles(WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(m_hDlg, RGB(255, 0, 0), (BYTE) res, LWA_COLORKEY | LWA_ALPHA);
#else

#ifndef LWA_ALPHA
#define LWA_ALPHA 2
#endif

#ifndef LWA_COLORKEY
#define LWA_COLORKEY  1
#endif
        this->modifyExStyles(WS_EX_LAYERED);
        UDllMan dlm(_T("user32.dll"));
        dlm.callFunc<BOOL, HWND, COLORREF, BYTE, DWORD>(_T("SetLayeredWindowAttributes"),
            m_hDlg,  RGB(255, 0, 0), (BYTE) res+20, LWA_COLORKEY | LWA_ALPHA);
#endif // _MSC_VER
        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);


