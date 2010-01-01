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
#include <cstdio>
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

BOOL UMailSlot::open(LPCTSTR sMailSlotName)
{
    m_hObj = CreateFile(
        sMailSlotName,            // The name of the mailslot
        GENERIC_WRITE,                // Write access (Mailslot is a mechanism
                                    // for one-way IPC. The client is just
                                    // responsible for writing to the slot)
        FILE_SHARE_READ,            // Share mode
        NULL,                        // Default security attributes
        OPEN_EXISTING,                // Opens existing mailslot
        FILE_ATTRIBUTE_NORMAL,        // The file has no other attributes set
        NULL);                        // No template file

    return (INVALID_HANDLE_VALUE != m_hObj);
}

BOOL UMailSlot::write(LPCTSTR sMessage)
{
    DWORD cbWritten;

    if (!WriteFile(m_hObj, sMessage, (DWORD)lstrlen(sMessage)+1, &cbWritten, NULL))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL UMailSlot::read(LPTSTR sMessage)
{
    if (m_hObj == INVALID_HANDLE_VALUE)
        return FALSE;

    DWORD cbMessageBytes = 0;        // Size of the message in bytes
    DWORD cbBytesRead = 0;            // Number of bytes read from the slot
    DWORD cMessages = 0;            // Number of messages in the mailslot
    DWORD nMessageId = 0;            // Message ID

    BOOL bResult;

    LPTSTR lpszBuffer;                // A buffer used to store one message


    /////////////////////////////////////////////////////////////////////////
    // Check for the number of messages in the mailslot.
    //

    bResult = GetMailslotInfo(
        m_hObj,                    // Handle of the mailslot
        NULL,                        // No maximum message size
        &cbMessageBytes,            // Size of next message
        &cMessages,                    // Number of messages
        NULL);                        // No read time-out

    if (!bResult)
    {
        _tprintf(_T("GetMailslotInfo failed w/err 0x%08lx\n"), GetLastError());
        return FALSE;
    }

    if (cbMessageBytes == MAILSLOT_NO_MESSAGE)
    {
        // There are no new messages in the mailslot at present
        _putts(_T("No new messages."));
        return TRUE;
    }


    /////////////////////////////////////////////////////////////////////////
    // Retrieve the messages one by one from the mailslot.
    //

    while (cMessages != 0) // If there are still un-read messages in the slot
    {
        nMessageId++;

        // Allocate the memory for the message based on its size info,
        // cbMessageBytes, which was retrieved from GetMailslotInfo.

        lpszBuffer = (LPTSTR) GlobalAlloc(GPTR, cbMessageBytes);
        if (NULL == lpszBuffer)
            return FALSE;

        bResult = ReadFile(            // Read from the mailslot.
            m_hObj,                // Handle of the slot
            lpszBuffer,                // Buffer to receive data
            cbMessageBytes,            // Size of buffer in bytes
            &cbBytesRead,            // Number of bytes read
            NULL);                    // Not overlapped I/O

        if (!bResult)
        {
            _tprintf(_T("ReadFile failed w/err 0x%08lx\n"), GetLastError());
            GlobalFree((HGLOBAL)lpszBuffer);
            return FALSE;
        }

        // Display the message.
        _tprintf(_T("Message #%ld: %s\n"), nMessageId, lpszBuffer);

        GlobalFree((HGLOBAL) lpszBuffer); // Free the buffer

        // Get the current number of un-read messages in the slot. The number
        // may not equal the initial message number because new messages may
        // arrive while we are reading the items in the slot.

        bResult = GetMailslotInfo(
            m_hObj,                // Handle of the mailslot
            NULL,                    // No maximum message size
            &cbMessageBytes,        // Size of next message
            &cMessages,                // Number of messages
            NULL);                    // No read time-out

        if (!bResult)
        {
            _tprintf(_T("GetMailslotInfo failed w/err 0x%08lx\n"), GetLastError());
            return FALSE;
        }
    }
    return TRUE;
}
