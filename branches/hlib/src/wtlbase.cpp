#include "wtlbase.h"

CAppModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    _Module.Init ( NULL, hInstance );

    CMyWindow wndMain;
    MSG msg;

    // Create the main window
    if ( NULL == wndMain.CreateEx() )
        return 1;       // uh oh, window creation failed

    // Show the window
    wndMain.ShowWindow ( nCmdShow );
    wndMain.UpdateWindow();

    // Standard Win32 message loop
    while ( GetMessage ( &msg, NULL, 0, 0 ) > 0 )
    {
        if ( !TranslateAccelerator ( msg.hwnd, wndMain.m_hAccel, &msg ) )
        {
            TranslateMessage ( &msg );
            DispatchMessage ( &msg );
        }
    }

    _Module.Term();
    return msg.wParam;
}
