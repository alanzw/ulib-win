#ifndef HU_EX_H
#define HU_EX_H

namespace huys
{

template <typename Owner, typename T, void (Owner::*SetPtr)(T), T (Owner::*GetPtr)()>
class PropertyClass
{
    template<void (Owner::*_SetPtr)(T)>
    struct Set
    {
        typedef void (Owner::*SetPtrType)(T);
        static SetPtrType Get_SetPtr()
        {
            return _SetPtr;
        }
    };

    template<>
    struct Set<&Owner::No<T> >
    {};

    template<void (Owner::*_GetPtr)(T)>
    struct Get
    {
        typedef void (Owner::*GetPtrType)(T);
        static SetPtrType Get_GetPtr()
        {
            return _GetPtr;
        }
    };

    template<>
    struct Get<&Owner::No<T> >
    {};

public:
    PropertyClass()
    {
        //
    }
    void operator = (T value)
    {
        //
    }
    operator T()
    {
    }
    T operator ->()
    {
        return (mThis->*(Get<GetPtr>::Get_GetPtr()))();
    }
private:
    Owner *mThis;
};

}; // namespace huys

#endif // HU_EX_H

