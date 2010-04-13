#ifndef U_COMBOBOX_EX_H
#define U_COMBOBOX_EX_H

#include "ucontrol.h"

class ULIB_API UComboBoxEx : public UControl
{
public:
    UComboBoxEx(HWND hParent, UINT nResource, HINSTANCE hInst);
    //
    UComboBoxEx(UBaseWindow *pWndParent, UINT nID);

    //
    virtual ~UComboBoxEx();

    //
    virtual BOOL create();

    //
    bool addItem(int nIndex, LPCTSTR sText, int cchTextMax);

    //
    HWND getCombo();

    /*
     * messages that a ComboBoxEx control forwards to its child combo box
     *  CB_DELETESTRING
     * CB_FINDSTRINGEXACT
     * CB_GETCOUNT
     * CB_GETCURSEL
     * CB_GETDROPPEDCONTROLRECT
     * CB_GETDROPPEDSTATE
     * CB_GETITEMDATA
     * CB_GETITEMHEIGHT
     * CB_GETLBTEXT
     * CB_GETLBTEXTLEN
     * CB_GETEXTENDEDUI
     * CB_LIMITTEXT
     * CB_RESETCONTENT
     * CB_SETCURSEL
     * CB_SETDROPPEDWIDTH
     * CB_SETEXTENDEDUI
     * CB_SETITEMDATA
     * CB_SETITEMHEIGHT
     * CB_SHOWDROPDOWN
     */
    //
    BOOL setCurSel(const int nIndex = 0);
    int getCurSel();

    //
    void setImageList(HIMAGELIST himl);
protected:
private:
};

#endif // U_COMBOBOX_EX_H

