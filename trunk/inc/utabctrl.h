#ifndef U_TAB_CTRL_H
#define U_TAB_CTRL_H

#include "ucontrol.h"

class ULIB_API UTabCtrl : public UControl
{
public:
    UTabCtrl(HWND hParent, UINT nResource, HINSTANCE hInst);
    UTabCtrl() {};
    virtual ~UTabCtrl();
    virtual BOOL create();

    BOOL insertItem(int nIndex, LPTCITEM pitem);

    //
    BOOL insertItemTextImage(int nIndex, LPTSTR lpText, int iImage = 0);

    BOOL setImageList(HIMAGELIST himl);

    int getCurSel();
    int setCurSel(int nIndex);
    int getItemCount();

    LONG setItemSize(int cx, int cy);
    BOOL getItemRect(int nIndex, LPRECT lpRect);
protected:
private:

};

#endif // U_TAB_CTRL_H

