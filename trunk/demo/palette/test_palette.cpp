#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shellapi.h>
#include <math.h>
#include <stdlib.h>

#include "udialogx.h"
#include "udlgapp.h"
#include "ubutton.h"
#include "colors.h"
#include "ugdi.h"
#include "udc.h"
#include "uregion.h"
#include "upalette.h"

#include "adt/uautoptr.h"

#define  WM_SHELLNOTIFY  WM_USER+5

#define PI 3.14159265358979323846

NOTIFYICONDATA ntd;

using huys::UDialogBox;

HMENU hPopupMenu;

class UDialogExt : public UDialogBox
{
    enum {
        ID_BN_OK     = 5555,
        ID_BN_CANCEL = 5556,
        ID_BN_MIN    = 5557,
        IDM_RESTORE  = 5558,
        IDM_EXIT     = 5559
    };

    enum {
        ID_TIMER_UPDATE = 1111
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID),
      m_bDirection(0)
    {
        m_nHeight=m_nWidth=200;
    }

    virtual BOOL onInit()
    {
        m_pUBtnOK = new UPushButton(m_hDlg, ID_BN_OK, m_hInst);
        m_pUBtnOK->create();
        RECT rcBtn = {250, 260, 320, 300};
        m_pUBtnOK->setPosition(&rcBtn);
        m_pUBtnOK->setWindowText(_T("OK"));

        m_pUBtnCancel = new UPushButton(m_hDlg, ID_BN_CANCEL, m_hInst);
        m_pUBtnCancel->create();
        rcBtn.left += 90;
        rcBtn.right += 90;
        m_pUBtnCancel->setPosition(&rcBtn);
        m_pUBtnCancel->setWindowText(_T("Cancel"));

        //
        UPushButton ubn_min(m_hDlg, ID_BN_MIN, m_hInst);
        rcBtn.left += 90;
        rcBtn.right += 110;
        ubn_min.create();
        ubn_min.setWindowText(_T("&Minimize"));
        ubn_min.setPosition(&rcBtn);

        ntd.cbSize = sizeof(NOTIFYICONDATA);
        ntd.hWnd = m_hDlg;
        ntd.uID = IDI_APP;
        ntd.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        ntd.uCallbackMessage = WM_SHELLNOTIFY;
        ntd.hIcon = ::LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_HELP));
        lstrcpy( ntd.szTip, "hello");

        hPopupMenu = CreatePopupMenu();
        AppendMenu(hPopupMenu, MF_STRING, IDM_RESTORE, "Restore");
        AppendMenu(hPopupMenu, MF_STRING, IDM_EXIT, "Exit");

        int r,g,b;

        r = 255;
        g = 0;
        b = 0;

        plp=(LOGPALETTE *)malloc(sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*64);
        plp->palVersion =0x300;
        plp->palNumEntries =64;

        for(int i=0;i<64;i++)
        {
            plp->palPalEntry [i].peRed =r*sin(PI*i/64);
            plp->palPalEntry[i].peGreen =g*sin(PI*i/64);
            plp->palPalEntry [i].peBlue =b*sin(PI*i/64);
            plp->palPalEntry [i].peFlags =PC_RESERVED;
        }
        palette.createPalette(plp);

        ::srand (::GetTickCount());

        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_COMMAND)
        {
            switch (wParam) {
                case ID_BN_OK:
                    return onBnOk();
                    break;
                case ID_BN_CANCEL:
                    this->onCancel();
                    break;
                case ID_BN_MIN:
                    ::ShowWindow(m_hDlg, SW_MINIMIZE);
                    break;
                default:
                    break;
            }

            if (0 == lParam)
            {
                Shell_NotifyIcon(NIM_DELETE, &ntd);
                switch (wParam)
                {
                case IDM_RESTORE:
                    ShowWindow(m_hDlg, SW_RESTORE);
                    break;
                case IDM_EXIT:
                    return this->onDestroy();
                }

            }

        }

        if (message == WM_SIZE)
        {
            if (wParam == SIZE_MINIMIZED)
            {
                this->hide();
                Shell_NotifyIcon(NIM_ADD, &ntd);
            }
        }

        if (message == WM_SHELLNOTIFY)
        {
            if (wParam==IDI_APP)
            {
                if (lParam==WM_RBUTTONDOWN)
                {
                POINT pt;
                ::GetCursorPos(&pt);
                ::SetForegroundWindow(m_hDlg);
                ::TrackPopupMenu(hPopupMenu, TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, NULL, m_hDlg, NULL);
                ::PostMessage(m_hDlg, WM_NULL, 0, 0);
                }
                else if (lParam==WM_LBUTTONDBLCLK)
                {
                    ::SendMessage(m_hDlg, WM_COMMAND, IDM_RESTORE, 0);
                }
            }
        }



        return result;
    }

    virtual BOOL onPaint()
    {
        UPaintDC dc(m_hDlg);

        RECT rc;
        ::GetClientRect(m_hDlg, &rc);

        return FALSE;
    }

    virtual BOOL onDestroy()
    {
        ::DestroyMenu(hPopupMenu);

        this->killTimer(ID_TIMER_UPDATE);
        return UDialogBox::onDestroy();
    }

    BOOL onTimer(WPARAM wParam, LPARAM lParam)
    {
        if(ID_TIMER_UPDATE == wParam)
        {
            UPrivateDC dc(m_hDlg);
            updateData(dc);
            drawBackground(dc, 0);
        }
        return FALSE;
    }
