#include "resource.h"

#define _WIN32_IE 0x0300

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shellapi.h>
#include <assert.h>

#include "ulistbox.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"
#include "colors.h"
#include "udc.h"
#include "ubitmap.h"

#include "adt/uautoptr.h"

using huys::UDialogBox;

#define BUFFER MAX_PATH
#define XBITMAP 80
#define YBITMAP 20

class UIconListBox : public UListBox
{
public:
    UIconListBox(HWND hParent, UINT nID, HINSTANCE hInst)
        : UListBox(hParent, nID, hInst)
    {
        m_dwStyles |= LBS_OWNERDRAWVARIABLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS;

		// Default Item Height: 36
        _nItemHeight = 36;
    }

    virtual BOOL create()
    {
		// Drag-and-Drop is supported
		return UListBox::createEx(WS_EX_ACCEPTFILES, _T("LISTBOX")) && this->subclassProc();
    }

    virtual BOOL onMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_DROPFILES == nMessage)
        {
            return onDropFiles((HDROP)wParam);
        }

        return UListBox::onMessage(nMessage, wParam, lParam);
    }

    /////////////////////////////////////////////////////////////////////////////
    // ILB_IMAGE_DATA
    //
    struct ILB_IMAGE_DATA
    {
        int m_nImage;
        DWORD m_dwUserData;

        ILB_IMAGE_DATA()
        {
            m_nImage = -1;
            m_dwUserData = 0;
        };
    };

    virtual BOOL onMeasureItem(WPARAM wParam, LPARAM lParam)
    {
        LPMEASUREITEMSTRUCT lpmis;
        lpmis = (LPMEASUREITEMSTRUCT) lParam;

        // Set the height of the list box items.
        lpmis->itemHeight = _nItemHeight;

        return TRUE;
    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        HBITMAP hbmpPicture, hbmpOld;

        LPDRAWITEMSTRUCT lpdis;
        lpdis = (LPDRAWITEMSTRUCT) lParam;

        USmartDC dc(lpdis->hDC);
        UMemDC memDC(dc);

        TEXTMETRIC tm;
        TCHAR tchBuffer[BUFFER] = {0};

        RECT rcBitmap;
        int y;

        // If there are no list box items, skip this message.
        if (lpdis->itemID == (unsigned int)-1)
        {
            return TRUE;
        }

        //SetBkMode(lpdis->hDC, TRANSPARENT);
        dc.setBKMode(TRANSPARENT);

        //_crOldBkColor = ::GetBkColor(lpdis->hDC);
        //_crOldTextColor = ::GetTextColor(lpdis->hDC);
        _crOldBkColor = dc.getBKColor();
        _crOldTextColor = dc.getTextColor();

        // Draw the bitmap and text for the list box item. Draw a
        // rectangle around the bitmap if it is selected.
        switch (lpdis->itemAction)
        {
        case ODA_SELECT:
            if (lpdis->itemState & ODS_SELECTED)
            {
                // Fill the item rect with the highlight blue color
                dc.setBKColor(::GetSysColor(COLOR_HIGHLIGHT));
                dc.extTextOut(0, 0, ETO_OPAQUE, &lpdis->rcItem, NULL, 0, NULL);

                // Set the color of the background of the text rect
                dc.setBKColor(::GetSysColor(COLOR_HIGHLIGHT));

                // Set the color of the text
                dc.setTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
            }
            else
            {
                // Fill the item rect with the highlight blue color
                dc.setBKColor(::GetSysColor(COLOR_BTNFACE));
                dc.extTextOut(0, 0, ETO_OPAQUE, &lpdis->rcItem, NULL, 0, NULL);
            }
        case ODA_DRAWENTIRE:

            // Display the bitmap associated with the item.
            hbmpPicture =(HBITMAP)SendMessage(lpdis->hwndItem,
                LB_GETITEMDATA, lpdis->itemID, (LPARAM) 0);

            //hdcMem = CreateCompatibleDC(lpdis->hDC);
            hbmpOld = (HBITMAP)memDC.selectObj(hbmpPicture);

            dc.bitBlt(lpdis->rcItem.left+2, lpdis->rcItem.top + 2,
                lpdis->rcItem.right - lpdis->rcItem.left + 2,
                lpdis->rcItem.bottom - lpdis->rcItem.top - 2,
                memDC, 0, 0, SRCCOPY);

            // Display the text associated with the item.
            SendMessage(lpdis->hwndItem, LB_GETTEXT,
                lpdis->itemID, (LPARAM) tchBuffer);

            GetTextMetrics(dc, &tm);

            y = (lpdis->rcItem.bottom + lpdis->rcItem.top -
                tm.tmHeight) / 2;

            SetTextColor(lpdis->hDC, huys::xpblue);
            dc.textOut(XBITMAP + 6,
                		y,
                        tchBuffer,
                        strlen(tchBuffer));

            memDC.selectObj(hbmpOld);

            // Is the item selected?
            if (lpdis->itemState & ODS_SELECTED)
            {
                // Set RECT coordinates to surround only the
                // bitmap.
                rcBitmap.left = lpdis->rcItem.left;
                rcBitmap.top = lpdis->rcItem.top;
                //rcBitmap.right = lpdis->rcItem.left + XBITMAP;
                //rcBitmap.bottom = lpdis->rcItem.top + YBITMAP;
                rcBitmap.right = lpdis->rcItem.right;
                rcBitmap.bottom = lpdis->rcItem.bottom;
                // Draw a rectangle around bitmap to indicate
                // the selection.
                DrawFocusRect(lpdis->hDC, &rcBitmap);
            }
            break;

        case ODA_FOCUS:
            // Do not process focus changes. The focus caret
            // (outline rectangle) indicates the selection.
            // The IDOK button indicates the final
            // selection.
            break;
        }
        return TRUE;
    }

    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdc = (HDC)wParam;
        ::SetBkMode(hdc, TRANSPARENT);
        return (BOOL)(HBRUSH)::GetSysColorBrush(COLOR_BTNFACE);
    }

