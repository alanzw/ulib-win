/*
 * =====================================================================================
 *
 *       Filename:  ubasewindow.h
 *
 *    Description:  Class UBaseWindow
 *
 *        Version:  1.0
 *        Created:  2009-8-8 6:03:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_BASE_WINDOW
#define U_BASE_WINDOW

#include "uwndclassx.h"

class ULIB_API UBaseWindow
{
public:
    UBaseWindow(HWND hParent = NULL, HINSTANCE hInst = NULL, LPCTSTR lpWindowClass = NULL, UINT nID = 0);

    //
    explicit UBaseWindow(UBaseWindow *pWndParent);

    //!! The dtor of base class should be virtual
    virtual ~UBaseWindow();

    //
    virtual bool create();

    //
    operator HWND () const
    { return m_hSelf; }

    HWND getHandle() const
    { return m_hSelf; }

    void setHandle(HWND hWnd)
    { m_hSelf = hWnd; }

    HWND getParent() const
    { return m_hParent; }

    void setParent(HWND hParent)
    { m_hParent = hParent; }

    HINSTANCE getInstance() const
    { return m_hInst; }

    LPCTSTR getWndClass() const
    { return m_lpWindowClass; }

    void setWndClassName(LPCTSTR lpWndClassName)
    {
        this->m_lpWindowClass = lpWndClassName;
    }

    UINT getID() const
    { return m_nID; }
    
    void setID(UINT id)
    { m_nID = id; }

    //
    void setTitle(LPCTSTR lpTitle)
    {
        this->m_lpWindowTitle = lpTitle;
    }

    //
    void setRect(RECT *lpRect)
    {
        m_rcWindow.left = lpRect->left;
        m_rcWindow.top = lpRect->top;
        m_rcWindow.right = lpRect->right;
        m_rcWindow.bottom = lpRect->bottom;
    }

    void setPos(int x, int y, int cx, int cy)
    {
        m_rcWindow.left = x;
        m_rcWindow.top = y;
        m_rcWindow.right = x + cx;
        m_rcWindow.bottom = y + cy;
    }

    //
    BOOL setWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlag);
    //
    BOOL setTopMost();
    BOOL setNoTopMost();
    //
    BOOL setWindowPlacement(WINDOWPLACEMENT *lpwndpl);
    BOOL getWindowPlacement(WINDOWPLACEMENT *lpwndpl);
    BOOL getWindowRect(LPRECT lpRect);
    BOOL getClientRect(LPRECT lpRect);
    //
    BOOL moveWindow(int x, int y, int cx, int cy, BOOL bRepaint = FALSE);
    BOOL moveWindow(LPRECT lpRect, BOOL bRepaint = FALSE);

    /* !!! Default Message Handler Entry Point
     *
     */
    virtual BOOL defaultMessageHandler(UINT uMessage, WPARAM wParam, LPARAM lParam);

    /// If the message need be filtered, it should return TRUE;
    virtual BOOL filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
    { return FALSE; }
    virtual BOOL onMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);
    /* !!! WM_CREATE
     * If an application processes this message,
     * it should return zero to continue creation of the window.
     */
    virtual BOOL onCreate() { return FALSE; }

    /* !!! WM_NCCREATE
     * If an application processes this message,
     * it should return TRUE to continue creation of the window.
     */
    virtual BOOL onNCCreate() { return TRUE; }

    /* !!! WM_PAINT
     * An application returns zero if it processes this message.
     */
    virtual BOOL onPaint();
    //
    virtual void onDraw(HDC hdc) {}

    /* !!! WM_DESTROY
     * If an application processes this message, it should return zero.
     */
    virtual BOOL onDestroy() { return FALSE; }

    /* !!! WM_LBUTTONDOWN
     * If an application processes this message, it should return zero.
     */
    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam) {return FALSE;}
    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam) {return FALSE;}
    /*
     *
     */
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam) {return FALSE;}

    /* !!! WM_CHAR
     * An application should return zero if it processes this message.
     */
    virtual BOOL onChar(WPARAM wParam, LPARAM lParam) {return FALSE;}

    /* !!! WM_CLOSE
     * If an application processes this message, it should return zero.
     */
    virtual BOOL onClose();

    //!!! WM_ERASEBKGND
    //!!! An application should return nonzero if it erases the background;
    //!!! otherwise, it should return zero.
    virtual BOOL onEraseBkgnd(HDC hdc) {return FALSE;}

    //
    virtual BOOL onSize(WPARAM wParam, LPARAM lParam) {return FALSE;}

    //!!!
    virtual BOOL onDropFiles(WPARAM, LPARAM) { return FALSE;}
    
    //
    BOOL onCtrlColor(WPARAM wParam, LPARAM lParam);

    //
    BOOL onMeasureItem(WPARAM wParam, LPARAM lParam);
    BOOL onDrawItem(WPARAM wParam, LPARAM lParam);

    //
    BOOL setTimer(UINT_PTR nIDEvent, UINT uElapse);
    BOOL killTimer(UINT_PTR nIDEvent);
    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam) {return FALSE;}

    BOOL show();
    BOOL update();
    BOOL destroy();
    BOOL invalidate(BOOL bErase = FALSE);
    BOOL hide();
    //
    DWORD getStyles() const
    { return m_dwStyles; }

    DWORD getExStyles() const
    { return m_dwExStyles; }

    void setStyles(DWORD dwStyles)
    {
        m_dwStyles = dwStyles;
    }

    void setExStyles(DWORD dwExStyles)
    {
        m_dwExStyles = dwExStyles;
    }

    //
    BOOL addStyles(DWORD dwStyles)
    {
        DWORD dwNewStyle = m_dwStyles | dwStyles;
        if (m_dwStyles == dwNewStyle)
        {
            return FALSE;
        } else {
            m_dwStyles = dwNewStyle;
            return TRUE;
        }
    }
    
    //
    BOOL addExStyles(DWORD dwExStyles)
    {
        DWORD dwNewExStyle = m_dwExStyles | dwExStyles;
        if (m_dwExStyles == dwNewExStyle)
        {
            return FALSE;
        } else {
            m_dwExStyles = dwNewExStyle;
            return TRUE;
        }
    }
    
    //
    virtual BOOL modifyStyles(DWORD dwStyle);
    virtual BOOL modifyExStyles(DWORD dwStyle);
    
    //
    BOOL setWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE);
    
    //
    BOOL adjustWindowRectEx();

    void setMenu(LPCTSTR lpMenuName)
    { m_lpMenuName = lpMenuName; }

    LPCTSTR getMenu() const;

    bool isChild(HWND hParent);

    bool isIconic();

    bool isVisible();

    bool isZoomed();

    LONG sendMsg(UINT message, WPARAM wParam =0, LPARAM lParam =0);
    LONG postMsg(UINT message, WPARAM wParam =0, LPARAM lParam =0);

    bool setIconBig(HICON hIcon);
    bool setIconBig(UINT nID);

    void showMsg(LPCTSTR sMsg, LPCTSTR sTitle = _T("Info"));

    BOOL setWindowText(LPCTSTR lpText);

    HWND setActive();

    BOOL isActive();
protected:
    virtual BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc) {return FALSE;}
    virtual BOOL onPreCreateWindow() {return TRUE;}
private:
    HWND m_hParent;
    HWND m_hSelf;
    HINSTANCE m_hInst;

    LPCTSTR m_lpMenuName;

    DWORD m_dwStyles;
    DWORD m_dwExStyles;

    LPCTSTR m_lpWindowClass;
    LPCTSTR m_lpWindowTitle;

    RECT m_rcWindow;
    UINT m_nID;
protected:
    bool registerWndClass();
public:
    LRESULT WindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);
};


#endif // U_BASE_WINDOW

