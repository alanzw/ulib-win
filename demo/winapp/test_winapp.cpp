#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "uwinapp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;

    app.init(hInstance);

    return app.run();
}

