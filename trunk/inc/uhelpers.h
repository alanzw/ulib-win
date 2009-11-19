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
    UPoint(T x = T(0), T y = T(0))
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

    void operator = (const UPoint<T> &aPoint)
    {
        this->_x = aPoint._x;
        this->_y = aPoint._y;
    }

    void operator= (const LPPOINT lpPoint)
    {
        this->_x = lpPoint->x;
        this->_y = lpPoint->y;
    }

    operator POINT()
    {
        POINT pt = {_x, _y};
        return pt;
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
    URect(T left, T top, T right, T bottom)
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


    operator RECT()
    {
        RECT rc = {_left, _top, _right, _bottom};
		return rc;
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
private:
    T _left;
    T _right;
    T _top;
    T _bottom;
};

//
template <typename T>
class USize
{
public:
	operator SIZE()
	{
		SIZE s = {_cx, _cy};
		return s;
	}
private:
	T _cx;
	T _cy;
};

typedef UPoint<int> UPointI;
typedef URect<int>  URectI;
typedef USize<int>  USizeI;

}; // namespace huys

#endif // U_HELPERS_H

