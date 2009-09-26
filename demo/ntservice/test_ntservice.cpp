#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "untservice.h"

int main(int argc, char *argv[])
{

    UNTServiceMan usvcman;

    //usvcman.installSvc("HelloSVC");
    usvcman.deleteSvc("HelloSVC");
    return 0;
}


