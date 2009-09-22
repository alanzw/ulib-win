#ifndef U_COMBOBOX_H
#define U_COMBOBOX_H

#include "ucontrol.h"

class ULIB_API UComboBox : public UControl
{
public:
    UComboBox(HWND hParent, UINT nResource, HINSTANCE hInst);
    UComboBox();
    ~UComboBox();

    //
    virtual BOOL create();

    //
    BOOL setCurSel(const int nIndex = 0);
    int getCurSel();
    BOOL addText(const TCHAR *lpszText);
    int deleteText(const int nIndex = 0);
    int findText(const TCHAR *lpszText, const int nStartIndex = 0);
    BOOL setText(const TCHAR *lpszText);
    int getText(TCHAR *lpszText);
    int setItemHeight(const int height = 20);
    int setDroppedWidth(const int width = 200);
    BOOL showDropDown(BOOL bShow=TRUE);
    BOOL clear();
    int getCount() const;
    
    //
    BOOL setItemData(int nIndex, DWORD dwData);
    DWORD getItemData(int nIndex);
protected:
private:
};

#endif // U_COMBOBOX_H

