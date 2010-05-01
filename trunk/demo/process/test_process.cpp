#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <psapi.h>

#include <stdio.h>

#include "uprocessman.h"
#include "uprocess.h"

void print_processes()
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
        if( aProcesses[i] != 0 )
            printProcessName( aProcesses[i] );

}

int _tmain(int argc, char *argv[])
{
    UProcess process;
    process.create("child.exe", 0);

    Sleep(1000);
    for (int i=0; i<10; ++i)
    {
        printf("!!!Here in Main!!!\n");
    }

    getchar();

    process.create("ping 127.0.0.0 -n 2");

    Sleep(1000);
    for (int i=0; i<10; ++i)
    {
        printf("!!!Here in Main!!!\n");
    }

    process.resumeAndWait();

    printf("Exit Code : %d\n", process.getExitCode());

    getchar();


    process.create("ping 127.0.0.1 -n 2");

    Sleep(1000);

    process.resumeAndWait();

    printf("Exit Code : %d\n", process.getExitCode());

    getchar();

    return 0;
}


