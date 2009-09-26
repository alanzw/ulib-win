#include "resource.h"
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include "uplugin.h"
#include "udllman.h"

char *lpsz_plugins[] =
{
    "plugin1.dll",
    "plugin2.dll",
    0
};

int main(int argc, char *argv[])
{
    using std::cout;
    using std::endl;
    using std::cin;
    //
    int i = 0;
    char *pz = lpsz_plugins[0];
    while (pz != 0 )
    {
        cout << i << endl;

        UDllMan udm;
        udm.load(pz);
        IPlugin *p = udm.callFunc<IPlugin *>("retrieveIPP");
        p->go();
        udm.callFunc<void>("freeIPP");
        ++i;
        pz = lpsz_plugins[i];

    }
    cin.ignore();
    return 0;
}

