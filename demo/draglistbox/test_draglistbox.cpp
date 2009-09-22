#include "resource.h"

#define _WIN32_IE 0x0300
//#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>

#include "udraglistbox.h"
#include "udialogx.h"
#include "udlgapp.h"
#include "uimagelist.h"
#include "umsg.h"

using huys::UDialogBox;

const UINT ID_LISTBOX = 13333;
const UINT ID_ICON_LISTBOX = 13334;


class UIconListBox : public UListBox
{
public:
    UIconListBox(HWND hParent, UINT nID, HINSTANCE hInst)
        : UListBox(hParent, nID, hInst)
    {
        m_dwStyles |= LBS_OWNERDRAWVARIABLE | WS_VSCROLL;
    }
    ~UIconListBox() {};

    virtual BOOL create()
    {
        BOOL ret = UListBox::create();
        this->subclassProc();
        return ret;
    }

    BOOL setImageList(UImageList *uil)
    {
        return TRUE;
    }

    virtual BOOL onMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
    {


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
protected:
private:
};

HBITMAP hbmpPencil, hbmpCrayon, hbmpMarker, hbmpPen, hbmpFork;
HBITMAP hbmpPicture, hbmpOld;

#define BUFFER MAX_PATH
#define XBITMAP 80
#define YBITMAP 20

void AddItem(HWND hwnd, LPTSTR lpstr, HBITMAP hbmp)
{
    int nItem;

    nItem = SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)lpstr);
    SendMessage(hwnd, LB_SETITEMDATA, (WPARAM)nItem, (LPARAM)hbmp);
}


