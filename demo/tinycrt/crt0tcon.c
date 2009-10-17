//==========================================
// TINYCRT - Matt Pietrek 1996
// Microsoft Systems Journal, October 1996
// FILE: CRT0TCON.C
//==========================================
#include <windows.h>
#include "argcargv.h"

#ifndef __BORLANDC__
// Force the linker to include KERNEL32.LIB
#pragma comment(linker, "/defaultlib:kernel32.lib")
#endif

int __cdecl main(int, char **, char **);    // In user supplied code

void __cdecl mainCRTStartup( void )
{
    int mainret, argc;

    argc = _ConvertCommandLineToArgcArgv( );

    mainret = main( argc, _ppszArgv, 0 );

    ExitProcess(mainret);
}
