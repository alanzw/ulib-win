#define _WIN32_IE  0x0300


#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <cassert>

#include "ulistview.h"

UListView::UListView(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst)
{
}

UListView::UListView(UBaseWindow *pWndParent, UINT nID)
: UControl(pWndParent, nID)
{
}

UListView::~UListView()
{}

BOOL UListView::create()
{

#if _WIN32_IE < 0x0300
#error _WIN32_IE >= 0x0300 is required.
#endif

    INITCOMMONCONTROLSEX icex;

    // Ensure that the common control DLL is loaded.
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC  = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    return UControl::create(_T("SysListView32"));
}

BOOL UListView::setImageList(HIMAGELIST himl, int iImage)
{
    return this->sendMsg(LVM_SETIMAGELIST, (WPARAM)(int)iImage, (LPARAM)himl);
}

BOOL UListView::setImageListNormal(HIMAGELIST himl)
{
    return this->setImageList(himl, LVSIL_NORMAL);
}

BOOL UListView::setImageListSmall(HIMAGELIST himl)
{
    return this->setImageList(himl, LVSIL_SMALL);
}

BOOL UListView::setImageListState(HIMAGELIST himl)
{
    return this->setImageList(himl, LVSIL_STATE);
}

BOOL UListView::setImageListGroupHeader(HIMAGELIST himl)
{
//    return this->setImageList(himl, LVSIL_GROUPHEADER);
	return TRUE;
}

BOOL UListView::addColumn( int iCol, LPLVCOLUMN lplvc )
{
	return this->sendMsg(LVM_INSERTCOLUMN, (WPARAM)(int)iCol, (LPARAM)(LPLVCOLUMN)lplvc);
}

BOOL UListView::addItem( LVITEM *lplvi )
{
	return this->sendMsg(LVM_INSERTITEM, 0, (LPARAM)(LVITEM*)lplvi);
}

BOOL UListView::addColTextWidth( int iCol, LPSTR lpText, int nWidth)
{
    LVCOLUMN lvc = {0};
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.iSubItem = 0;
    lvc.pszText = lpText;
    lvc.cx = nWidth;     // width of column in pixels
    lvc.fmt = LVCFMT_LEFT;  // left-aligned column
    return this->addColumn(iCol, &lvc);
}

BOOL UListView::addItemTextImage( int nIndex, LPSTR lpText, int iImage )
{
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iItem = nIndex;
    lvi.iImage = iImage;
    lvi.iSubItem = 0;
    lvi.lParam = 0;
    lvi.pszText = lpText; // sends an LVN_GETDISP message.

    return this->addItem(&lvi);
}

BOOL UListView::getColumn( int iCol, LPLVCOLUMN lplvc )
{
    assert(NULL != lplvc);
    return this->sendMsg(LVM_INSERTCOLUMN, (WPARAM)iCol, (LPARAM)lplvc);
}

BOOL UListView::setItem(int nIndex, LVITEM *lplvi)
{
	return this->sendMsg(LVM_SETITEMTEXT, (WPARAM)nIndex, (LPARAM)lplvi);
}

BOOL UListView::setItemText(int nIndex, int nSubIndex, LPTSTR lpText)
{
	LVITEM lstItem = {0};
	lstItem.mask = LVIF_TEXT;
	lstItem.iItem = nIndex;
	lstItem.iSubItem = nSubIndex;
	lstItem.pszText = lpText;
	return this->setItem(nIndex, &lstItem);
}

