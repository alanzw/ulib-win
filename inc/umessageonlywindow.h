#ifndef U_MESSAGE_ONLY_WINDOW_H
#define U_MESSAGE_ONLY_WINDOW_H

#include "ubasewindow.h"

class ULIB_API UMessageOnlyWindow : public UBaseWindow
{
public:
    enum{
        WM_CUSTOM_MSG = WM_USER + 100
    };
public:
    UMessageOnlyWindow();
    ~UMessageOnlyWindow();

    static BOOL setMessageOnly(HWND hwnd);

    static HWND findWindowEx(LPCTSTR lpszClass, LPCTSTR lpszWindow);

    virtual BOOL onMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);
private:
    BOOL msgHandler();
};

#endif // U_MESSAGE_ONLY_WINDOW_H
