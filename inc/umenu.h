#ifndef U_MENU_H
#define U_MENU_H

#include "uobject.h"

class ULIB_API UMenu : public UUserObject
{
public:
    UMenu();
    virtual ~UMenu();
    virtual HMENU create();
    virtual HMENU createPopup();
    //
    virtual bool destroy();

    //
    BOOL append(UINT uMenuItem, LPCTSTR lpText);
    BOOL append(UINT uMenuItem, HBITMAP hbmp);
    BOOL addSubMenu(HMENU hSubMenu, LPCTSTR lpText);
    BOOL addSeparator();
    //
    BOOL load(HINSTANCE hInst, LPCTSTR lpMenuName);
    //
    BOOL getMenu(HWND hWnd);

    //
    operator HMENU ()
    { return (HMENU)m_hObj; }

    //
    HMENU getSubMenu(int nPos);

    //
    BOOL trackPopup(UINT uFlag, int x, int y);

    //
    BOOL trackSubPopup(int nPos, UINT uFlag, int x, int y);
    //
    BOOL attach(HWND hParent);
    //
#if(WINVER >= 0x0500)
    BOOL setMenuInfo(LPCMENUINFO lpcmi);
    BOOL getMenuInfo(LPMENUINFO lpmi);
#endif
    //
    HMENU getHandle() const { return (HMENU)m_hObj;}

    //
    UINT getItemID(int nPos);
    BOOL checkItemByID(UINT uId);
    BOOL checkItemByPos(int nPos);
    BOOL uncheckItemByID(UINT uId);
    BOOL uncheckItemByPos(UINT uPos);

    BOOL isItemChecked(UINT uId);
    BOOL isItemDisabled(UINT uId);

    //
    BOOL setBmpByID(UINT nId, HBITMAP hBmpUnChked, HBITMAP hBmpChk);
    BOOL setBmpByPos(int nPos, HBITMAP hBmpUnChked, HBITMAP hBmpChk);

    //
    BOOL isMenu(HMENU hMenu) const
    { return ::IsMenu(hMenu); }

    //
    int getItemCount();

    //
    UINT getMenuStateByID(UINT uId);
    UINT getMenuStateByPos(UINT uPos);

    //
    int getMenuStringByID(UINT uId, LPTSTR sText, int nMaxCount);
    int getMenuStringByPos(UINT uPos, LPTSTR sText, int nMaxCount);

    //

protected:
    //HMENU m_hMenu;
    HWND m_hParent;
    BOOL bManaged;
private:
    //std::vector<UINT> m_childIds;
};

#endif // U_MENU_H

