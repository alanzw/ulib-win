/*
 * =====================================================================================
 *
 *       Filename:  factory.h
 *
 *    Description:  dp.creational.factory
 *
 *        Version:  1.0
 *        Created:  2009-7-19 12:58:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */
#ifndef U_DP_FACTORY_H
#define U_DP_FACTORY_H

#include <stack>

namespace huys
{

namespace dp
{

namespace creational
{

template <typename T>
class Product
{
public:
    const T & get()
    {
        return m_p;
    }

    void set(const T & value)
    {
        m_p = value;
    }
private:
    T m_p;
};

template <typename T>
class Factory
{
public:
    typedef Product<T> ProductType;

    ProductType * produce()
    {
        ProductType * p = new ProductType;
        _s.push(p);
        return p;
    }

    Factory()
    {
    }

    ~Factory()
    {
        ProductType * p = 0;
        while (!_s.empty())
        {
            p = _s.top();
            delete p;
            _s.pop();
        }
    }
private:
    typedef std::stack<ProductType *> ProductStack;
    ProductStack _s;
};

}; // namespace creational


}; // namespace dp


}; // namespace huys

#endif // U_DP_FACTORY_H
