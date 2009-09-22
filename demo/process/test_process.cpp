#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <psapi.h>

#include <stdio.h>

#include "uprocessman.h"

int _tmain(int argc, char *argv[])
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return 1;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
        if( aProcesses[i] != 0 )
            printProcessName( aProcesses[i] );

    getchar();
    return 0;
}


