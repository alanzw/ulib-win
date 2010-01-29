#ifndef U_DIALOG_H
#define U_DIALOG_H

#include "ulib.h"

namespace huys
{

//
class ULIB_API UDialogBox
{
public:
    UDialogBox(
        HINSTANCE hInst,
        UINT nID,
        DLGPROC lpDialogFunc = UDialogBox::DefaultDlgProc,
        HWND hParent=NULL
        );

    virtual ~UDialogBox();

    HWND getHWND();
    void setHWND(HWND hWnd) {m_hDlg = hWnd;}

    typedef enum {
        U_DLG_MODALESS,
        U_DLG_MODAL
    } DlgMode;  
    
    DlgMode getMode() const
    { return m_mode; }
    
    virtual BOOL setParentH(HWND hParent);

    virtual BOOL create();
    virtual BOOL hide();
    virtual BOOL show();
    virtual BOOL destroy();
    
    //
    virtual BOOL modifyStyles(DWORD dwStyle);
    virtual BOOL modifyExStyles(DWORD dwStyle);

    virtual BOOL DialogProc(UINT, WPARAM, LPARAM);

    static BOOL CALLBACK DefaultDlgProc(HWND hDlg, UINT message,
        WPARAM wParam, LPARAM lParam);
    //
    virtual BOOL onInit() {return TRUE;}
    virtual BOOL onCancel();
    virtual BOOL onClose();
    virtual BOOL onDestroy();
    virtual BOOL onNCDestroy();

    //
    virtual BOOL onPaint();

    virtual void onDraw(HDC hdc) {}

    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onRButtonDown(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onRButtonUp(WPARAM wParam, LPARAM lParam) {return FALSE;}

    //!!! If an application processes this message, it should return zero.
    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam)  {return FALSE;}

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL onNotify(WPARAM wParam, LPARAM lParam);
    static BOOL onCtrlColor(WPARAM wParam, LPARAM lParam);
    static BOOL onDrawItem(WPARAM wParam, LPARAM lParam);
    static BOOL onMeasureItem(HWND hDlg, WPARAM wParam, LPARAM lParam);

    //!!! An application should return nonzero if it erases the background;
    //!!! otherwise, it should return zero.
    virtual BOOL onEraseBkgnd(HDC hdc) {return FALSE;}

    //!!!
    virtual BOOL onSysCommand(WPARAM wParam, LPARAM lParam) { return FALSE;}

    //!!!
    virtual BOOL onDropFiles(WPARAM wParam, LPARAM lParam) { return FALSE;}

    //
    BOOL setTimer(UINT_PTR nIDEvent, UINT uElapse);
    BOOL killTimer(UINT_PTR nIDEvent);
    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam) {return FALSE;}

    //
    BOOL setDlgIconBig(HICON hIcon);
    BOOL setDlgIconSmall(HICON hIcon);

    //
    BOOL setDlgItemText(UINT uID, LPCTSTR lpString);
    BOOL setDlgItemInt(UINT uID, UINT uValue, BOOL bSigned = FALSE);
    BOOL sendDlgItemMessage(UINT uID, UINT uMessage, WPARAM wParam, LPARAM lParam);

    HWND getDlgItem(UINT uID);
    UINT getDlgItemText(UINT uID, LPTSTR lpString, int nMaxCount);
    BOOL getDlgItemInt(UINT uID, int *lpValue, BOOL bSigned = FALSE);
    int getDlgCtrlID(HWND hCtrl);
    
    //
    BOOL setWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE);
    
    //
    BOOL setTopMost();
    BOOL setNoTopMost();
    
    //
    BOOL redraw();
    BOOL bringToTop();
    BOOL update();
    //
    BOOL bInitialized;
protected:
    BOOL sendMsg(UINT, WPARAM wParam=0, LPARAM lParam=0);

    BOOL sendChildMsg(UINT nID, UINT nMessage, WPARAM wParam=0, LPARAM lParam=0);

    BOOL setLong();
    virtual BOOL go();
    UINT m_nDialogID;
    HINSTANCE m_hInst;
    HWND m_hDlg;
    DLGPROC m_lpDialogFunc;
    HWND m_hParent;

    DlgMode m_mode;
private:
};

} // namespace huys

#endif // U_DIALOG_H

