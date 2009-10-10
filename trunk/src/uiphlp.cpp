/*
 * =====================================================================================
 *
 *       Filename:  uiphlp.cpp
 *
 *    Description:  implement of Class UIPHelp
 *
 *        Version:  1.0
 *        Created:  2009-6-27 16:16:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <tchar.h>
#include <winsock2.h>

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
  #include "fix/iphlpapi.h"
  #include "fix/icmpapi.h"
#else
  #include <iphlpapi.h>
#ifndef   __GNUC__
  #include <icmpapi.h>
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "uiphlp.h"

UIPHelp::UIPHelp()
: m_pFixedInfo(0),
  m_pAdapterInfo(0),
  m_pInterfaceInfo(0),
  m_pIPAddrTable(0)
{}

UIPHelp::~UIPHelp()
{
    if (m_pFixedInfo)
    {
        free(m_pFixedInfo);
    }

    if (m_pAdapterInfo)
    {
        free(m_pAdapterInfo);
    }

    if (m_pInterfaceInfo)
    {
        free(m_pInterfaceInfo);
    }

    if (m_pInterfaceInfo)
    {
        free(m_pIPAddrTable);
    }
}

bool UIPHelp::getNetworkInfo()
{
    ULONG ulOutBufLen;
    DWORD dwRetVal;

    m_pFixedInfo = (FIXED_INFO *) malloc( sizeof( FIXED_INFO ) );
    ulOutBufLen = sizeof( FIXED_INFO );

    // Make an initial call to GetNetworkParams to get the size required for the
    // ulOutBufLen variable.
    if ( ::GetNetworkParams( m_pFixedInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW )
    {
        free( m_pFixedInfo );
        m_pFixedInfo = (FIXED_INFO *) malloc( ulOutBufLen );
    }

    // Make a second call to GetNetworkParams using general error checking and
    // returning its value to the DWORD variable dwRetVal; used for more
    // advanced error checking.
    if ( (dwRetVal = ::GetNetworkParams( m_pFixedInfo, &ulOutBufLen )) != NO_ERROR)
    {
        return false;
    }

    return true;
}

const char * UIPHelp::hostname() const
{
    return m_pFixedInfo->HostName;
}

const char * UIPHelp::domainName() const
{
    return m_pFixedInfo->DomainName;
}

const char * UIPHelp::dnsServer() const
{
    return m_pFixedInfo->DnsServerList.IpAddress.String;
}

const char * UIPHelp::dnsServerExtra() const
{
    return m_pFixedInfo->DnsServerList.Next->IpAddress.String;
}

bool UIPHelp::getAdapterInfo()
{
    ULONG ulOutBufLen;
    DWORD dwRetVal;

    m_pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    if (::GetAdaptersInfo(m_pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
    {
        free(m_pAdapterInfo);
    }
    m_pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);

    if ((dwRetVal = ::GetAdaptersInfo(m_pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS)
    {
        printf("GetAdaptersInfo call failed with %d\n", dwRetVal);
        return false;
    }
    return true;
}

bool UIPHelp::getInterfaceInfo()
{
    ULONG ulOutBufLen;
    DWORD dwRetVal;

    m_pInterfaceInfo = (IP_INTERFACE_INFO *)malloc(sizeof(IP_INTERFACE_INFO));
    ulOutBufLen = sizeof(IP_INTERFACE_INFO);

    if ( GetInterfaceInfo(m_pInterfaceInfo, &ulOutBufLen ) == ERROR_INSUFFICIENT_BUFFER)
    {
        free(m_pInterfaceInfo);
        m_pInterfaceInfo = (IP_INTERFACE_INFO *) malloc (ulOutBufLen);
    }

    if ((dwRetVal = GetInterfaceInfo(m_pInterfaceInfo, &ulOutBufLen)) != NO_ERROR ) {
        printf("GetInterfaceInfo failed with error: %d\n", dwRetVal);
        return false;
    }
    
    return true;
}

bool UIPHelp::getIPAddressTable()
{
    DWORD dwSize = 0;
    DWORD dwRetVal;

    m_pIPAddrTable = (MIB_IPADDRTABLE*) malloc( sizeof(MIB_IPADDRTABLE) );

    if (GetIpAddrTable(m_pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
    {
        free( m_pIPAddrTable );
        m_pIPAddrTable = (MIB_IPADDRTABLE *) malloc ( dwSize );
    }

    if ( (dwRetVal = GetIpAddrTable( m_pIPAddrTable, &dwSize, 0 )) != NO_ERROR )
    { 
        printf("GetIpAddrTable call failed with %d\n", dwRetVal);
        return false;
    }

    return true;
}

bool UIPHelp::releaseIPAddress()
{
    assert(m_pInterfaceInfo);
    if ( IpReleaseAddress(&m_pInterfaceInfo->Adapter[0]) == NO_ERROR) {
        return true;
    }
    return false;
}

bool UIPHelp::renewIPAddress()
{
    assert(m_pInterfaceInfo);
    if (IpRenewAddress(&m_pInterfaceInfo->Adapter[0]) == NO_ERROR)
    {
        return true;
    }
    return false;
}

#ifdef   __GNUC__
HANDLE WINAPI IcmpCreateFile(VOID);
#endif

bool UIcmpUtility::createFile()
{
//    if (INVALID_HANDLE_VALUE == (m_hIcmpFile = ::IcmpCreateFile()))
    {
        return false;
    }
    return true;
}

UIcmpUtility::UIcmpUtility()
: m_hIcmpFile(0)
{}

