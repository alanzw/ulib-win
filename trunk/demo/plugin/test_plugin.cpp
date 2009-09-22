#include "resource.h"
#include <windows.h>
#include <iostream>

using namespace std;

#include "uplugin.h"

char *lpsz_plugins[] =
{
    "plugin1.dll",
    "plugin2.dll",
    0
};

int main(int argc, char *argv[])
{
    //
    int i = 0;
    char *pz = lpsz_plugins[0];
    while (pz != 0 )
    {
        cout << i << endl;
        HMODULE hDll = ::LoadLibrary(pz);

        if (!hDll)
        {
            cout << "Error in loading library: " << pz << endl;
            return 2;
        }

        LPRetrieveIP pfun;


        pfun = (LPRetrieveIP)GetProcAddress(hDll, "retrieveIPP");

        if (!pfun)
        {
            cout << "Error in GetProcAddress: " << "retrieveIPP" << endl;
            ::FreeLibrary(hDll);
            return 1;
        }

        IPlugin *p = pfun();

        p->go();

        delete p;
        ::FreeLibrary(hDll);

        i++;
        pz = lpsz_plugins[i];

    }
    cin.ignore();
    return 0;
}

