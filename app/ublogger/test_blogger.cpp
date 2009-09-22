#include <windows.h>
#include <tchar.h>

#include "ublogger.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UBloggerApp app;
    app.init(hInstance);
    return app.run();
}

