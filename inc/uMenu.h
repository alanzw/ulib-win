#ifndef U_MENU_H
#define U_MENU_H

#ifndef PUBLIC_AREA
#define PUBLIC_AREA 20 //最左边灰色区域宽度
#endif // PUBLIC_AREA

struct UMenuItemContext//保存菜单的所有有关信息
{
    int     nMenuID; 
    //-2:顶层菜单条 //-1:弹出菜单
    //0:分隔条      //>0:一般的菜单
    UINT    U_icon;
    CString strText;//menu text 
};

class UCoolMenu : public CMenu  
{
public:
    UCoolMenu();
    virtual ~UCoolMenu();
public:
    //property
    CUIntArray m_menuIDs;       // store the menu ids
    CImageList   m_ImageList;   //store the menu icons
public:
    void show();
    //method
    int			GetImageFromToolBar(UINT uToolBarID, CSize sz, CImageList *pImageList,  CUIntArray *uIDArray);
    int			GetIconIndexByID(UINT uID);
    BOOL		ChangeMenuStyle(HMENU hMenu, BOOL bTop);
    BOOL		AttachMenu(HMENU hMenu,UINT uToolBarID,CSize sz);
    UINT		AddToolBar(UINT uToolBarID, CSize sz);
    void		DrawBestRect(CDC *pDC, CRect rect, COLORREF cr1,COLORREF cr2, BOOL bHor);
    void		DrawTop(CDC *pDC,CRect rect,BOOL bSelected);
    void		DrawBGColor(CDC* pDC, CRect rect, BOOL bSelected);
    void		DrawIcon(CDC* pDC, CRect rect, UINT uIndex, BOOL IsEnabled);
    void		DrawText(CDC* pDC, CRect rect, CString sText);
    void		DrawGray(CDC* pDC, CRect rect, CString sText);
    virtual		void DrawItem(LPDRAWITEMSTRUCT lpDIS);
    virtual		void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
};

#endif // U_MENU_H