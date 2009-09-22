#ifndef U_DIALOG_H
#define U_DIALOG_H

#include "ulib.h"

//
class ULIB_API UDialogBox
{
public:
    UDialogBox(
        HINSTANCE hInst,
        UINT nID,
        DLGPROC lpDialogFunc=UDialogBox::DefaultDlgProc,
        HWND hParent=NULL);

    virtual ~UDialogBox();

    HWND getHWND();

    virtual BOOL create();
    virtual BOOL hide();
    virtual BOOL show();

    static BOOL CALLBACK DefaultDlgProc(HWND hDlg, UINT message,
        WPARAM wParam, LPARAM lParam);
    //
    static BOOL onCancel(HWND hDlg);
protected:
    UINT m_nDialogID;
    HINSTANCE m_hInst;
    HWND m_hDlg;
    DLGPROC m_lpDialogFunc;
    HWND m_hParent;
private:
};

#endif // U_DIALOG_H

