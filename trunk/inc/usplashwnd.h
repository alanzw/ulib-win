#ifndef U_SPLASH_WINDOW_H
#define U_SPLASH_WINDOW_H

#include "ubitmap.h"
#include "ubasewindow.h"

class ULIB_API USplashWindow : public UBaseWindow
{
public:
    USplashWindow(HINSTANCE hInst);
    USplashWindow(LPCTSTR sFileName);
    ~USplashWindow();

    void setBmp(LPCTSTR sFileName);

    void setBmp(UINT nID, HINSTANCE hInst);

    bool create();

    bool showSplash(DWORD dwDelayedTime = 3000);

    bool showByTimer(DWORD dwDelayedTime = 3000);
//protected:
    //
    void onDraw(HDC hdc);
    BOOL onTimer(WPARAM wParam, LPARAM lParam);
private:
    TCHAR m_sFilename[MAX_PATH];

    UBitmap m_ubmp;
private:
    void centerWindow();
};

#endif // U_SPLASH_WINDOW_H
