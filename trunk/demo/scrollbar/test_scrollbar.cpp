#include "resource.h"

#define _WIN32_IE 0x0300

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"
#include "uscrollbar.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_SCROLLBAR = 13333
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pScrollBar(0)
    {}

    ~UDialogExt()
    {
        CHECK_PTR(m_pScrollBar);
    }

    virtual BOOL onInit()
    {
        m_pScrollBar = new UScrollBar(m_hDlg, ID_SCROLLBAR, m_hInst);
        m_pScrollBar->setStyles(SBS_HORZ);
        m_pScrollBar->setPos(100, 100, 300, 50);
        m_pScrollBar->create();
        m_pScrollBar->setRange(1, 100);
        m_pScrollBar->setScrollPos(50);
//        m_pScrollBar->disable();
        return TRUE;
    }

	virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (WM_HSCROLL == message)
			return this->onScroll(wParam, lParam);

		return UDialogBox::DialogProc(message, wParam, lParam);
	}

private:
    UScrollBar *m_pScrollBar;

	//
	BOOL onScroll(WPARAM wParam, LPARAM lParam)
	{
		int nCurPos = m_pScrollBar->getScrollPos();
		int min, max;
		m_pScrollBar->getRange(&min, &max);

		switch (LOWORD(wParam))
        {
                case SB_LEFT:
                        nCurPos = min;
                        break;

                case SB_LINELEFT:
                        if (nCurPos > min)
                                nCurPos--;
                        break;

                case SB_THUMBPOSITION:
                        nCurPos = HIWORD(wParam);
                        break;

                case SB_THUMBTRACK:
                        nCurPos = HIWORD(wParam);
                        break;

                case SB_LINERIGHT:
                        if (nCurPos < max)
                                nCurPos++;
                        break;

                case SB_RIGHT:
                        nCurPos = max;
                        break;

                case SB_ENDSCROLL:
                        break;
        }

		m_pScrollBar->setScrollPos(nCurPos);

		
		return FALSE;
	}
};

UDLGAPP_T(UDialogExt, IDD_TEST);

