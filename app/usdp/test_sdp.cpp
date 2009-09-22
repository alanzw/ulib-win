#include <windows.h>
#include <tchar.h>

#include "usdp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    USDPApp app;
    app.init(hInstance);
    return app.run();
}

