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

    virtual ~ULayout()= 0; /// Implement must be supplied, or link error.

    BOOL addControl(UControl *pCtrl);
    BOOL addControls(UControl **p, int n);
    virtual BOOL go() = 0;

    void setRect(const LPRECT lpRect)
    {
        m_rect = lpRect;
    }

    void setRect(int x, int y, int w, int h)
    {
        m_rect.set(x, y, w, h);
    }
protected:
    huys::URectL m_rect;
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

