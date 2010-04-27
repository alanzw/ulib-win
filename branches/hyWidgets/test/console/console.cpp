#include "hwApp.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

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
