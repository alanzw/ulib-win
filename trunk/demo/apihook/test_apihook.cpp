#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "apihook.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
    if (!InstallHook())
    {
        MessageBoxA(NULL, "Hook Error", "Hook", MB_OK);
        return 1;
    }
    MessageBoxA(NULL, "TEST", "TEST", MB_OK);
    
    if (!UninstallHook())
    {
        MessageBoxA(NULL, "Uninstall Error", "Hook", MB_OK);
        return 1;
    }

    return 0;
}
