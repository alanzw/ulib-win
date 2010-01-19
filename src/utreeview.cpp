#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

#include "utreeview.h"

UTreeView::UTreeView(HWND hParent, UINT nResource, HINSTANCE hInst)
: UControl(hParent, nResource, hInst)
{}

UTreeView::~UTreeView()
{}

UTreeView::UTreeView()
: UControl()
{}

UTreeView::UTreeView(UBaseWindow *pWndParent, UINT nID)
: UControl(pWndParent, nID)
{}

BOOL UTreeView::create()
{
    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    return UControl::create(_T("SysTreeView32"));
}

BOOL UTreeView::setImageList(HIMAGELIST himl, int iImage)
{
    return this->sendMsg(TVM_SETIMAGELIST, (WPARAM)iImage, (LPARAM)himl);
}

BOOL UTreeView::setNormalImageList(HIMAGELIST himl)
{
    return setImageList(himl, TVSIL_NORMAL);
}

BOOL UTreeView::setStateImageList(HIMAGELIST himl)
{
    return setImageList(himl, TVSIL_STATE);
}

BOOL UTreeView::removeImageList()
{
    return ( setNormalImageList(NULL) && setStateImageList(NULL));
}

HTREEITEM UTreeView::addItem( TVINSERTSTRUCT *ptvis )
{
    return (HTREEITEM)this->sendMsg(TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)ptvis);
}

HTREEITEM UTreeView::addItemRoot(TVITEM *ptvi)
{
    return this->addItemChild(TVI_ROOT, ptvi);
}

HTREEITEM UTreeView::addItemChild( HTREEITEM hParent, TVITEM *ptvi )
{
    TVINSERTSTRUCT tvis;
    tvis.hParent = hParent;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item = *ptvi;

    return this->addItem(&tvis);
}

HTREEITEM UTreeView::addTextRoot( LPSTR lpText )
{
    return this->addTextChild(TVI_ROOT, lpText);
}

HTREEITEM UTreeView::addTextChild( HTREEITEM hParent, LPSTR lpText )
{
    TVITEM tvi;
    tvi.pszText = lpText;
    tvi.mask = TVIF_TEXT | TVIF_PARAM;
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]);

    return this->addItemChild(hParent, &tvi);
}

BOOL UTreeView::HitTest( LPTVHITTESTINFO lpht )
{
    return this->sendMsg(TVM_HITTEST, 0, (LPARAM) (LPTVHITTESTINFO) lpht);
}

BOOL UTreeView::getItem( TVITEM *ptvi )
{
    return this->sendMsg(TVM_GETITEM, 0, (LPARAM) (LPTVITEM) ptvi);
}


/*
HTREEITEM UTreeView::getHitItem(TVITEM *ptvi)
{
    TVHITTESTINFO tvhi;
    tvhi.flags = TVHT_ONITEMLABEL;
    this->HitTest(&tvhi);

    if (!tvhi.hItem)
    {
        return NULL;
    }

    return this->getItem(ptvi);
}


LPTSTR UTreeView::getHitText( LPSTR lpText )
{
    TVITEM tvi = {0};
    //this->getHitItem(&tvi);
    //strcpy(lpText, tvi.pszText);
    HTREEITEM item;
    if (!getSelectedItem(item))
    {
        return NULL;
    }

    if (this->getSelectedItem(&tvi))
    {
        wsprintf(lpText, "%s", tvi.pszText);
        return lpText;
    }
    else
        return NULL;
}
*/
LPSTR UTreeView::getSelectedItem( LPSTR lpText )
{
    HTREEITEM item = 0;
    item = (HTREEITEM)this->sendMsg(TVM_GETNEXTITEM, (WPARAM) (DWORD)TVGN_CARET, (LPARAM) (HTREEITEM) item);

    TVITEM tvi = {0};
    tvi.hItem = item;
    tvi.mask = TVIF_TEXT;
    tvi.pszText = lpText;
    tvi.cchTextMax = 256;
    this->sendMsg(TVM_GETITEM, 0, (LPARAM) (LPTVITEM) &tvi);

    return lpText;
}

BOOL UTreeView::onLButtonDown( WPARAM wParam, LPARAM lParam )
{
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    TVHITTESTINFO tvhi = {0};
    tvhi.flags = TVHT_ONITEMLABEL;
    tvhi.pt = pt;
    this->HitTest(&tvhi);

    char buffer[256] = {0};

    TVITEM tvi = {0};
    tvi.hItem = tvhi.hItem;
    tvi.mask = TVIF_TEXT;
    tvi.pszText = buffer;
    tvi.cchTextMax = 256;
    this->sendMsg(TVM_GETITEM, 0, (LPARAM) (LPTVITEM) &tvi);

    //::MessageBox(this->m_hSelf, buffer, "Info", MB_OK);

    return FALSE;
}

