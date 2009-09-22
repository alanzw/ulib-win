#include <windows.h>
#include <tchar.h>

#include <cassert>

#include "uevent.h"

UEvent::UEvent(LPCTSTR sName)
: UKernelObject()
{
    assert(NULL != sName && lstrlen(sName) < MAX_PATH);
    lstrcpy(m_sName, sName);
}

bool UEvent::create()
{
    m_hObj = ::CreateEvent(NULL , false , false , m_sName);
    
    return (NULL != m_hObj);
}

bool UEvent::open()
{
    m_hObj = ::OpenEvent(EVENT_ALL_ACCESS, false, m_sName);
    
    return (NULL != m_hObj);
}

bool UEvent::set()
{
    return ::SetEvent(m_hObj);
}

bool UEvent::reset()
{
    return ::ResetEvent(m_hObj);
}

