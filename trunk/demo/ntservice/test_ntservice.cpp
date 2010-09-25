#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "untservice.h"
#include "uconsole.h"


int main(int argc, char *argv[])
{

    UNTServiceMan usvcman;

    UConsole::PrintStdout("Install the Service: HelloSVC\n");
    usvcman.installSvc("HelloSVC");

    UConsole::PrintStdout("Delete the Service: HelloSVC!\n");
    usvcman.deleteSvc("HelloSVC");

    UConsole::PrintStdout("Start the Service: Spooler\n");
    usvcman.startSvc("Spooler");

    UConsole::PrintStdout("Stop the Service: Spooler!\n");
    usvcman.stopSvc("Spooler");

    return 0;
}


