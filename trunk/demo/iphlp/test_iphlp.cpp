#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <iphlpapi.h>

#include "uiphlp.h"
#include "uconsole.h"

int main(int argc, char *argv[])
{
    UIPHelp uih;

    uih.getNetworkInfo();

    UConsole::PrintStdoutFormat( "Host Name: %s\n", uih.hostname() );
    UConsole::PrintStdoutFormat( "Domain Name: %s\n", uih.domainName() );

    UConsole::PrintStdoutFormat( "DNS Servers:\n" );
    UConsole::PrintStdoutFormat( "\t%s\n", uih.dnsServer());
    UConsole::PrintStdoutFormat( "\t%s\n", uih.dnsServerExtra());

    return 0;
}

