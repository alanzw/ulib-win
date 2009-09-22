#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <cassert>
#include "umenu.h"

UMenu::UMenu()
{
    //m_childIds.clear();
}

UMenu::~UMenu()
{
    if (m_hMenu)
    {
        ::DestroyMenu(m_hMenu);
    }
}

HMENU UMenu::create()
{
    m_hMenu = ::CreateMenu();
    //
    assert(m_hMenu);
    return m_hMenu;
}

HMENU UMenu::createPopup()
{
    m_hMenu = ::CreatePopupMenu();
    //
    assert(m_hMenu);
    return m_hMenu;
    m_hMenu = ::CreateMenu();
    //
    assert(m_hMenu);
    return m_hMenu;
}

BOOL UMenu::append(UINT uMenuItem, TCHAR *lpszText)
{
    //m_childIds.push_back(uMenuItem);
    return ::AppendMenu(m_hMenu, MF_STRING, uMenuItem, lpszText);
}

BOOL UMenu::append( UINT uMenuItem, HBITMAP hbmp )
{
    return ::AppendMenu(m_hMenu, MF_BITMAP, uMenuItem, (LPCTSTR) hbmp);
}

BOOL UMenu::load(HINSTANCE hInst, LPCSTR lpMenuName)
{
    m_hMenu = ::LoadMenu(hInst, lpMenuName);
    return TRUE;
}

BOOL UMenu::trackPopup(UINT uFlag, int x, int y)
{
    return ::TrackPopupMenu(m_hMenu, uFlag, x, y, NULL, m_hParent, NULL);
}

BOOL UMenu::attach(HWND hParent)
{
    m_hParent = hParent;
    return TRUE;
}

BOOL UMenu::setMenuInfo(LPCMENUINFO lpc)
{
    return ::SetMenuInfo(m_hMenu, lpc);
}

BOOL UMenu::addSubMenu( UMenu *pSubMenu, TCHAR *lpText )
{
    return ::AppendMenu(m_hMenu, MF_STRING|MF_POPUP, (UINT_PTR)pSubMenu->m_hMenu, lpText);
}

UINT UMenu::getItemID( int nPos ) const
{
    return ::GetMenuItemID(m_hMenu, nPos);
}

BOOL UMenu::checkItemByID( UINT nId )
{
    return ::CheckMenuItem(m_hMenu, nId, MF_CHECKED);
}

BOOL UMenu::checkItemByPos( int nPos )
{
    UINT nId = this->getItemID(nPos);
    return ::CheckMenuItem(m_hMenu, nId, MF_CHECKED);
}

int UMenu::getItemCount() const
{
    return ::GetMenuItemCount(m_hMenu);
}

BOOL UMenu::setBmpByID( UINT nId, HBITMAP hBmpUnChked, HBITMAP hBmpChk)
{
    return ::SetMenuItemBitmaps(m_hMenu, nId, MF_BYCOMMAND, hBmpUnChked, hBmpChk);
}

BOOL UMenu::setBmpByPos( int nPos, HBITMAP hBmpUnChked, HBITMAP hBmpChk)
{
    return ::SetMenuItemBitmaps(m_hMenu, nPos, MF_BYPOSITION, hBmpUnChked, hBmpChk);
}

HMENU UMenu::getSubMenu( int nPos )
{
	return ::GetSubMenu(m_hMenu, nPos);
}

BOOL UMenu::trackSubPopup( int nPos, UINT uFlag, int x, int y )
{
	HMENU hSubMenu = this->getSubMenu(nPos);
	return ::TrackPopupMenu(hSubMenu, uFlag, x, y, NULL, m_hParent, NULL);
}