class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID),
        m_pListBox(0),
        m_pIconListBox(0)
    {}

    ~UDialogExt()
    {
        if (m_pListBox)
        {
            delete m_pListBox;
            m_pListBox = 0;
        }

        if (m_pIconListBox)
        {
            delete m_pIconListBox;
            m_pIconListBox = 0;
        }

        // Free any resources used by the bitmaps.

        DeleteObject(hbmpPencil);
        DeleteObject(hbmpCrayon);
        DeleteObject(hbmpMarker);
        DeleteObject(hbmpPen);
        DeleteObject(hbmpFork);

    }

    virtual BOOL onInit()
    {
        if (!m_pListBox)
        {
            m_pListBox = new UDragListBox(m_hDlg, ID_LISTBOX, m_hInst);
            //m_pListBox->setStyles(WS_BORDER | LVS_REPORT | LVS_EDITLABELS);
            m_pListBox->setStyles(WS_HSCROLL|WS_VSCROLL|LBS_DISABLENOSCROLL );
            m_pListBox->create();
            m_pListBox->setColumnWidth(300);
            RECT rc;
            ::GetClientRect(m_hDlg, &rc);
            rc.left += 50;
            rc.right = rc.left + 200;
            rc.top += 50;
            rc.bottom -= 50;
            m_pListBox->setPosition(&rc);
            //

            //
            char str[] = "I love you!\tI am here.\tI am here. ";
            m_pListBox->addString(str);
            m_pListBox->addString(str);
            m_pListBox->addString(str);



            //
            m_pIconListBox = new UListBox(m_hDlg, ID_ICON_LISTBOX, m_hInst);
            m_pIconListBox->setStyles(LBS_OWNERDRAWVARIABLE);
            m_pIconListBox->create();
            rc.left += 250;
            rc.right = rc.left + 200;
            //rc.bottom -= 200;
            m_pIconListBox->setPosition(&rc);

            // Load bitmaps.

            hbmpPencil = LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_ICON1));
            hbmpCrayon = LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_ICON2));
            hbmpMarker = LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_ICON3));
            hbmpPen = LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_ICON2));
            hbmpFork = LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_ICON2));

            // Retrieve list box handle.

            HWND hListBox = m_pIconListBox->getHWND();

            // Initialize the list box text and associate a bitmap
            // with each list box item.

            AddItem(hListBox, "pencil", hbmpPencil);
            AddItem(hListBox, "crayon", hbmpCrayon);
            AddItem(hListBox, "marker", hbmpMarker);
            AddItem(hListBox, "pen",    hbmpPen);
            AddItem(hListBox, "fork",   hbmpFork);

            SetFocus(hListBox);
            SendMessage(hListBox, LB_SETCURSEL, 0, 0);

        }
        return TRUE;
    }

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        BOOL result = UDialogBox::DialogProc(message, wParam, lParam);

        if (message == WM_NOTIFY)
        {
        }

        if (message == WM_MEASUREITEM)
        {
            LPMEASUREITEMSTRUCT lpmis;
            lpmis = (LPMEASUREITEMSTRUCT) lParam;
            // Set the height of the list box items.
            lpmis->itemHeight = 36;
            return TRUE;
        }

        if (message == WM_DRAWITEM)
        {
            LPDRAWITEMSTRUCT lpdis;
            lpdis = (LPDRAWITEMSTRUCT) lParam;
            HDC hdcMem;
            HRESULT hr;
            TEXTMETRIC tm;
            TCHAR tchBuffer[BUFFER] = "xxx";
            size_t cch;
            RECT rcBitmap;
            int y;

            // If there are no list box items, skip this message.

            if (lpdis->itemID == -1)
            {
                return TRUE;
            }

            // Draw the bitmap and text for the list box item. Draw a
            // rectangle around the bitmap if it is selected.

            switch (lpdis->itemAction)
            {
            case ODA_SELECT:
            case ODA_DRAWENTIRE:

                // Display the bitmap associated with the item.

                hbmpPicture =(HBITMAP)SendMessage(lpdis->hwndItem,
                    LB_GETITEMDATA, lpdis->itemID, (LPARAM) 0);

                hdcMem = CreateCompatibleDC(lpdis->hDC);
                hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmpPicture);

                BitBlt(lpdis->hDC,
                    lpdis->rcItem.left, lpdis->rcItem.top,
                    lpdis->rcItem.right - lpdis->rcItem.left,
                    lpdis->rcItem.bottom - lpdis->rcItem.top,
                    hdcMem, 0, 0, SRCCOPY);

                // Display the text associated with the item.

                //SendMessage(lpdis->hwndItem, LB_GETTEXT,
                //    lpdis->itemID, (LPARAM) tchBuffer);

                GetTextMetrics(lpdis->hDC, &tm);

                y = (lpdis->rcItem.bottom + lpdis->rcItem.top -
                    tm.tmHeight) / 2;

                //hr = StringCchLength(tchBuffer, BUFFER, pcch);
                //if (FAILED(hr))
                //{
                    // TODO: Handle error.
                //}
                //cch = strlen(tchBuffer);

                TextOut(lpdis->hDC,
                    XBITMAP + 6,
                    y,
                    tchBuffer,
                    strlen(tchBuffer));

                SelectObject(hdcMem, hbmpOld);
                DeleteDC(hdcMem);

                // Is the item selected?

                if (lpdis->itemState & ODS_SELECTED)
                {
                    // Set RECT coordinates to surround only the
                    // bitmap.

                    rcBitmap.left = lpdis->rcItem.left;
                    rcBitmap.top = lpdis->rcItem.top;
                    rcBitmap.right = lpdis->rcItem.left + XBITMAP;
                    rcBitmap.bottom = lpdis->rcItem.top + YBITMAP;

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

        return result;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hDlg, &ps);
        RECT rt;
        GetClientRect(m_hDlg, &rt);
        rt.top = 5;
        //DrawText(hdc, "Hello World!", strlen("Hello World!"), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
        EndPaint(m_hDlg, &ps);
        return FALSE;
    }
private:
    UDragListBox *m_pListBox;
    UListBox *m_pIconListBox;
};

UDLGAPP_T(UDialogExt, IDD_TEST);

