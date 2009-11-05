#include "resource.h"

#include <windows.h>
#include <iostream>

using namespace std;

#include "usys.h"

int main(int argc, char *argv[])
{
    USystem usys;

    cout << "Screen Width: " << usys.getScreenWidth()  << " pixels"  << endl;
    cout << "Screen Height: " << usys.getScreenHeight() << " pixels" << endl;

    //
    cout << "Minimized window arranged: " << usys.getArrange() << endl;
    cout << "Arrange mode: "  << endl;
    cout << "    " << usys.getArrangeText() << endl;
    //
    cout << "Boot Mode: " << usys.getBootMode() << " -- " << usys.getBootModeText() << endl;

    //
    cout << "Monitor Number: " << usys.getMonitorNum() << endl;
    //
    cout << "Mouse Number: " << usys.getMouseButtonNum() << endl;

    //
    cout << "Computer Name: " << usys.getComputerName() << endl;

    //
    cout << "Press any key to exit..." << endl;

    cin.ignore();

    return 0;
}


