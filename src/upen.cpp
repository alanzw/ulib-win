#include <windows.h>
#include <tchar.h>

#include "upen.h"

UPen::UPen()
{
}

UPen::~UPen()
{}

HPEN UPen::createSolidBrush(huys::Color clr)
{
    return (HPEN)(m_hObj = ::CreatePen(PS_SOLID, 1, clr));
}

HPEN UPen::getStockPen( int fnObject )
{
    return (HPEN)(m_hObj = ::GetStockObject(fnObject));
}

