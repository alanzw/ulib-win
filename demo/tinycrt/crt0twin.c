//==========================================
// TINYCRT - Matt Pietrek 1996
// Microsoft Systems Journal, October 1996
// FILE: CRT0TWIN.C
//==========================================
#include <windows.h>

#ifndef __BORLANDC__
// Force the linker to include KERNEL32.LIB
#pragma comment(linker, "/defaultlib:kernel32.lib")
#endif

//
// Modified version of the Visual C++ 4.1 startup code.  Simplified to
// make it easier to read.  Only supports ANSI programs.
//

void __cdecl WinMainCRTStartup( void )

{
    int mainret;
    char *lpszCommandLine;
    STARTUPINFO StartupInfo;

    lpszCommandLine = GetCommandLine();

    // Skip past program name (first token in command line).

    if ( *lpszCommandLine == '"' )  //Check for and handle quoted program name
    {
        // Scan, and skip over, subsequent characters until  another
        // double-quote or a null is encountered

        while( *lpszCommandLine && (*lpszCommandLine != '"') )
        lpszCommandLine++;

        // If we stopped on a double-quote (usual case), skip over it.

        if ( *lpszCommandLine == '"' )
        lpszCommandLine++;
    }
    else    // First token wasn't a quote
    {
        while ( *lpszCommandLine > ' ' )
        lpszCommandLine++;
    }

    // Skip past any white space preceeding the second token.

    while ( *lpszCommandLine && (*lpszCommandLine <= ' ') )
    lpszCommandLine++;

    StartupInfo.dwFlags = 0;
    GetStartupInfo( &StartupInfo );

    mainret = WinMain( GetModuleHandle(NULL),
    NULL,
    lpszCommandLine,
    StartupInfo.dwFlags & STARTF_USESHOWWINDOW
    ? StartupInfo.wShowWindow : SW_SHOWDEFAULT );

    ExitProcess(mainret);
}
