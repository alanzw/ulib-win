/*
 * =====================================================================================
 *
 *       Filename:  ufile.cpp
 *
 *    Description:  implement of UFile class
 *
 *        Version:  1.0
 *        Created:  2009-7-22 20:29:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <cstdio>
#include "ufile.h"

UFile::UFile()
: UKernelObject()
{
    ::ZeroMemory(m_sFilename, sizeof(m_sFilename));
    m_hObj = INVALID_HANDLE_VALUE;
}

UFile::~UFile()
{
    if (isOpen())
    {
        close();
    }

}

bool UFile::isOpen() const
{
    return (m_hObj != INVALID_HANDLE_VALUE && m_hObj != 0 );
}

bool UFile::open( LPCSTR lpFilename /*= _T("defalut")*/ )
{
    assert( !::IsBadStringPtr(lpFilename, -1) );
    lstrcpy(m_sFilename, lpFilename);
    m_hObj = ::CreateFile(
            m_sFilename,
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    return ( INVALID_HANDLE_VALUE != m_hObj);
}

bool UFile::create( LPCTSTR lpFilename /*= _T("default")*/ )
{
    assert( !::IsBadStringPtr(lpFilename, -1) );
    lstrcpy(m_sFilename, lpFilename);
    m_hObj = ::CreateFile(
        m_sFilename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    return ( INVALID_HANDLE_VALUE != m_hObj);
}

bool UFile::write( LPBYTE lpData, DWORD dwNumOfBytes )
{
    assert( INVALID_HANDLE_VALUE != m_hObj  );
    assert( NULL != lpData );
    assert( !::IsBadStringPtr((LPTSTR)lpData, dwNumOfBytes) );

    if( dwNumOfBytes == 0 ) return TRUE; // avoid Win32 "null-write" option

    DWORD dwNumOfBytesWriten = 0;
    BOOL bOk = TRUE;
    while (dwNumOfBytesWriten < dwNumOfBytes)
    {
        bOk = ::WriteFile( m_hObj,
                           lpData+dwNumOfBytesWriten,
                           dwNumOfBytes-dwNumOfBytesWriten,
                           &dwNumOfBytesWriten,
                           NULL);
        if (!bOk)
        {
            return false;
        }
    }
    return true;
}

bool UFile::read( LPBYTE lpBuffer, DWORD dwBufSize, LPDWORD dwNumOfBytesRead )
{
    BOOL bOk = TRUE;

    bOk = ::ReadFile(m_hObj, lpBuffer, dwBufSize, dwNumOfBytesRead, NULL);

    if (!bOk)
    {
        return false;
    }
    return true;
}

DWORD UFile::seek(LONG lOffset, DWORD dwFrom)
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return ::SetFilePointer(m_hObj, lOffset, NULL, dwFrom);
}

DWORD UFile::pos()
{
    return this->seek(0, FILE_CURRENT);
}

bool UFile::lock(DWORD dwOffset, DWORD dwSize)
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return ::LockFile(m_hObj, dwOffset, 0, dwSize, 0);
}

bool UFile::unlock(DWORD dwOffset, DWORD dwSize)
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return ::UnlockFile(m_hObj, dwOffset, 0, dwSize, 0);
}

bool UFile::flush()
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return ::FlushFileBuffers(m_hObj);
}

DWORD UFile::size() const
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return ::GetFileSize(m_hObj, NULL);
}

DWORD UFile::type() const
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return ::GetFileType(m_hObj);
}

