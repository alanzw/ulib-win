#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "umsg.h"

int _tmain(int argc, TCHAR* argv[])
{
    // Retrieve current user name
    TCHAR szUserName[ 128 ];
    DWORD cchUserName = 128;
    GetUserName( szUserName, &cchUserName );

    TCHAR szBuffer[ 512 ];
    DWORD cchBuffer = 512;


// Part 1: Using FormatMessage() to retrieve message resources

    // Change threads locale explicitly to English
    SetThreadLocale( MAKELCID( MAKELANGID( 0x0409, SUBLANG_NEUTRAL ), SORT_DEFAULT ) );

    // Load first message
    LoadMessage( IDS_HELLO, szBuffer, cchBuffer);
    _tprintf( szBuffer );

    // Load second message with insertion parameter
    LoadMessage( IDS_GREETING, szBuffer, cchBuffer, szUserName );
    _tprintf( szBuffer );

    LoadMessage( IDS_GREETING, szBuffer, cchBuffer, szUserName );
    _tprintf( szBuffer );

    // Change threads locale to explicitly to German
    SetThreadLocale( MAKELCID( MAKELANGID( 0x0407, SUBLANG_NEUTRAL ), SORT_DEFAULT ) );
    LoadMessage( IDS_GREETING, szBuffer, cchBuffer, szUserName );
    _tprintf( szBuffer );

    // Change threads locale to explicitly to German
    SetThreadLocale( MAKELCID( MAKELANGID( 0x0804, SUBLANG_NEUTRAL ), SORT_DEFAULT ) );
    LoadMessage( IDS_GREETING, szBuffer, cchBuffer, szUserName );
    _tprintf( szBuffer );
    showMsg(szBuffer);
    return 0;

}

