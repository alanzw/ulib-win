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

#include <stdlib.h>

#include "uiphlp.h"

UIPHelp::UIPHelp()
: m_pFixedInfo(0),
  m_pIPAddr(0)
{
}

UIPHelp::~UIPHelp()
{

}

bool UIPHelp::getInfo()
{
    ULONG ulOutBufLen;
    DWORD dwRetVal;

    m_pFixedInfo = (FIXED_INFO *) malloc( sizeof( FIXED_INFO ) );
    ulOutBufLen = sizeof( FIXED_INFO );

    // Make an initial call to GetNetworkParams to get the size required for the
    // ulOutBufLen variable.
    if ( GetNetworkParams( m_pFixedInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW ) {
        free( m_pFixedInfo );
        m_pFixedInfo = (FIXED_INFO *) malloc( ulOutBufLen );
    }

    // Make a second call to GetNetworkParams using general error checking and
    // returning its value to the DWORD variable dwRetVal; used for more
    // advanced error checking.
    if ( (dwRetVal = GetNetworkParams( m_pFixedInfo, &ulOutBufLen )) != NO_ERROR) {
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

