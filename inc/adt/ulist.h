/*
 * =====================================================================================
 *
 *       Filename:  ulist.h
 *
 *    Description:  Class UList
 *
 *        Version:  1.0
 *        Created:  2009-9-1 2:07:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_LIST_H
#define U_LIST_H

namespace huys
{

namespace ADT
{

template <class T>
class UList
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
public:
    UList()
    {}
    
    ~UList()
    {}
    
};

}; // namespace ADT

}; // namespace huys

#endif // U_LIST_H

