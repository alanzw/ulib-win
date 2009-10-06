#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uheader.h"


UHeaderCtrl::UHeaderCtrl( HWND hParent, UINT nResource, HINSTANCE hInst )
: UControl(hParent, nResource, hInst)
{
    m_dwStyles |= HDS_BUTTONS | HDS_HORZ;
}

UHeaderCtrl::~UHeaderCtrl()
{

}

BOOL UHeaderCtrl::create()
{
    // Ensure that the common control DLL is loaded, and then create
    // the header control.
    InitCommonControls();

    return UControl::create(_T("SysHeader32"));
}

BOOL UHeaderCtrl::layout()
{
    RECT rc;
    HDLAYOUT hdl;
    WINDOWPOS wp;
    // Retrieve the bounding rectangle of the parent window's
    // client area, and then request size and position values
    // from the header control.
    GetClientRect(m_hParent, &rc);

    hdl.prc = &rc;
    hdl.pwpos = &wp;
    if (!SendMessage(m_hSelf, HDM_LAYOUT, 0, (LPARAM) &hdl))
        return FALSE;

    // Set the size, position, and visibility of the header control.
    SetWindowPos(m_hSelf, wp.hwndInsertAfter, wp.x, wp.y,
        wp.cx, wp.cy, wp.flags | SWP_SHOWWINDOW);

}

// DoInsertItem - inserts an item into a header control.
// Returns the index of the new item.
// hwndHeader - handle to the header control.
// iInsertAfter - index of the previous item.
// nWidth - width of the new item.
// lpsz - address of the item string.
int UHeaderCtrl::insertItem(int iInsertAfter, int nWidth, LPSTR lpsz)
{
    HDITEM hdi;
    int index;

    hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
    hdi.pszText = lpsz;
    hdi.cxy = nWidth;
    hdi.cchTextMax = sizeof(hdi.pszText)/sizeof(hdi.pszText[0]);
    hdi.fmt = HDF_LEFT | HDF_STRING;

    index = SendMessage(m_hSelf, HDM_INSERTITEM,
        (WPARAM) iInsertAfter, (LPARAM) &hdi);

    return index;
}
