/*
 * =====================================================================================
 *
 *       Filename:  ufilemapping.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009-9-25 0:12:06
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

#include "ufilemapping.h"

UFileMapping::UFileMapping()
: m_lpBuffer(0)
{}

UFileMapping::~UFileMapping()
{}

BOOL UFileMapping::create(LPCTSTR sMapFilename)
{
    lstrcpy(m_sMapFilename, sMapFilename);
    m_hObj = ::CreateFileMapping(
        INVALID_HANDLE_VALUE,   // Use paging file instead of existing file.
        NULL,                   // Default security
        PAGE_READWRITE,         // Read/write access
        0,                      // Max. object size
        UFileMapping::BUFFER_SIZE,    // Buffer size
        sMapFilename
        );
    return (NULL != m_hObj);
}

BOOL UFileMapping::open(LPCTSTR sMapFilename)
{
    lstrcpy(m_sMapFilename, sMapFilename);
    m_hObj = ::OpenFileMapping(
        FILE_MAP_ALL_ACCESS,    // Read/write access
        FALSE,                  // Do not inherit the name
        sMapFilename           // Name of mapping object
        );
    return (NULL != m_hObj);
}

BOOL UFileMapping::mapViewOfFile()
{
    m_lpBuffer = (LPSTR)::MapViewOfFile(
        m_hObj,
        FILE_MAP_ALL_ACCESS,    // Read/write permission
        0,                      // A high-order DWORD of the file offset
        0,                      // A low-order DWORD of the file offset
        BUFFER_SIZE             // The number of bytes of a file mapping
        );
    return (NULL != m_lpBuffer);
}

BOOL UFileMapping::unmapViewOfFile()
{
    return ::UnmapViewOfFile(m_lpBuffer);
}

BOOL UFileMapping::write(LPCTSTR lpText, int cbBytes)
{
    if (!m_lpBuffer)
    {
        return FALSE;
    }

    ::CopyMemory(m_lpBuffer, lpText, cbBytes);

    return TRUE;
}


