#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <cpl.h>

#include "uconsole.h"
#include "udllman.h"

using namespace UConsole;

int main(int argc, char *argv[])
{
    union {
        NEWCPLINFOA NewCplInfoA;
        NEWCPLINFOW NewCplInfoW;
    } Newcpl;

    //HINSTANCE hLib; // Library Handle to *.cpl file
    //APPLET_PROC CplCall; // Pointer to CPlApplet() function
    //LONG i;

    // -------------------
    //if (!(hLib = LoadLibrary(argv[1])))
    //    return 1;

    //if (!(CplCall=(APPLET_PROC)GetProcAddress(hLib,"CPlApplet")))
    //{
    //    FreeLibrary(hLib);
    //    return 2;
    //}

    UDllMan udm(argv[1]);
    APPLET_PROC CplCall = (APPLET_PROC)udm.find("CPlApplet");
    // -------------------
    CplCall(NULL, CPL_INIT, 0, 0); // Init the *.cpl file

    for (LONG i=0; i<CplCall(NULL,CPL_GETCOUNT,0,0); i++)
    {
        //printf("Control %s",argv[1]);
        PrintStdoutFormat("Control %s", argv[1]);
        Newcpl.NewCplInfoA.dwSize = 0;
        Newcpl.NewCplInfoA.dwFlags = 0;
        CplCall(NULL,CPL_NEWINQUIRE,i,(long)&Newcpl);

        if (Newcpl.NewCplInfoA.dwSize == sizeof(NEWCPLINFOW))
        {   // Case #1, CPL_NEWINQUIRE has returned an Unicode String
            //wprintf(L",%s\n", Newcpl.NewCplInfoW.szName);
            PrintStdoutFormat(_T(",%s\n"), Newcpl.NewCplInfoW.szName);
        }
        else
        {   // Case #2, CPL_NEWINQUIRE has returned an ANSI String
            if (Newcpl.NewCplInfoA.dwSize != sizeof(NEWCPLINFOA))
            {
                // Case #3, CPL_NEWINQUIRE failed to return a string
                //    Get the string from the *.cpl Resource instead
                CPLINFO CInfo;

                CplCall(NULL,CPL_INQUIRE,i,(long)&CInfo);
                LoadStringA(udm, CInfo.idName,
                    Newcpl.NewCplInfoA.szName,32);
            }
            //printf(",%s\n", Newcpl.NewCplInfoA.szName);
            PrintStdoutFormat(",%s\n", Newcpl.NewCplInfoW.szName);
        }
    } // for

    CplCall(NULL,CPL_EXIT,0,0);

    // -------------------
    //FreeLibrary(hLib);
    return 0;

    return 0;
}


