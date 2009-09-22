#include "resource.h"
#include "uwindow.h"

UApp *theApp = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    theApp = new UApp;
    theApp->initInstance(hInstance);
    theApp->run();
    return 0;
}

