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
    int setItemData(WPARAM index, LPARAM data);
    //
    int findString(const char *lpszString);
    int deleteString(int nIndex);
    
    //
    int getCurSel();
    
    //
    int getText(int nIndex, char *sText);
    
    //
    int setColumnWidth(DWORD pixels);
protected:
private:
};

#endif // U_LISTBOX_H
