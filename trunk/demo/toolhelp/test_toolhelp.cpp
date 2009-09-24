#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "utoolhelp.h"

int main(int argc, char *argv[])
{
    UToolHelp uth; 
    uth.listProcesses();
    //uth.listProcessModules(::GetCurrentProcessId());    
   
    getchar();
    return 0;
}
