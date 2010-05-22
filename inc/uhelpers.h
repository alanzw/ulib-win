/*
 * =====================================================================================
 *
 *       Filename:  uhelpers.h
 *
 *    Description:  Miscellaneous Classes and Functions.
 *
 *        Version:  1.0
 *        Created:  2009-6-21 21:29:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_HELPERS_H
#define U_HELPERS_H

#include <windows.h>

namespace huys
{
//
template <typename T>
class UPoint
{
public:
    UPoint(T x = 0, T y = 0)
    : _x(x), _y(y)
    {}

    explicit UPoint(LPPOINT lpPoint)
    : _x(lpPoint->x), _y(lpPoint->y)
    {}

    explicit UPoint(const UPoint<T> &aPoint)
    : _x(aPoint._x), _y(aPoint._y)
    {}

    ~UPoint()
    {}

    UPoint& operator = (const UPoint<T> &aPoint)
    {
        this->_x = aPoint._x;
        this->_y = aPoint._y;
        return *this;
    }

    UPoint& operator= (const LPPOINT lpPoint)
    {
        this->_x = lpPoint->x;
        this->_y = lpPoint->y;
        return *this;
    }

    operator LPPOINT()
    {
        //POINT pt = {_x, _y};
        //return pt;
        return (LPPOINT)(this);
    }

    void moveTo(T x, T y)
    {
        _x = x;
        _y = y;
    }

    T x() const
    {
        return _x;
    }

    T y() const
    {
        return _y;
    }
private:
    T _x;
    T _y;
};

//
template <typename T>
class URect
{
public:
    URect(T left = 0, T top = 0, T right = 0, T bottom = 0)
    : _left(left), _top(top), _right(right), _bottom(bottom)
    {}

    URect(LPRECT lpRect)
    : _left(lpRect->left),
      _top(lpRect->top),
      _right(lpRect->right),
      _bottom(lpRect->bottom)
    {}

    ~URect()
    {}

    URect& operator= (const LPRECT lpRect)
    {
        _left = lpRect->left;
        _top = lpRect->top;
        _right = lpRect->right;
        _bottom = lpRect->bottom;

        return *this;
    }

    void set(T left = 0, T top = 0, T w = 0, T h = 0)
    {
        _left = left;
        _top = top;
        _right = left + w;
        _bottom = top + h;
    }

    void resize(T cx, T cy)
    {
        _right = _left + cx;
        _bottom = _top + cy;
    }

    operator LPRECT()
    {
        //RECT rc = {_left, _top, _right, _bottom};
        //return rc;
        return (LPRECT)(this);
    }

    bool PtrInRect(const UPoint<T> &aPoint)
    {
        if ( aPoint.x()<_left || aPoint.x()>_right
            || aPoint.y()<_top || aPoint.y()>_bottom)
        {
            return false;
        }

        return true;
    }

    void inflate(T x, T y)
    {
        _left -= x;
        _top -= y;
        _right += x;
        _bottom += y;
    }

    void deflate(T x, T y)
    {
        inflate(-x, -y);
    }

    void offset(T x, T y)
    {
        _left += x;
        _top += y;
        _right += x;
        _bottom += y;
    }

    T left() const { return _left; }
    T top() const { return _top; }
    T right() const { return _right; }
    T bottom() const { return _bottom; }
    T width() const { return _right - _left; }
    T height() const { return _bottom - _top; }
private:
    T _left;
    T _top;
    T _right;
    T _bottom;
};

//
template <typename T>
class USize
{
public:
    USize(T cx = 0, T cy = 0)
    :  _cx(cx), _cy(cy)
    {}

    USize(LPSIZE lpSize)
    : _cx(lpSize->cx), _cy(lpSize->cy)
    {}

    operator LPSIZE()
    {
        //SIZE s = {_cx, _cy};
        //return s;
        return (LPSIZE)(this);
    }

    T width() const { return _cx; }
    T height() const { return _cy; }
private:
    T _cx;
    T _cy;
};

typedef UPoint<int> UPointI;
typedef URect<int>  URectI;
typedef USize<int>  USizeI;
typedef UPoint<long> UPointL;
typedef URect<long> URectL;
typedef USize<long> USizeL;

}; // namespace huys

#endif // U_HELPERS_H

