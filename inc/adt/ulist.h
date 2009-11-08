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

// TODO: Rewrite with the memory allocator

#ifndef U_LIST_H
#define U_LIST_H

#include <memory>
#include <cassert>
#include <iostream>

namespace huys
{

namespace ADT
{

template <class T, class _Alloc = std::allocator<T> >
class UList
{
private:
    struct ListNode {
        ListNode *_next;
        ListNode *_prev;
        T _value;
    };
public:
    typedef T value_type;
    typedef ListNode node_type;
    typedef node_type* iterator;
    typedef const node_type* const_iterator;
    typedef size_t size_type;
    typedef T& reference;
    typedef const T& const_reference;
public:
    explicit UList(const _Alloc& _A = _Alloc())
    : _allocator(_A), _head(_buynode()), _size(0)
    {}

    ~UList()
    {
        erase(begin(), end());
        _freenode(_head);
        _head = 0;
        _size = 0;
    }

    UList(const UList<T> &s)
    {

    }

    UList<T> & operator=(const UList<T> &s)
    {
    }

    //
    iterator begin()
    {
        return _head->_next;
    }

    const_iterator begin() const
    {
        return _head->_next;
    }

    iterator end()
    {
        return _head;
    }

    const_iterator end() const
    {
        return _head;
    }

    //
    bool empty() const
    {
        return (0==_size);
    }

    size_type size() const
    {
        return _size;
    }

    reference front()
    {
        assert(!empty());
        return (begin()->_value);
    }

    reference back()
    {
        assert(!empty());
        iterator p = end();
        //--p;
        return (p->_prev->_value);
    }

    void push_front(const T& x)
    {
        insert(begin(), x);
    }

    void push_back(const T& x)
    {
        insert(end(), x);
    }

    T pop_front()
    {
    }

    T pop_back()
    {
    }

    void clear()
    {
        if (empty()) return;
        erase(begin(), end());
    }

    void remove(const_reference x)
    {
        iterator p = begin();
        iterator q = p->_next;
        while (p != end())
        {
            if ( p->_value == x)
            {
                erase(p);
            }

            p = q;
            q = p->_next;
        }
    }

    iterator insert(iterator p, const_reference x = T())
    {
        iterator s = _buynode(p, p->_prev);
        p->_prev->_next = s;
        p->_prev = s;
        s->_value = x;
        ++_size;
        return p;
    }

    iterator erase(iterator p)
    {
        if (end() == p)
            return end();

        iterator s = p;
        s->_prev->_next= s->_next;
        s->_next->_prev = s->_prev;
        _allocator.destroy(&s->_value);
        _freenode(s);
        --_size;
        return p;
    }

    iterator erase(iterator _F, iterator _L)
    {
        iterator p = _F;
        while (_F != _L)
        {
            p = _F->_next;
            erase(_F);
            _F = p;
        }
        return (_F);
    }
private:
    _Alloc _allocator;
    node_type *_head;
    size_type _size;

private:
    node_type * _buynode(node_type * next = 0, node_type * prev = 0)
    {
        //node_type * p = (node_type *)_allocator.allocate(1 * sizeof (ListNode)/sizeof(T), (char *)0);
        node_type * p = static_cast<node_type *>(::operator new(sizeof(node_type)));
        // node_type * p = new node_type;
        p->_next = next != 0 ? next : p;
        p->_prev = prev != 0 ? prev : p;
        return p;
    }

    void _freenode(node_type *p)
    {
        //_allocator.deallocate((T *)x, sizeof (ListNode)/sizeof(T));
        ::operator delete(p);
        //delete p;
    }

    template <typename S>
    friend std::ostream & operator << (std::ostream &os, UList<S> & v);
};

template <typename T>
std::ostream & operator << (std::ostream &os, UList<T> &v)
{
    typename UList<T>::iterator it;
    for( it = v.begin(); it != v.end(); it=it->_next)
    {
        os << "     " << it->_value << std::endl;
    }
}

}; // namespace ADT

}; // namespace huys

#endif // U_LIST_H

