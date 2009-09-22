#ifndef U_MENU_H
#define U_MENU_H

#include "ulib.h"

//#include <vector>

class ULIB_API UMenu
{
public:
    UMenu();
    virtual ~UMenu();
    virtual HMENU create();
    virtual HMENU createPopup();
    //
    BOOL append(UINT uMenuItem, TCHAR *lpText);
    BOOL append(UINT uMenuItem, HBITMAP hbmp);
    BOOL addSubMenu(UMenu *pSubMenu, TCHAR *lpText);
    //
    BOOL load(HINSTANCE hInst, LPCSTR lpMenuName);

	//
	HMENU getSubMenu(int nPos);

    //
    BOOL trackPopup(UINT uFlag, int x, int y);

	//
	BOOL trackSubPopup(int nPos, UINT uFlag, int x, int y); 
    //
    BOOL attach(HWND hParent);
    //
    BOOL setMenuInfo(LPCMENUINFO lpc);

    //
    HMENU getHandle() const { return m_hMenu;}

    //
    UINT getItemID(int nPos) const;
    BOOL checkItemByID(UINT nId);
    BOOL checkItemByPos(int nPos);

    //
    BOOL setBmpByID(UINT nId, HBITMAP hBmpUnChked, HBITMAP hBmpChk);
    BOOL setBmpByPos(int nPos, HBITMAP hBmpUnChked, HBITMAP hBmpChk);

    //
    BOOL isMenu(HMENU hMenu) const
    { return ::IsMenu(hMenu); }

    //
    int getItemCount() const;
protected:
    HMENU m_hMenu;
    HWND m_hParent;
private:
    //std::vector<UINT> m_childIds;
};

#endif // U_MENU_H

