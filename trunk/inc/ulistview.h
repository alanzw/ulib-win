#ifndef U_LISTVIEW_H
#define U_LISTVIEW_H

#include "ucontrol.h"
#include "colors.h"

#ifdef UNICODE
 #ifndef LVCOLUMN
  #define  LVCOLUMN               LVCOLUMNW
 #endif // LVCOLUMN
 #ifndef LPLVCOLUMN
  #define  LPLVCOLUMN             LPLVCOLUMNW
 #endif // LPLVCOLUMN
#else
 #ifndef LVCOLUMN
  #define  LVCOLUMN               LVCOLUMNA
 #endif // LVCOLUMN
 #ifndef LPLVCOLUMN
  #define  LPLVCOLUMN             LPLVCOLUMNA
 #endif // LPLVCOLUMN
#endif

class ULIB_API UListView : public UControl
{
public:
    UListView(HWND hParent, UINT nID, HINSTANCE hInst);
    UListView() {};
    UListView(UBaseWindow *pWndParent, UINT nID);
    ~UListView();
    //
    virtual BOOL create();
    //
    BOOL setExStylesListView(DWORD dwStyle);

    //
    virtual BOOL addColumn(int iCol, LPLVCOLUMN lplvc);
    //
    BOOL getColumn(int iCol, LPLVCOLUMN lplvc);
    //
    int getColumnWidth(int iCol);

    virtual BOOL addItem(LVITEM *lplvi);
    //
    virtual BOOL addColTextWidth(int iCol, LPCSTR lpText, int nWidth);
    //
    virtual BOOL addItemTextImage(int nIndex, LPCSTR lpText, int iImage);
    //
    virtual BOOL setImageList(HIMAGELIST himl, int iImage);
    virtual BOOL setImageListNormal(HIMAGELIST himl);
    virtual BOOL setImageListSmall(HIMAGELIST himl);
    virtual BOOL setImageListState(HIMAGELIST himl);
    virtual BOOL setImageListGroupHeader(HIMAGELIST himl);
    //
    virtual BOOL setItem(int nIndex, LVITEM *lplvi);
    virtual BOOL setItemText(int nIndex, int nSubIndex, LPCTSTR lpText);

    BOOL getItem(LVITEM *lplvi);
    int getItemText(int nItem, int nSubItem, LPTSTR lpBuffer, int cchTextMax);

    int getItemCount();

    BOOL deleteAllItems();

	//
	BOOL isItemChecked(int nItem);
	void setItemChecked(int nItem, BOOL bChecked);

	//
	BOOL getItemRect(int nItem, LPRECT lpRect);
	BOOL getSubItemRect(int nItem, int nSubItem, LPRECT lpRect);


    BOOL setBKColor(huys::Color clr);
    BOOL setTextColor(huys::Color clr);
    BOOL setTextBKColor(huys::Color clr);

    BOOL setBKImage(TCHAR *url, int xPos = 0, int yPos = 0);
protected:
private:
};

#endif // U_LISTVIEW_H

