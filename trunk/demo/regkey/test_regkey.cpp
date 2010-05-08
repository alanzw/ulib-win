#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

using namespace std;

#include "uregkey.h"
#include "umsg.h"

int main(int argc, char *argv[])
{
    char subkey[] = "XML\\CLSID";

    URegKey urk;

    if (urk.openClassRoot(subkey) == ERROR_SUCCESS)
    {
        cout << "OK!" << endl;
    }
    else
    {
        cout << "Failed!" << endl;
    }

    char buf[256];
    long size;
    if (urk.queryValue(buf, size) == ERROR_SUCCESS)
    {
        cout << buf << endl;
        if (!urk.saveKey("new.reg"))
        {
            showError("RegSaveKey");
        }
    }

    cin.ignore();
    return 0;
}


