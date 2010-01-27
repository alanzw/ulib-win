#ifndef U_AUTOPTR_H
#define U_AUTOPTR_H

namespace huys
{

namespace ADT
{

template <class T>
struct auto_ptr_ref
{
    T* _p;
    auto_ptr_ref (T* rhs) : _p(rhs)
    {}
};

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

    explicit UAutoPtr(T * p)
    : _p(p)
    {}

    UAutoPtr(UAutoPtr &p)
    {

    }

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

    T& operator*() const throw()
    {
        return *_p;
    }

    UAutoPtr & operator=(UAutoPtr &x)
    {
        _p = x._p;
        x._p = 0;
        return *this;
    }

    T * release() throw()
    {
        T* tmp(_p);
        _p = 0;
        return tmp;
    }

    void reset(T* ptr=0) throw()
    {
        if(_p != ptr)
        {
            delete _p;
            _p = ptr;
        }
    }
private:
    TypePtr _p;
private:
    UAutoPtr& operator= (UAutoPtr& rhs) const;

};

}; // namespace ADT

}; // namespace huys

#endif // U_AUTOPTR_H