private:
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnOK;
    huys::ADT::UAutoPtr<UPushButton> m_pUBtnCancel;

    BOOL m_bDirection;

    huys::ADT::UAutoPtr<LOGPALETTE> plp;
    UPalette palette;

    int m_nHeight;
    int m_nWidth;
private:
    BOOL onBnOk()
    {
        this->setTimer(ID_TIMER_UPDATE, 50);
        return FALSE;
    }

    void updateData(HDC hdc)
    {
        PALETTEENTRY temp;

        for(int j=0;j<3;j++)
        {
            if(m_bDirection)
            {
                temp=plp->palPalEntry [0];
                for(int i=0;i<64;i++)
                {
                    plp->palPalEntry[i]=plp->palPalEntry [i+1];
                    plp->palPalEntry [63]=temp;
                }
            }
            else
            {
                temp=plp->palPalEntry [63];
                for(int i=63;i>0;i--)
                {
                    plp->palPalEntry[i]=plp->palPalEntry [i-1];
                }
                plp->palPalEntry [0]=temp;
            }
        }

        palette.animatePalette(0,64,plp->palPalEntry);
    }

    void drawBackground(HDC hdc, int nType)
    {
        palette.selectPalette(hdc, FALSE);
        palette.realizePalette(hdc);
        RECT rt = {0};
        HBRUSH hBrush;
        int i, j;
        switch(nType)
        {
        case 0:
            for (i = 63; i >=0; i--)
            {
                for(j=63;j>=0;j--)
                {
                    rt.left       = i * m_nWidth/ 64 ;
                    rt.top        = j*m_nHeight/64;
                    rt.right      =  (i+1)*m_nWidth / 64;
                    rt.bottom     = (j+1)*m_nHeight/64;
                    hBrush =CreateSolidBrush (PALETTEINDEX (min(64*sin(PI*j/64),64*sin(PI*i/64)))) ;
                    ::FillRect (hdc, &rt, hBrush) ;
                    DeleteObject (hBrush);
                }
            }

            break;
        case 1:
            for (i = 63; i >=0; i--)
            {
                for(j=63;j>=0;j--)
                {
                    rt.left       = i * m_nWidth/ 64 ;
                    rt.top        = j*m_nHeight/64;
                    rt.right      =  (i+1)*m_nWidth / 64;
                    rt.bottom     = (j+1)*m_nHeight/64;

                    if(i%2==0)
                        hBrush = CreateSolidBrush (PALETTEINDEX (64*sin(PI*i/64))) ;
                    else
                        hBrush = CreateSolidBrush (PALETTEINDEX (64*sin(PI*j/64))) ;

                    ::FillRect (hdc, &rt, hBrush) ;
                    DeleteObject (hBrush);
                }
            }
            break;
        case 2:
            for(i=0;i<64;i++)
            {
                rt.left       = i * m_nWidth/ 64 ;
                rt.top        = 0;
                rt.right      =  (i+1)*m_nWidth / 64;
                rt.bottom     = m_nHeight;

                hBrush = CreateSolidBrush (PALETTEINDEX (i)) ;

                ::FillRect (hdc, &rt, hBrush) ;
                DeleteObject (hBrush);
            }
            break;
        case 3:
            for (i = 63; i >=0; i--)
            {

                rt.left       = i * m_nWidth/ 64 ;
                rt.top        = 0;
                rt.right      =  (i+1)*m_nWidth / 64;
                rt.bottom     = m_nHeight;

                hBrush = CreateSolidBrush (PALETTEINDEX (64*sin(PI*i/64))) ;

                ::FillRect (hdc, &rt, hBrush) ;
                DeleteObject (hBrush);
            }
            break;
        case 4:
            for (i = 63; i >=0; i--)
            {

                rt.left       = 0;
                rt.top        = i * m_nHeight/ 64 ;
                rt.right      = m_nWidth;
                rt.bottom     = (i+1)*m_nHeight/64;

                hBrush = CreateSolidBrush (PALETTEINDEX (64*sin(PI*i/64))) ;

                ::FillRect (hdc, &rt, hBrush) ;
                DeleteObject (hBrush);
            }
            break;
        case 5:
            for (i = 63; i >=0; i--)
            {
                for(j=63;j>=0;j--)
                {
                    rt.left       = i * m_nWidth/ 64 ;
                    rt.top        = j*m_nHeight/64;
                    rt.right      =  (i+1)*m_nWidth / 64;
                    rt.bottom     = (j+1)*m_nHeight/64;


                    hBrush = CreateSolidBrush (PALETTEINDEX (j*sin(PI*i/64))) ;


                    ::FillRect (hdc, &rt, hBrush) ;
                    DeleteObject (hBrush);
                }
            }
            break;
        case 6:
            for (i = 63; i >=0; i--)
            {
                for(j=63;j>=0;j--)
                {
                    rt.left       = i * m_nWidth/ 64 ;
                    rt.top        = j*m_nHeight/64;
                    rt.right      =  (i+1)*m_nWidth / 64;
                    rt.bottom     = (j+1)*m_nHeight/64;


                    hBrush = CreateSolidBrush (PALETTEINDEX ((64-j)*sin(PI*i/64))) ;


                    ::FillRect (hdc, &rt, hBrush) ;
                    DeleteObject (hBrush);
                }
            }
            break;

              }
        palette.restorePalette(hdc, FALSE);
    }
};

UDLGAPP_T(UDialogExt, IDD_TEST);
