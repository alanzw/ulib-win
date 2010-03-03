#ifndef U_VECTOR_H
#define U_VECTOR_H

#define NOMINMAX
#include <memory>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace huys
{

namespace ADT
{

using std::allocator;
#if defined(_MSC_VER) && !(_MSC_VER > 1200)  // VC6
  #ifndef max
    #define max(x, y) std::_cpp_max(x,y)
  #endif
#else
  using std::max;
#endif // (_MSC_VER <= 1200)

using std::uninitialized_copy;
using std::uninitialized_fill;
using std::ostream;


template <typename T, class _Alloc = std::allocator<T> >
class UVector
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef T & reference_type;
    typedef const T & const_ref_type;

    UVector()
    {
        create();
    }

    explicit UVector(size_type n, const T& val = T())
    {
        create(n, val);
    }

    UVector(const_iterator start, const_iterator end)
    {
        create(start, end);
    }

    ~UVector()
    {
        uncreate();
    }

    UVector(const UVector& v)
    {
        create(v.begin(), v.end());
    }

    //
    UVector& operator=(const UVector& v)
    {
        // check for self-assignment
        if (&v != this)
        {

            // free the array in the left-hand side
            uncreate();

            // copy elements from the right-hand to the left-hand side
            create(v.begin(), v.end());
        }
        return *this;
    }

    //
    size_type size() const { return avail-data; }

    reference_type operator[](size_type i) { return data[i]; }
    const_ref_type operator[](size_type i) const { return data[i]; }

    //
    iterator begin() { return data; }
    const_iterator begin() const { return data; }

    iterator end() { return avail; }
    const_iterator end() const { return avail; }

    const_ref_type front() const
    {
        return this->at(0);
    }

    const_ref_type back() const
    {
        return this->at(size()-1);
    }

    void push_back(const T& val)
    {
        if (avail == limit)
        {
            grow();
        }
        uncheck_append(val);
    }

    value_type pop_back()
    {
        value_type temp = back();
        cutTo(size()-1);
        return temp;
    }

    void append(const_iterator start, const_iterator end)
    {
        const_iterator it = start;
        for (; it!=end; ++it)
        {
            push_back(*start);
        }
    }

    reference_type at(size_type n)
    {
        assert(n>=0 && n<size());
        return data[n];
    }

    const_ref_type at(size_type n) const
    {
        assert(n>=0 && n<size());
        return data[n];
    }

    void assign(size_type num, const_ref_type val)
    {
        uncreate();
        create(num, val);
    }

    void assign(iterator start, iterator end)
    {
        uncreate();
        create(start, end);
    }

    void cutTo(size_type n)
    {
        if (this->size() < n)
        {
            return;
        }

        avail = data + n;
    }

    void zero()
    {
        cutTo(0);
    }

    void resize(size_type n, const_ref_type val = T())
    {
        if (n <= size())
        {
            cutTo(n);
            return;
        }
        else if (n < limit-data)
        {
            for (int i=n-size(); i>0; --i)
            {
                push_back(val);
            }
        }
        else
        {
            grow(n);
            for (int i=n-size(); i>0; --i)
            {
                push_back(val);
            }
        }
    }

    bool empty() const
    {
        return (0 == size());
    }
    //template <class ST>
    //friend ostream & operator << (ostream & os, UVector<ST> v);

    iterator erase(iterator p)
    {
        std::copy(p + 1, end(), p);
        destroy(avail - 1, avail);
        --avail;
        return p;
    }

    iterator erase(iterator f, iterator l)
    {
        iterator s = copy(l, end(), f);
        destroy(s, end());
        avail = s;
        return f;
    }

    void clear()
    {
        erase(begin(), end());
    }
private:
    iterator data;
    iterator avail;
    iterator limit;

    //
    //allocator<T> alloc;
    _Alloc alloc;

    //
    void create()
    {
        data = avail = limit = 0;
    }

    void create(size_type n, const T& val)
    {
        data = alloc.allocate(n);
        limit = avail = data + n;
        uninitialized_fill(data, limit, val);
    }

    void create(const_iterator begin, const_iterator end)
    {
        data = alloc.allocate(end-begin, 0);
        limit = avail = uninitialized_copy(begin, end, data);
    }

    //
    void uncreate()
    {
        if (data)
        {
            iterator it = avail;
            while ( it != data )
            {
                alloc.destroy(--it);
            }
            alloc.deallocate(data, limit - data);
        }
        data = limit = avail = 0;
    }

    //
    void grow(size_type n = 0)
    {
        // when growing, allocate twice as much space as currently in use
        size_type new_size = (0==n ? max(2 * (limit - data), ptrdiff_t(1)) : n);

        // allocate new space and copy existing elements to the new space
        iterator new_data = alloc.allocate(new_size, 0);
        iterator new_avail = uninitialized_copy(data, avail, new_data);

        // return the old space
        uncreate();

        // reset pointers to point to the newly allocated space
        data = new_data;
        avail = new_avail;
        limit = data + new_size;

    }

    //
    void uncheck_append(const T& val)
    {
        alloc.construct(avail++, val);
    }

    void destroy(iterator f, iterator l)
    {
        for (; f != l; ++f)
        {
            alloc.destroy(f);
        }
    }
};

template <typename T>
ostream & operator << (ostream &os, UVector<T> v)
{
    typename UVector<T>::const_iterator it;
    for( it = v.begin(); it != v.end(); ++it)
    {
        os << "     " << *it << std::endl;
    }

    return os;
}

}; // namespace ADT

}; // namespace huys

#endif // U_VECTOR_H

