#include <windows.h>

#include "utime.h"

UTime::UTime()
{}

UTime::~UTime()
{}

void UTime::getSysTime()
{
    ::GetSystemTime(&m_st);
}

void UTime::getLocalTime()
{
    ::GetLocalTime(&m_st);
}
