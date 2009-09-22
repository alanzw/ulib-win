#include <windows.h>
#include <tchar.h>

#include "ulayout.h"

namespace huys
{

namespace layout
{

ULayout::ULayout()
{
}

ULayout::~ULayout()
{
}

BOOL ULayout::addControl(UControl *pCtrl)
{
    m_ctrls.push_back(pCtrl);
    return TRUE;
}

UHBoxLayout::UHBoxLayout()
{
}

UHBoxLayout::~UHBoxLayout()
{}

BOOL UHBoxLayout::go()
{ 
    int w = m_rc.right - m_rc.left;
    int h = m_rc.bottom - m_rc.top;
    
    int n = m_ctrls.size();
    
    if (0 == n)
    {
        return FALSE;
    }
    
    int dx = w/n;
    
    RECT rc;
    rc.left = m_rc.left;
    rc.right = rc.left + dx;
    
    rc.top = m_rc.top;
    rc.bottom = rc.top + h;
    
    for (int i=0; i<n; ++i)
    {
        m_ctrls[i]->setPosition(&rc);
        rc.left += dx;
        rc.right = rc.left + dx;
    }
    
    return TRUE;
}

UVBoxLayout::UVBoxLayout()
{
}

UVBoxLayout::~UVBoxLayout()
{
}

BOOL UVBoxLayout::go()
{
    int w = m_rc.right - m_rc.left;
    int h = m_rc.bottom - m_rc.top;
    
    int n = m_ctrls.size();
    
    if (0 == n)
    {
        return FALSE;
    }
    
    int dy = h/n;
    
    RECT rc;
    rc.left = m_rc.left;
    rc.right = m_rc.right;
    
    rc.top = m_rc.top;
    rc.bottom = rc.top + dy;
    
    for (int i=0; i<n; ++i)
    {
        m_ctrls[i]->setPosition(&rc);
        rc.top += dy;
        rc.bottom = rc.top + dy;
    }

    return TRUE;
}

UGridLayout::UGridLayout(int nx /*= 2*/, int ny /*= 2*/)
: nXNum(nx), nYNum(ny)
{
}

UGridLayout::~UGridLayout()
{
}

BOOL UGridLayout::go()
{
    return TRUE;
}

}; // namespace layout


}; // namespace huys
