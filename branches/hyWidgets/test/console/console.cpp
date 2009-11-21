#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include "hwApp.h"

extern hwAppConsole * hwCreateApp();

int hwMain(int argc, hwCharPtrPtr argv)
{
    hwAppConsole *app = hwCreateApp();

    if (!app->init(argc, argv))
    {
        return -1;
    }

    if (!app->onRun())
    {
        return -2;
    }

    if (!app->onCleanup())
    {
        return -3;
    }

    if (!app->onExit())
    {
        return -4;
    }

    return 0;
}

class MyApp : public hwAppConsole
{
public:
    bool onInit()
    {
        cout << "----Init MyApp" << endl;
        return hwAppConsole::onInit();
    }

    bool onRun()
    {
        cout << "----Run  MyApp" << endl;
        return true;
    }

    bool onExit()
    {
        cout << "----Exit MyApp" << endl;
        cin.ignore();
        return hwAppConsole::onExit();
    }
};

IMPLEMENT_HWAPP(MyApp)

int hwMain(int argc, hwCharPtrPtr argv);

extern "C" int _tmain(int argc, TCHAR **argv)
{
    return hwMain(argc, argv);
}
