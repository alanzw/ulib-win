#include <windows.h>
#include <tchar.h>

#include "ubrush.h"

UBrush::UBrush()
{
}

UBrush::~UBrush()
{}

HBRUSH UBrush::createSolidBrush(huys::Color clr)
{
    return (HBRUSH)(m_hObj = ::CreateSolidBrush(clr));
}

