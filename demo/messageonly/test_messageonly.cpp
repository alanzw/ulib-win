#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "umessageonlywindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UMessageOnlyWindow umsgwnd;

    umsgwnd.create();

    umsgwnd.sendMsg(UMessageOnlyWindow::WM_CUSTOM_MSG);

    Sleep(1000);

    return 0;
}

