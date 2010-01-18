#ifndef U_AUTOPTR_H
#define U_AUTOPTR_H

namespace huys
{

namespace ADT
{

template <typename T>
class UAutoPtr
{
public:
    typedef T Type;
    typedef T* TypePtr;
public:
    UAutoPtr()
    : _p(0)
    {
    }

    UAutoPtr(T * p)
    : _p(p)
    {}

    ~UAutoPtr()
    {
        if(_p)
        {
            delete _p;
            _p = 0;
        }
    }

    operator T*() const
    {
        return (T*)_p;
    }

    T& operator*() const
    {
        return *_p;
    }

    T* operator->() const
    {
        return (T*)_p;
    }

    UAutoPtr & operator=(T *p)
    {
        _p = p;
        return *this;
    }
private:
    TypePtr _p;
};

}; // namespace ADT

}; // namespace huys

#endif // U_AUTOPTR_H
