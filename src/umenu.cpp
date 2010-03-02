#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>
#include "umenu.h"

UMenu::UMenu()
:bManaged(TRUE)
{
    //m_childIds.clear();
}

UMenu::~UMenu()
{
    this->destroy();
}

HMENU UMenu::create()
{
    m_hObj = ::CreateMenu();
    //
    assert(m_hObj);
    return (HMENU)*this;
}

HMENU UMenu::createPopup()
{
    m_hObj = ::CreatePopupMenu();
    //
    assert(m_hObj);
    return (HMENU)*this;
}

bool UMenu::destroy()
{
    if (bManaged && m_hObj)
    {
        ::DestroyMenu(*this);
    }
    return true;
}

BOOL UMenu::append(UINT uMenuItem, LPCTSTR lpszText)
{
    //m_childIds.push_back(uMenuItem);
    return ::AppendMenu(*this, MF_STRING, uMenuItem, (LPTSTR)lpszText);
}

BOOL UMenu::append( UINT uMenuItem, HBITMAP hbmp )
{
    return ::AppendMenu(*this, MF_BITMAP, uMenuItem, (LPTSTR) hbmp);
}

BOOL UMenu::load(HINSTANCE hInst, LPCSTR lpMenuName)
{
    m_hObj = ::LoadMenu(hInst, lpMenuName);
    return TRUE;
}

BOOL UMenu::getMenu(HWND hWnd)
{
    assert(NULL == m_hObj);
    m_hObj = ::GetMenu(hWnd);
    bManaged = FALSE;
    return (NULL != m_hObj);
}

BOOL UMenu::trackPopup(UINT uFlag, int x, int y)
{
    return ::TrackPopupMenu(*this, uFlag, x, y, NULL, m_hParent, NULL);
}

BOOL UMenu::attach(HWND hParent)
{
    m_hParent = hParent;
    return TRUE;
}

#if(WINVER >= 0x0500)
BOOL UMenu::setMenuInfo(LPCMENUINFO lpcmi)
{
    return ::SetMenuInfo(*this, lpcmi);
}

BOOL UMenu::getMenuInfo(LPMENUINFO lpmi)
{
    return ::GetMenuInfo(*this, lpmi);
}
#endif

BOOL UMenu::addSubMenu( HMENU hSubMenu, LPCTSTR lpText )
{
    return ::AppendMenu(*this, MF_STRING|MF_POPUP, (UINT_PTR)hSubMenu, (LPTSTR)lpText);
}

UINT UMenu::getItemID( int nPos )
{
    return ::GetMenuItemID(*this, nPos);
}

BOOL UMenu::checkItemByID( UINT uId )
{
    return ::CheckMenuItem(*this, uId, MF_CHECKED);
}

BOOL UMenu::checkItemByPos( int nPos )
{
    UINT uId = this->getItemID(nPos);
    return ::CheckMenuItem(*this, uId, MF_CHECKED);
}

BOOL UMenu::uncheckItemByID( UINT uId )
{
    return ::CheckMenuItem(*this, uId, MF_BYCOMMAND|MF_UNCHECKED);
}

BOOL UMenu::uncheckItemByPos( UINT uPos )
{
    UINT uId = this->getItemID(uPos);
    return ::CheckMenuItem(*this, uId, MF_BYPOSITION|MF_UNCHECKED);
}


BOOL UMenu::isItemChecked(UINT uId)
{
    return (MF_CHECKED & this->getMenuStateByID(uId));
}

BOOL UMenu::isItemDisabled(UINT uId)
{
    return (MF_DISABLED & this->getMenuStateByID(uId));
}

int UMenu::getItemCount()
{
    return ::GetMenuItemCount(*this);
}

BOOL UMenu::setBmpByID( UINT nId, HBITMAP hBmpUnChked, HBITMAP hBmpChk)
{
    return ::SetMenuItemBitmaps(*this, nId, MF_BYCOMMAND, hBmpUnChked, hBmpChk);
}

BOOL UMenu::setBmpByPos( int nPos, HBITMAP hBmpUnChked, HBITMAP hBmpChk)
{
    return ::SetMenuItemBitmaps(*this, nPos, MF_BYPOSITION, hBmpUnChked, hBmpChk);
}

HMENU UMenu::getSubMenu( int nPos )
{
    return ::GetSubMenu(*this, nPos);
}

BOOL UMenu::trackSubPopup( int nPos, UINT uFlag, int x, int y )
{
    HMENU hSubMenu = this->getSubMenu(nPos);
    return ::TrackPopupMenu(hSubMenu, uFlag, x, y, NULL, m_hParent, NULL);
}

UINT UMenu::getMenuStateByID(UINT uId)
{
    return ::GetMenuState(*this, uId, MF_BYCOMMAND);
}

UINT UMenu::getMenuStateByPos(UINT uPos)
{
    return ::GetMenuState(*this, uPos, MF_BYPOSITION);
}

int UMenu::getMenuStringByID(UINT uId, LPTSTR sText, int nMaxCount)
{
    return ::GetMenuString(*this, uId, sText, nMaxCount, MF_BYCOMMAND);
}

int UMenu::getMenuStringByPos(UINT uPos, LPTSTR sText, int nMaxCount)
{
    return ::GetMenuString(*this, uPos, sText, nMaxCount, MF_BYPOSITION);
}

BOOL UMenu::addSeparator()
{
    return ::AppendMenu(*this, MF_SEPARATOR, -1, NULL);
}



