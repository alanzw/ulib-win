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

#include "ubitmap.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
    enum {
        ID_SCROLLBAR = 13333,
        ID_SCROLLBAR_V = 13334
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
          _nStartX(100), _nStartY(50)
    {}

    virtual BOOL onInit()
    {
        m_pScrollBar = new UScrollBar(m_hDlg, ID_SCROLLBAR, m_hInst);
        m_pScrollBar->setStyles(SBS_HORZ);
        m_pScrollBar->setPos(100, 30, 300, 20);
        m_pScrollBar->create();
        m_pScrollBar->setRange(1, 700);
        m_pScrollBar->setScrollPos(100);
//        m_pScrollBar->disable();

        m_pScrollBarV = new UScrollBar(m_hDlg, ID_SCROLLBAR_V, m_hInst);
        m_pScrollBarV->setStyles(SBS_VERT);
        m_pScrollBarV->setPos(450, 50, 20, 200);
        m_pScrollBarV->create();
        m_pScrollBarV->setRange(1, 400);
        m_pScrollBarV->setScrollPos(50);

        _bmp.loadFromResource(IDB_PICTURE, m_hInst);

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (WM_HSCROLL == message)
            return this->onHScroll(wParam, lParam);

        if (WM_VSCROLL == message)
            return this->onVScroll(wParam, lParam);

        return UDialogBox::DialogProc(message, wParam, lParam);
    }

    void onDraw(HDC hdc)
    {
        _bmp.show(hdc, 100, 100, 300, 200, _nStartX, _nStartY);
    }

private:
    huys::ADT::UAutoPtr<UScrollBar> m_pScrollBar;
    huys::ADT::UAutoPtr<UScrollBar> m_pScrollBarV;
    UBitmap _bmp;
    int _nStartX;
    int _nStartY;

    //
    BOOL onHScroll(WPARAM wParam, LPARAM lParam)
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

        _nStartX = m_pScrollBar->getScrollPos();

        RECT rcPicture = { 100, 100, 400, 300 };
        this->redraw(&rcPicture);

        return FALSE;
    }

    BOOL onVScroll(WPARAM wParam, LPARAM lParam)
    {
        int nCurPos = m_pScrollBarV->getScrollPos();
        int min, max;
        m_pScrollBarV->getRange(&min, &max);

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

        m_pScrollBarV->setScrollPos(nCurPos);

        _nStartY = m_pScrollBarV->getScrollPos();

        RECT rcPicture = { 100, 100, 400, 300 };
        this->redraw(&rcPicture);

        return FALSE;
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);

