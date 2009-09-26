#ifndef U_STACK_H
#define U_STACK_H

#include "adt/uvector.h"

namespace huys
{

namespace ADT
{

template <typename T>
class UStack : public UVector<T>
{
public:
    UStack()
    {}

    ~UStack()
    {}

    //
    void push(const T &x)
    {
        UVector<T>::push_back(x);
    }

    T pop()
    {
        T temp = *(UVector<T>::end() - 1);
        UVector<T>::cutTo(UVector<T>::size()-1);
        return temp;
    }

    const T & top()
    {
        return *(UVector<T>::end() - 1);
    }
    //
    bool isEmpty() const
    {
        return false;
    }
private:
};

}; // namespace ADT


}; // namespace huys

#endif // U_STACK_H

