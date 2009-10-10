#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "resource.h"

#include <winsock2.h>
#include <iphlpapi.h>

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "uiphlp.h"


int main(int argc, char *argv[])
{
    UIPHelp uih;

    uih.getNetworkInfo();

    printf( "Host Name: %s\n", uih.hostname() );
    printf( "Domain Name: %s\n", uih.domainName() );

    printf( "DNS Servers:\n" );
    printf( "\t%s\n", uih.dnsServer());
    printf( "\t%s\n", uih.dnsServerExtra());

    return 0;
}

