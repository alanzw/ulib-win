/*! \class UControl
 *  \brief  The Base Windows Control Class.
 *  \author hys
 *  \version 0.1
 *  \date   2008-2009
 *  \bug It contains bugs.
 *  \warning This class may explode in your face.
 *
 * Some details about the Test class
 */

#ifndef U_CONTROL_H
#define U_CONTROL_H

#include "ubasewindow.h"

class ULIB_API UControl
{
public:
    UControl(HWND hParent, UINT nResource, HINSTANCE hInst);
    explicit UControl(UBaseWindow *pWinParent, UINT nID);
    UControl();
    virtual ~UControl() {};

    UControl *fromHandle(HWND hwnd);
    UControl *fromID(HWND hParent, UINT nID);

    virtual BOOL create() { return FALSE; }
    virtual BOOL destroy();
    //
    BOOL enable() {return ::EnableWindow(m_hSelf, TRUE);}
    BOOL disable() {return ::EnableWindow(m_hSelf, FALSE);}

    //
    operator HWND() const {return m_hSelf;}
    HWND getHWND() const {return m_hSelf;}
    HWND getHParent() const {return m_hParent;}
    HINSTANCE getHInstance() const {return m_hInstance;}
    UINT getResID() const {return m_nResourceID;}
    BOOL getClientRect(RECT *rc);
    void clientToDialog(HWND hDlg, LPRECT lpRect);

    //
    BOOL setPosition(const RECT *rect);
    BOOL setPositionEx(int x, int y, int nWidth, int nHeight, BOOL bRepaint = FALSE);
    //
    void setPos(LONG x, LONG y, LONG w, LONG h);
    void setRect(const LPRECT rc);

    HWND setParentWnd(HWND hParent);

    HWND getPrevParent() const
    {
        return m_hPrevParent;
    }

    //
    BOOL setWindowText(const TCHAR *lpText);
    BOOL getWindowText(TCHAR *lpText, int nMaxCount);

    //
    BOOL setStyles(DWORD dwStyle);
    BOOL modifyStyles(DWORD dwStyle);

    BOOL modifyExStyles(DWORD dwExStyle);
    //
    BOOL isVisible() const;
    BOOL show();
    BOOL hide();

    //
    BOOL invalidate(BOOL bRedraw = TRUE);
    BOOL update();
    //
    virtual BOOL subclassProc();
    virtual BOOL restoreProc();
    WNDPROC getOriginProc() const;

    virtual BOOL onMessage(UINT,WPARAM,LPARAM);
    virtual BOOL onLButtonDown(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onRButtonDown(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onLButtonUp(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onRButtonUp(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onMouseMove(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onChar(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onEnable(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onShow(WPARAM,LPARAM) {return FALSE;}
    virtual BOOL onPaint();
    virtual void onDraw(HDC hdc) {}
    virtual BOOL onSetFocus() {return FALSE;}
    virtual BOOL onKillFocus() {return FALSE;}
    virtual BOOL onKeyDown(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onKeyUp(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam) {return FALSE;}
    //
    virtual BOOL onSize(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onMove(WPARAM wParam, LPARAM lParam) {return FALSE;}
    //
    BOOL setTimer(UINT_PTR nIDEvent, UINT uElapse);
    BOOL killTimer(UINT_PTR nIDEvent);
    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam) {return FALSE;}

    //
    void setFont(HFONT hFont);
    HFONT getFont();

    /// Functions for message reflection
    virtual BOOL onNotify(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onNotifyReflect(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onCtrlColor(WPARAM, LPARAM){return FALSE;}
    virtual BOOL onDrawItem(WPARAM,LPARAM){return FALSE;}
    virtual BOOL onMeasureItem(WPARAM, LPARAM) {return FALSE;}

    //
    virtual BOOL onEraseBkgnd(HDC hdc) {return FALSE;}

    //////////////////////////////////////////////////////////////////////////
    virtual BOOL subclass(HWND hCtrl);
    virtual BOOL subclass(UINT nID, HWND hParent);
protected:

    ///
    BOOL create(const TCHAR *strCtrlClass, const TCHAR *strWindowText = _T(""));

    ///
    BOOL createEx( DWORD dwExStyle,
                   const TCHAR *strCtrlClass = _T(""),
                   const TCHAR *strWindowText = _T("") );

    ///
    BOOL sendMsg(UINT, WPARAM =0, LPARAM =0);
    BOOL postMsg(UINT, WPARAM =0, LPARAM =0);
    //////////////////////////////////////////////////////////////////////////
    BOOL sendMsgParent(UINT uMessage, WPARAM wParam=0, LPARAM lParam=0);
    BOOL postMsgParent(UINT uMessage, WPARAM wParam=0, LPARAM lParam=0);
    ///
    BOOL setLong();

    ///
    HWND m_hParent;
    UINT m_nResourceID;
    HWND m_hSelf;
    HINSTANCE m_hInstance;
    RECT m_rc;
    DWORD m_dwStyles;

    //
    HWND m_hPrevParent;
private:
    WNDPROC m_OriginProc;

    /// Redirected control procedure
    static LRESULT APIENTRY newControlProc(HWND, UINT, WPARAM, LPARAM);

    //
    bool m_bManaged;
};

/*! \fn const BOOL *UControl::invalidate(BOOL bRedraw = TRUE)
 *  \brief Triggering the redraw of the control.
 *  \param bRedraw whether to redraw.
 *  \exception none.
 *  \return the state.
 */

#endif // U_CONTROL_H

