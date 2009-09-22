#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "resource.h"

#include <winsock2.h>
#include <iphlpapi.h>

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

int _tmain(int argc, char *argv[])
{

    FIXED_INFO  *pFixedInfo;
    ULONG       ulOutBufLen;
    DWORD       dwRetVal;
    IP_ADDR_STRING *pIPAddr;

    pFixedInfo = (FIXED_INFO *) malloc( sizeof( FIXED_INFO ) );
    ulOutBufLen = sizeof( FIXED_INFO );

    // Make an initial call to GetNetworkParams to get the size required for the
    // ulOutBufLen variable.
    if ( GetNetworkParams( pFixedInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW ) {
        free( pFixedInfo );
        pFixedInfo = (FIXED_INFO *) malloc( ulOutBufLen );
    }

    // Make a second call to GetNetworkParams using general error checking and
    // returning its value to the DWORD variable dwRetVal; used for more
    // advanced error checking.
    if ( dwRetVal = GetNetworkParams( pFixedInfo, &ulOutBufLen ) != NO_ERROR) {
        printf( "GetNetworkParams call failed with %d", dwRetVal);
    }

    printf( "Host Name: %s\n", pFixedInfo -> HostName );
    printf( "Domain Name: %s\n", pFixedInfo -> DomainName );

    printf( "DNS Servers:\n" );
    printf( "\t%s\n", pFixedInfo -> DnsServerList.IpAddress.String );

    pIPAddr = pFixedInfo -> DnsServerList.Next;
    while ( pIPAddr ) {
        printf( "\t%s\n", pIPAddr -> IpAddress.String );
        pIPAddr = pIPAddr -> Next;
    }

    if (pFixedInfo) {
        free(pFixedInfo);
    }

    return 0;
}