public:
    void setItemHeight(int n)
    {
        _nItemHeight = n;
    }

	void addItem(LPCTSTR str, HBITMAP hbmp)
	{
		int nItem = this->addString(str);
		this->setItemData(nItem, (LPARAM)hbmp);
	}

private:
    BOOL onDropFiles(HDROP hDropInfo)
    {
        char strFileName[MAX_PATH], strDrive[MAX_PATH], strDir[MAX_PATH], strExt[MAX_PATH];
        char szFilePath[MAX_PATH];

        ::DragQueryFile (hDropInfo, 0, szFilePath, sizeof (szFilePath));

        //showMsg(szFilePath);

        HICON h = ::ExtractIcon(m_hInstance,szFilePath,0);

        // This function splits the whole path into Drive, Dir, File Name and Extension
        ::_splitpath(szFilePath, strDrive, strDir, strFileName, strExt);

        if (h)
        {
            // After getting the the Icon handle and the Icon file name,
            // Add them to the list box
            UBitmap bm;
            //UPrivateDC dc(m_hSelf);
            //huys::Color clrBk = dc.getBKColor();
            HBITMAP hbm = bm.copyIcon(h, ::GetSysColor(COLOR_BTNFACE));

            addItem(strFileName, hbm);
        }

        ::DestroyIcon(h);

        ::DragFinish(hDropInfo);

        return FALSE;
    }
private:
    int _nItemHeight;

    COLORREF _crOldTextColor;
    COLORREF _crOldBkColor;

};

class UDialogExt : public UDialogBox
{
    enum {
        ID_LISTBOX = 13333,
        ID_ICON_LISTBOX = 13334
    };
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        huys::URectL rect;
        GetClientRect(m_hDlg, rect);


        m_pListBox = new UListBox(m_hDlg, ID_LISTBOX, m_hInst);
        //m_pListBox->setStyles(WS_BORDER | LVS_REPORT | LVS_EDITLABELS);
        m_pListBox->setStyles(WS_HSCROLL|WS_VSCROLL|LBS_DISABLENOSCROLL );
        m_pListBox->setPos(rect.left()+50, rect.top()+50,
            150, rect.height()-100);
        m_pListBox->create();
        m_pListBox->setColumnWidth(300);
        //
        char str[] = "I love you!\tI am here.\tI am here. ";
        m_pListBox->addString(str);
        m_pListBox->addString(str);
        m_pListBox->addString(str);

        m_pIconListBox = new UIconListBox(m_hDlg, ID_ICON_LISTBOX, m_hInst);
        m_pIconListBox->setStyles(LBS_OWNERDRAWVARIABLE);
        m_pIconListBox->setPos(rect.left()+250, rect.top()+50, 200, rect.height()-100);
        m_pIconListBox->create();

        // Load bitmaps.
        bmpPencil.loadFromResource(IDB_ICON1, m_hInst);
        bmpCrayon.loadFromResource(IDB_ICON2, m_hInst);
        bmpMarker.loadFromResource(IDB_ICON3, m_hInst);
        bmpPen.loadFromResource(IDB_ICON1, m_hInst);
        bmpFork.loadFromResource(IDB_ICON2, m_hInst);

        m_pIconListBox->addItem("pencil", bmpPencil);
        m_pIconListBox->addItem("crayon", bmpCrayon);
        m_pIconListBox->addItem("marker", bmpMarker);
        m_pIconListBox->addItem("pen",    bmpPen);
        m_pIconListBox->addItem("fork", bmpFork);

        m_pIconListBox->setCurSel(-1);

        TCHAR tchCurDir[MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, tchCurDir);
        ::DlgDirList(m_hDlg, tchCurDir, IDC_LIST_FILE, IDC_SDIR, DDL_DIRECTORY);

        return TRUE;
    }

    BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        if ( ID_ICON_LISTBOX == LOWORD(wParam) && LBN_DBLCLK == HIWORD(wParam))
        {
            int index = m_pIconListBox->getCurSel();
            char buf[256] = {0};
            m_pIconListBox->getText(index, buf);
            showMsgFormat("info", "%d -- %s", index, buf);
        }
        return UDialogBox::onCommand(wParam, lParam);
    }

private:
    huys::ADT::UAutoPtr<UListBox> m_pListBox;
    huys::ADT::UAutoPtr<UIconListBox> m_pIconListBox;

    UBitmap bmpPencil, bmpCrayon, bmpMarker, bmpPen, bmpFork;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

