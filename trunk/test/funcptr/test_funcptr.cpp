#include <iostream>
#include <cassert>

#include "adt/ufuncptr.h"

using std::cout;
using std::endl;

using huys::ADT::FuncPtr0;
using huys::ADT::FuncPtr0_;

FuncPtr0_ f()
{
    cout << __FUNCTION__ << endl;
    return f;
} // natural return syntax

template <typename T>
class UComPtr
{
public:
    typedef T Type;
    typedef T* TypePtr;
public:
    UComPtr()
    : _p(0)
    {
    }

    UComPtr(T * p)
    {
        if (0 != (_p=p))
        {
            //_p->AddRef();
        }
    }

    ~UComPtr()
    {
        if(_p)
        {
            //_p->Release();
            delete _p;
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
    
    T * operator->() const
    {
        assert(_p);
        return (T*)_p;
    }
private:
    TypePtr _p;
};

struct Object
{
    int x;
};

int main()
{
    cout << "=========================" << endl;
    FuncPtr0 p = f();  // natural usage syntax
    p();

    huys::ADT::Func0<FuncPtr0_>::type pp = f();
    pp();

    UComPtr<Object> px(new Object);
    
    cout << px->x << endl;
    
    return 0;
}

