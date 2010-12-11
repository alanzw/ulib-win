#ifndef U_GL_WINDOW_H
#define U_GL_WINDOW_H

#include "ubasewindow.h"

class ULIB_API UGLWindow : public UBaseWindow
{
public:
    typedef void (*InitFunc)();
    typedef void (*DisplayFunc)();
    typedef void (*ReshapeFunc)(int,int);
public:
    UGLWindow(HINSTANCE hInst = ::GetModuleHandle(NULL));

    BOOL onPreRegisterWindowClass(huys::UWindowClass &uwc);

    BOOL onPreCreateWindow();

    BOOL onCreate();

    BOOL onDestroy();

    void onDraw(HDC hdc);

    BOOL onChar(WPARAM wParam, LPARAM lParam);

    BOOL onSize(WPARAM wParam, LPARAM lParam);

    BOOL onTimer(WPARAM wParam, LPARAM lParam);

    //
    void setInit(InitFunc pf)
    { _init_func = pf; }

    void setDisplay(DisplayFunc pf)
    { _disp_func = pf; }
    
    void setReshape(ReshapeFunc pf)
    {
        _reshape_func = pf;
    }

    //
    void setInterval(UINT uInterval)
    {
        m_uInterval = uInterval;
    }

private:
    HDC m_hdc;
    HGLRC m_hrc;
    UINT m_uInterval;
    BOOL m_bFullscreen;

    InitFunc _init_func;
    DisplayFunc _disp_func;
    ReshapeFunc _reshape_func;

    enum { ID_TIMER_INTERNAL = 22 };
private:
    void render();

};

#endif // U_GL_WINDOW_H
