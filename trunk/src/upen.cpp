#include <windows.h>
#include <tchar.h>

#include "colors.h"

#include "upen.h"

UPen::UPen()
{
}

UPen::~UPen()
{}

HPEN UPen::createSolidPen(huys::Color clr)
{
    return (HPEN)(m_hObj = ::CreatePen(PS_SOLID, 1, clr));
}

HPEN UPen::getStockPen( int fnObject )
{
    return (HPEN)(m_hObj = ::GetStockObject(fnObject));
}

HPEN UPen::createPen( int nStyle, int nWidth, huys::Color clr )
{
    return (HPEN)(m_hObj = ::CreatePen(nStyle, nWidth, clr));  
}

