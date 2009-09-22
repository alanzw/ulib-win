#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "urt.h"

int main(int argc, char *argv[])
{
    std::cout << "OSVer: " << getOSVer() << std::endl;

    std::cout << "Major: " << getWinMajor() << std::endl;

    std::cout << "Minor: " << getWinMinor() << std::endl;

    std::cout << "WinVer: " << getWinVer() << std::endl;


    int myArgc = getArgc();
    TCHAR ** myArgv = getArgv();

    std::cout << "Argument Number: " << myArgc << std::endl;
    for (int i=0; i<myArgc; i++)
    {

        std::cout << i << "  " << myArgv[i] << std::endl;
    }


    std::cout << "CommandLine: " << ::GetCommandLine() << std::endl;

    std::cout << "Press any key to continue...";

    std::cin.ignore();

    return 0;
}
