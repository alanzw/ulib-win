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

