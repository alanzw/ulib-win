#ifndef U_STACK_H
#define U_STACK_H

namespace huys
{

namespace ADT
{

template <typename T, size_t size = 10>
class UStack
{
public:
    UStack()
    {
        ;
    }
    
    ~UStack()
    {
    }
    
    //
    bool push(T x)
    {
        return false;
    }
    
    T pop()
    {
        ;
    }
    
    //
    bool isEmpty() const
    {
        return false;
    }
    bool isFull() const
    {
        return false;
    }
private:
};

}; // namespace ADT


}; // namespace huys

#endif // U_STACK_H

