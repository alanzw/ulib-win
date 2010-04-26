#include "hwApp.h"


class MyApp : public hwAppGUI
{
public:
    bool onInit()
    {
        return hwAppGUI::onInit();
    }

    bool onRun()
    {
        return true;
    }

    bool onExit()
    {
        return hwAppGUI::onExit();
    }
};

IMPLEMENT_HWAPP(MyApp)

extern "C" int _tmain(int argc, TCHAR **argv)
{
    return 0;
}

