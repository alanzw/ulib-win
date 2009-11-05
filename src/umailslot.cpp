/*
 * =====================================================================================
 *
 *       Filename:  umailslot.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/01/2009 11:35:48 PM
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

#include "umailslot.h"

UMailSlot::UMailSlot()
{}

UMailSlot::~UMailSlot()
{}

BOOL UMailSlot::create(LPCTSTR sMailSlotName)
{
    m_hObj = ::CreateMailslot(
        sMailSlotName,
        0,
        MAILSLOT_WAIT_FOREVER,
        NULL
        );
    return (INVALID_HANDLE_VALUE != m_hObj);
}

BOOL UMailSlot::send(LPCTSTR sMessage)
{
    HANDLE hFile = NULL;
    DWORD cbWritten;
    
    hFile = ::CreateFile(
        m_sMailSlotName,
        GENERIC_WRITE,
        FILE_SHARD_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return FALSE;
    }
    
    if (!WriteFile(hFile, sMessage, (DWORD)lstrlen(sMessage)+1, &cbWritten, NULL))
    {
        ::CloseHandle(hFile);
        return FALSE;
    }
    
    ::CloseHandle(hFile);
    return TRUE;
}
