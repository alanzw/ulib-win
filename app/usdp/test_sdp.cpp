#include <windows.h>
#include <tchar.h>

#include "usdp.h"

bool is_simulating = true;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    USDPApp app;
    app.init(hInstance);
    return app.run();
}

