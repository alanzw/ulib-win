#include "resource.h"

#include <windows.h>

#include <iostream>
#include <string>

using namespace std;

#include "uregkey.h"

int main(int argc, char *argv[])
{
    char subkey[] = "SOFTWARE\\Microsoft";

    URegKey urk;

    if (urk.openClassRoot(subkey) == ERROR_SUCCESS)
    {
        cout << "OK!" << endl;
    }
    else
    {
        cout << "Failed!" << endl;
    }

    cin.ignore();
    return 0;
}


