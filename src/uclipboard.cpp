#include <windows.h>

#include "uclipboard.h"

UClipBoard::UClipBoard()
{}

UClipBoard::~UClipBoard()
{}


/*

#define BOX_ELLIPSE  0

#define BOX_RECT     1



#define CCH_MAXLABEL 80

#define CX_MARGIN    12



typedef struct tagLABELBOX {  // box

    RECT rcText;    // coordinates of rectangle containing text

    BOOL fSelected; // TRUE if the label is selected

    BOOL fEdit;     // TRUE if text is selected

    int nType;      // rectangular or elliptical

    int ichCaret;   // caret position

    int ichSel;     // with ichCaret, delimits selection

    int nXCaret;    // window position corresponding to ichCaret

    int nXSel;      // window position corresponding to ichSel

    int cchLabel;   // length of text in atchLabel

    TCHAR atchLabel[CCH_MAXLABEL];

} LABELBOX, *PLABELBOX;



void WINAPI InitMenu(HMENU hmenu)
{
    int  cMenuItems = GetMenuItemCount(hmenu);
    int  nPos;
    UINT id;
    UINT fuFlags;
    PLABELBOX pbox = (hwndSelected == NULL) ? NULL :
        (PLABELBOX) GetWindowLong(hwndSelected, 0);

    for (nPos = 0; nPos < cMenuItems; nPos++)
    {
        id = GetMenuItemID(hmenu, nPos);

        switch (id)
        {
            case IDM_CUT:
            case IDM_COPY:
            case IDM_DELETE:
                if (pbox == NULL || !pbox->fSelected)
                    fuFlags = MF_BYCOMMAND | MF_GRAYED;
                else if (pbox->fEdit)
                    fuFlags = (id != IDM_DELETE && pbox->ichSel
                            == pbox->ichCaret) ?
                        MF_BYCOMMAND | MF_GRAYED :
                        MF_BYCOMMAND | MF_ENABLED;
                else
                    fuFlags = MF_BYCOMMAND | MF_ENABLED;

                EnableMenuItem(hmenu, id, fuFlags);
                break;

            case IDM_PASTE:
                if (pbox != NULL && pbox->fEdit)
                    EnableMenuItem(hmenu, id,
                        IsClipboardFormatAvailable(CF_TEXT) ?
                            MF_BYCOMMAND | MF_ENABLED :
                            MF_BYCOMMAND | MF_GRAYED
                    );
                else
                    EnableMenuItem(hmenu, id,
                        IsClipboardFormatAvailable(
                                uLabelFormat) ?
                            MF_BYCOMMAND | MF_ENABLED :
                            MF_BYCOMMAND | MF_GRAYED
                    );

        }
    }
}

#define BOX_ELLIPSE  0
#define BOX_RECT     1

#define CCH_MAXLABEL 80
#define CX_MARGIN    12

typedef struct tagLABELBOX {  // box
    RECT rcText;    // coordinates of rectangle containing text
    BOOL fSelected; // TRUE if the label is selected
    BOOL fEdit;     // TRUE if text is selected
    int nType;      // rectangular or elliptical
    int ichCaret;   // caret position
    int ichSel;     // with ichCaret, delimits selection
    int nXCaret;    // window position corresponding to ichCaret
    int nXSel;      // window position corresponding to ichSel
    int cchLabel;   // length of text in atchLabel
    TCHAR atchLabel[CCH_MAXLABEL];
} LABELBOX, *PLABELBOX;

BOOL WINAPI EditCopy(VOID)
{
    PLABELBOX pbox;
    LPTSTR  lptstrCopy;
    HGLOBAL hglbCopy;
    int ich1, ich2, cch;

    if (hwndSelected == NULL)
        return FALSE;

    // Open the clipboard, and empty it.

    if (!OpenClipboard(hwndMain))
        return FALSE;
    EmptyClipboard();

    // Get a pointer to the structure for the selected label.

    pbox = (PLABELBOX) GetWindowLong(hwndSelected, 0);

    // If text is selected, copy it using the CF_TEXT format.

    if (pbox->fEdit)
    {
        if (pbox->ichSel == pbox->ichCaret)     // zero length
        {
            CloseClipboard();                   // selection
            return FALSE;
        }

        if (pbox->ichSel < pbox->ichCaret)
        {
            ich1 = pbox->ichSel;
            ich2 = pbox->ichCaret;
        }
        else
        {
            ich1 = pbox->ichCaret;
            ich2 = pbox->ichSel;
        }
        cch = ich2 - ich1;

        // Allocate a global memory object for the text.

        hglbCopy = GlobalAlloc(GMEM_MOVEABLE,
            (cch + 1) * sizeof(TCHAR));
        if (hglbCopy == NULL)
        {
            CloseClipboard();
            return FALSE;
        }

        // Lock the handle and copy the text to the buffer.

        lptstrCopy = GlobalLock(hglbCopy);
        memcpy(lptstrCopy, &pbox->atchLabel[ich1],
            cch * sizeof(TCHAR));
        lptstrCopy[cch] = (TCHAR) 0;    // null character
        GlobalUnlock(hglbCopy);

        // Place the handle on the clipboard.

        SetClipboardData(CF_TEXT, hglbCopy);
    }

    // If no text is selected, the label as a whole is copied.

    else
    {
        // Save a copy of the selected label as a local memory
        // object. This copy is used to render data on request.
        // It is freed in response to the WM_DESTROYCLIPBOARD
        // message.

        pboxLocalClip = (PLABELBOX) LocalAlloc(
            LMEM_FIXED,
            sizeof(LABELBOX)
        );
        if (pboxLocalClip == NULL)
        {
            CloseClipboard();
            return FALSE;
        }
        memcpy(pboxLocalClip, pbox, sizeof(LABELBOX));
        pboxLocalClip->fSelected = FALSE;
        pboxLocalClip->fEdit = FALSE;

        // Place a registered clipboard format, the owner-display
        // format, and the CF_TEXT format on the clipboard using
        // delayed rendering.

        SetClipboardData(uLabelFormat, NULL);
        SetClipboardData(CF_OWNERDISPLAY, NULL);
        SetClipboardData(CF_TEXT, NULL);
    }

    // Close the clipboard.

    CloseClipboard();

    return TRUE;
}

#define BOX_ELLIPSE  0
#define BOX_RECT     1

#define CCH_MAXLABEL 80
#define CX_MARGIN    12

typedef struct tagLABELBOX {  // box
    RECT rcText;    // coordinates of rectangle containing text
    BOOL fSelected; // TRUE if the label is selected
    BOOL fEdit;     // TRUE if text is selected
    int nType;      // rectangular or elliptical
    int ichCaret;   // caret position
    int ichSel;     // with ichCaret, delimits selection
    int nXCaret;    // window position corresponding to ichCaret
    int nXSel;      // window position corresponding to ichSel
    int cchLabel;   // length of text in atchLabel
    TCHAR atchLabel[CCH_MAXLABEL];
} LABELBOX, *PLABELBOX;

VOID WINAPI EditPaste(VOID)
{
    PLABELBOX pbox;
    HGLOBAL   hglb;
    LPTSTR    lptstr;
    PLABELBOX pboxCopy;
    int cx, cy;
    HWND hwnd;

    pbox = hwndSelected == NULL ? NULL :
        (PLABELBOX) GetWindowLong(hwndSelected, 0);

    // If the application is in edit mode,
    // get the clipboard text.

    if (pbox != NULL && pbox->fEdit)
    {
        if (!IsClipboardFormatAvailable(CF_TEXT))
            return;
        if (!OpenClipboard(hwndMain))
            return;

        hglb = GetClipboardData(CF_TEXT);
        if (hglb != NULL)
        {
            lptstr = GlobalLock(hglb);
            if (lptstr != NULL)
            {
                // Call the application-defined ReplaceSelection
                // function to insert the text and repaint the
                // window.

                ReplaceSelection(hwndSelected, pbox, lptstr);
                GlobalUnlock(hglb);
            }
        }
        CloseClipboard();

        return;
    }

    // If the application is not in edit mode,
    // create a label window.

    if (!IsClipboardFormatAvailable(uLabelFormat))
        return;
    if (!OpenClipboard(hwndMain))
        return;

    hglb = GetClipboardData(uLabelFormat);
    if (hglb != NULL)
    {
        pboxCopy = GlobalLock(hglb);
        if (pboxCopy != NULL)
        {
            cx = pboxCopy->rcText.right + CX_MARGIN;
            cy = pboxCopy->rcText.top * 2 + cyText;

            hwnd = CreateWindowEx(
                WS_EX_NOPARENTNOTIFY | WS_EX_TRANSPARENT,
                atchClassChild, NULL, WS_CHILD, 0, 0, cx, cy,
                hwndMain, NULL, hinst, NULL
            );
            if (hwnd != NULL)
            {
                pbox = (PLABELBOX) GetWindowLong(hwnd, 0);
                memcpy(pbox, pboxCopy, sizeof(LABELBOX));
                ShowWindow(hwnd, SW_SHOWNORMAL);
                SetFocus(hwnd);
            }
            GlobalUnlock(hglb);
        }
    }
    CloseClipboard();
}
*/

bool UClipBoard::copyToClipborad(const TCHAR *text)
{
    if (::OpenClipboard(NULL) != 0)
    {
        if (::EmptyClipboard() != 0)
        {
            const int len = lstrlen(text);

            if (len>0)
            {
                HGLOBAL hbuf = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
                                             (len+1)*sizeof(TCHAR));
                if (hbuf != 0)
                {
                    TCHAR *data = (TCHAR *)::GlobalLock(hbuf);
                    lstrcpy(data, text);
                    data[len] = (TCHAR)0;
                    ::GlobalUnlock(hbuf);
                    ::SetClipboardData(CF_TEXT, hbuf);
                }
            }
        }

        ::CloseClipboard();

        return true;
    }

    return false;
}

bool UClipBoard::getFromClipboard(TCHAR *buf)
{

    if (OpenClipboard (0) != 0)
    {
        HANDLE bufH = GetClipboardData (CF_TEXT);

        if (bufH != 0)
        {
            const char* const data = (const char*) GlobalLock (bufH);

            if (data != 0)
            {
                ::memcpy(buf, data, (int) (GlobalSize (bufH) / sizeof(char)));

                GlobalUnlock (bufH);
            }
        }

        CloseClipboard();
    }



    return false;
}

