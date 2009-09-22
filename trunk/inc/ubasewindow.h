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
    UBaseWindow(HWND hParent = NULL, HINSTANCE hInst = NULL, LPCTSTR lpWindowClass = NULL);
    ~UBaseWindow();

    virtual bool create();

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

    void setTitle(LPCTSTR lpTitle)
    {
        this->m_lpWindowTitle = lpTitle;
    }

    void setRect(RECT *lpRect)
    {
        m_rcWindow.left = lpRect->left;
        m_rcWindow.top = lpRect->top;
        m_rcWindow.right = lpRect->right;
        m_rcWindow.bottom = lpRect->bottom;
    }

    virtual BOOL defaultMessageHandler(UINT uMessage, WPARAM wParam, LPARAM lParam);

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

    //
    BOOL setTimer(UINT_PTR nIDEvent, UINT uElapse);
    BOOL killTimer(UINT_PTR nIDEvent);
    virtual BOOL onTimer(WPARAM wParam, LPARAM lParam) {return FALSE;}

    BOOL show();
    BOOL update();
    BOOL destroy();

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
        if (dwStyles == dwNewStyle)
        {
            return FALSE;
        } else {
            m_dwStyles = dwNewStyle;
            return TRUE;
        }
    }

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
private:
    HWND m_hSelf;
    HWND m_hParent;
    HINSTANCE m_hInst;

    LPCTSTR m_lpMenuName;

    DWORD m_dwStyles;
    DWORD m_dwExStyles;

    LPCTSTR m_lpWindowClass;
    LPCTSTR m_lpWindowTitle;

    RECT m_rcWindow;
private:
    bool registerWndClass();
public:
    LRESULT WindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);
};


#endif // U_BASE_WINDOW

