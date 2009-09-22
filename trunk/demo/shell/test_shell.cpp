#include "resource.h"
#include <windows.h>
#include <stdio.h>

#include "ushell.h"

int main()
{
    int nRet = UShell::execute(NULL, "notepad.exe");
    
    if (nRet < 32 )
    {
        fprintf(stderr, "Failed to execute the file.\n");
    }
    getchar();
    return 0;
}
