#ifndef U_LISTBOX_H
#define U_LISTBOX_H

#include "ucontrol.h"

class ULIB_API UListBox : public UControl
{
public:
    UListBox(HWND hParent, UINT nID, HINSTANCE hInst);
    UListBox() {}
    ~UListBox();
    //
    virtual BOOL create();
    //
    int addString(const char *lpszString);
    int setItemData(int index, LPARAM data);
    LONG getItemData(int index);
    //
    int findString(const char *lpszString);
    int deleteString(int nIndex);

    //
    int getCurSel();
    BOOL setCurSel(int nIndex);
    //
    int getText(int nIndex, char *sText);

    //
    int setColumnWidth(DWORD pixels);
protected:
private:
};

#endif // U_LISTBOX_H