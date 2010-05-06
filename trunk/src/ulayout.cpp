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

ULayout::~ULayout() /// Implement must be supplied, or link error.
{}

BOOL ULayout::addControl(UControl *pCtrl)
{
    m_ctrls.push_back(pCtrl);
    return TRUE;
}

BOOL ULayout::addControls( UControl **p, int n )
{
    for (int i=0; i<n; ++i)
    {
        addControl(p[i]);
    }
    return TRUE;    
}

UHBoxLayout::UHBoxLayout()
{
}

UHBoxLayout::~UHBoxLayout()
{}

BOOL UHBoxLayout::go()
{
    UControlVec::size_type n = m_ctrls.size();

    if (0 == n)
    {
        return FALSE;
    }

    int dx = m_rect.width()/n;
    int h = m_rect.height();

    huys::URectL rect;
    rect.set(m_rect.left(), m_rect.top(), dx, h);

    for (UControlVec::size_type i=0; i<n; ++i)
    {
        m_ctrls[i]->setPosition(rect);
        rect.offset(dx, 0);
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
    int w = m_rect.width();
    int h = m_rect.height();

    UControlVec::size_type n = m_ctrls.size();

    if (0 == n)
    {
        return FALSE;
    }

    int dy = h/n;

    huys::URectL rect;
    rect.set(m_rect.left(), m_rect.top(), w, dy);

    for (UControlVec::size_type i=0; i<n; ++i)
    {
        m_ctrls[i]->setPosition(rect);
        rect.offset(0, dy);
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
    int w = m_rect.width();
    int h = m_rect.height();
    
    UControlVec::size_type n = m_ctrls.size();
    
    if (0 == n)
    {
        return FALSE;
    }
    
    int dx = w/nXNum;
    int dy = h/nYNum;
    
    huys::URectL rect;
    rect.set(m_rect.left(), m_rect.top(), dx, dy);
    
    for (UControlVec::size_type i=0; i<n; ++i)
    {
        m_ctrls[i]->setPosition(rect);
        if ((i+1)%nXNum == 0)
        {
            rect.offset(dx-w, dy);
        }
        else
        {
            rect.offset(dx, 0);
        }
    }

    return TRUE;
}

}; // namespace layout


}; // namespace huys
