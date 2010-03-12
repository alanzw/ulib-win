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
    return TRUE == ::LockFile(m_hObj, dwOffset, 0, dwSize, 0);
}

bool UFile::unlock(DWORD dwOffset, DWORD dwSize)
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return TRUE == ::UnlockFile(m_hObj, dwOffset, 0, dwSize, 0);
}

bool UFile::flush()
{
    assert( INVALID_HANDLE_VALUE != m_hObj );
    return TRUE == ::FlushFileBuffers(m_hObj);
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

UCFile::UCFile(const char *sFilename)
: m_pFile(NULL)
{
    strcpy(m_sFilename, sFilename);
}

UCFile::~UCFile()
{
    if (m_pFile)
    {
        close();
    }
}

//
bool UCFile::create(bool bOverwriten /*= true*/)
{
    const char *sMode = (bOverwriten ? "w":"w+");
    m_pFile = fopen(m_sFilename, sMode);
    return true;
}
//
bool UCFile::open(const char *mode)
{
    m_pFile = fopen(m_sFilename, mode);
    return true;
}
bool UCFile::close()
{
    fclose(m_pFile);
    m_pFile = NULL;
    return true;
}

bool UCFile::reopen(const char *mode)
{
    freopen(m_sFilename, mode, m_pFile);
    return true;
}

bool UCFile::isOpen() const
{
    return (NULL == m_pFile);
}

bool UCFile::isEOF() const
{
    return 0 != feof(m_pFile);
}

bool UCFile::read(void *buf, size_t size)
{
    return 1 == fread(buf, size, 1, m_pFile);
}
bool UCFile::write(const void *buf, size_t size)
{
    return 1 == fwrite(buf, size, 1, m_pFile);
}

size_t UCFile::size()
{
    size_t s = 0;
    this->seek(0, SEEK_END);
    s = this->tell();
    this->rewind();
    return s;
}

bool UCFile::directStdOut(const char *mode)
{
    m_pFile = freopen(m_sFilename, mode, stdout);
    return true;
}

bool UCFile::directStdErr(const char *mode)
{
    m_pFile = freopen(m_sFilename, mode, stderr);
    return true;
}

bool UCFile::directStdIn(const char *mode)
{
    m_pFile = freopen(m_sFilename, mode, stdin);
    return true;
}

bool UCFile::flush()
{
    fflush(m_pFile);
    return true;
}

bool UCFile::seek(long int offset, int origin)
{
    return (0 != fseek(m_pFile, offset, origin) );
}

long UCFile::tell()
{
    return ftell(m_pFile);
}

void UCFile::rewind()
{
    ::rewind(m_pFile);
}
