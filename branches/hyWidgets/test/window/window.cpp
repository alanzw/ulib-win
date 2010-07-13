#include "hwApp.h"

class MyApp : public hwAppGUI
{
public:
    bool onInit()
    {
        ::MessageBox(NULL, "init", "info", MB_OK|MB_ICONINFORMATION);
        return hwAppGUI::onInit();
    }

    bool onRun()
    {
        ::MessageBox(NULL, "run", "info", MB_OK|MB_ICONINFORMATION);
        return true;
    }

    bool onExit()
    {
        ::MessageBox(NULL, "exit", "info", MB_OK|MB_ICONINFORMATION);
        return hwAppGUI::onExit();
    }
};

IMPLEMENT_HWAPP(MyApp)
