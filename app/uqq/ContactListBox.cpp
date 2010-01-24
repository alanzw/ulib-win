#include <windows.h>
#include <tchar.h>
#include <ocidl.h>
#include <olectl.h>
#include <shlwapi.h>

#include "upicture.h"
#include "ContactListBox.h"

UContactListBox::UContactListBox(HWND hParent, UINT nID, HINSTANCE hInst)
: UListBox(hParent, nID, hInst)
{
    m_dwStyles |= LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS | WS_VSCROLL;
}

BOOL UContactListBox::create()
{
    BOOL ret = UListBox::create();
    this->subclassProc();
    return ret;
}

void UContactListBox::addItem(LPCTSTR str, UPicture *ppic)
{
    int nItem = this->addString(str);
    this->setItemData(nItem, (LPARAM)ppic);
}

BOOL UContactListBox::onDrawItem(WPARAM wParam, LPARAM lParam)
{
    LPDRAWITEMSTRUCT lpDrawItem;
    HDC hdc;

    lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
    hdc = lpDrawItem->hDC;

    TEXTMETRIC tm;
    TCHAR tchBuffer[BUFFER] = "xxx";
    //size_t cch;
    RECT rcBitmap;
    int y;


    // If there are no list box items, skip this message.
    if (lpDrawItem->itemID == (unsigned int)-1)
    {
        return TRUE;
    }

    UPicture *ppic = 0;
    RECT rcPic = {
        lpDrawItem->rcItem.left + 20,
        lpDrawItem->rcItem.top + 5,
        lpDrawItem->rcItem.left + 60,
        lpDrawItem->rcItem.bottom - 5
    };
    // Draw the image and text for the list box item.
    switch (lpDrawItem->itemAction)
    {
    case ODA_SELECT:
    case ODA_DRAWENTIRE:
        // Display the image associated with the item.
        ppic =(UPicture *)::SendMessage(lpDrawItem->hwndItem,
        LB_GETITEMDATA, lpDrawItem->itemID, (LPARAM) 0);
        if (0 != ppic)
        {
            ppic->show(hdc, &rcPic);
        }

        // Display the text associated with the item.

        SendMessage(lpDrawItem->hwndItem, LB_GETTEXT, lpDrawItem->itemID, (LPARAM) tchBuffer);

        GetTextMetrics(hdc, &tm);

        y = (lpDrawItem->rcItem.bottom + lpDrawItem->rcItem.top - tm.tmHeight) / 2;

        TextOut(hdc, XBITMAP + 6, y, tchBuffer, lstrlen(tchBuffer));

        // Is the item selected?

        if (lpDrawItem->itemState & ODS_SELECTED)
        {

            rcBitmap.left = rcPic.left;
            rcBitmap.top = rcPic.top;
            rcBitmap.right = lpDrawItem->rcItem.right-20;
            rcBitmap.bottom = lpDrawItem->rcItem.bottom-5;

            // Draw a rectangle around bitmap to indicate
            // the selection.
            DrawFocusRect(hdc, &rcBitmap);
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

BOOL UContactListBox::onMeasureItem(WPARAM wParam, LPARAM lParam)
{
    LPMEASUREITEMSTRUCT lpmis;
    lpmis = (LPMEASUREITEMSTRUCT) lParam;

    // Set the height of the list box items.
    lpmis->itemHeight = 50;

    return TRUE;
}

