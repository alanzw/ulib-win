#include <windows.h>

#include "ucritical.h"

UCriticalSection::UCriticalSection()
{
    ::InitializeCriticalSection(&m_cs);
}

UCriticalSection::~UCriticalSection()
{
    ::DeleteCriticalSection(&m_cs);
}

bool UCriticalSection::lock()
{
    ::EnterCriticalSection(&m_cs);
}

bool UCriticalSection::unlock()
{
    ::LeaveCriticalSection(&m_cs); 
}
