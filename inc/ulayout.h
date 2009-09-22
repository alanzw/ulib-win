#ifndef U_LAYOUT_H
#define U_LAYOUT_H

#include "ucontrol.h"

#include "adt/uvector.h"

namespace huys
{

namespace layout
{

typedef huys::ADT::UVector<UControl *> UControlVec;

class ULIB_API ULayout
{
public:
    ULayout();
    virtual ~ULayout() = 0; /// Implement must be supplied, or link error.
    BOOL addControl(UControl *pCtrl);
    virtual BOOL go() = 0;
    void setRect(const LPRECT lpRect)
    {
        m_rc = *lpRect;
    }
    void setRect(int x, int y, int w, int h)
    {
        m_rc.left = x;
        m_rc.top = y;
        m_rc.right = x + w;
        m_rc.bottom = y + h;
    }
protected:
    RECT m_rc;
    UControlVec m_ctrls;
};

class ULIB_API UHBoxLayout : public ULayout
{
public:
    UHBoxLayout();
    virtual ~UHBoxLayout();
    virtual BOOL go();
private:
};


class ULIB_API UVBoxLayout : public ULayout
{
public:
    UVBoxLayout();
    virtual ~UVBoxLayout();
    virtual BOOL go();
private:
};

class ULIB_API UGridLayout : public ULayout
{
public:
    UGridLayout(int nx = 2, int ny = 2);
    virtual ~UGridLayout();
    virtual BOOL go();
private:
    int nXNum;
    int nYNum;
};

}; // namespace layout

}; // namespace huys

#endif // U_LAYOUT_H

