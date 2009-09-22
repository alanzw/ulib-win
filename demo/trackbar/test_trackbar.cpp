#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include <cassert>

#include "udialogx.h"
#include "utrackbar.h"
#include "udlgapp.h"
#include "uimagelist.h"

#define HU_VERIFY assert

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_TRACK_BAR = 11
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_pTrackBar(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pTrackBar);
    }

    BOOL onInit()
    {
        m_pTrackBar = new UTrackBar(m_hDlg, ID_TRACK_BAR, m_hInst);
        m_pTrackBar->setPos(20, 20, 240, 240);
        m_pTrackBar->create();
        return TRUE;
    }
private:
    UTrackBar *m_pTrackBar;
};

UDLGAPP_T(UDialogExt, IDD_TEST);


