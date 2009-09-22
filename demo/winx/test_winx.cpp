#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwindowx.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    huys::theApp = new huys::UWindowApp;
    huys::theApp->initInstance(hInstance);
    huys::theApp->run();
    return 0;
}

