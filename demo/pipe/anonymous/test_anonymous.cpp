#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#include "upipe.h"

int main(int argc, char *argv[])
{

    UPipe up;

    up.create();

    up.startChild();

    up.read();

    return 0;
}


