#include <iostream>

using std::cout;
using std::endl;

class VectorBase
{
    void insert(void *)
    {
    }
    
    void *at(int index)
    {
        
    }
};

template <class T>
class Vector<T *> : VectorBase
{

inline void insert(T *t)
{
    VectorBase::insert(t);
}

inline T *at(int index)
{
    return VectorBase::at(index);
}

};


int main()
{
    return 0;
}

