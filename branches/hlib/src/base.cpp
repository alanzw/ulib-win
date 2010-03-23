#include <iostream>
using namespace std;

template <class T>
class B1
{
public: 
    void SayHi() 
    {
        T* pT = static_cast<T *>(this);   // HUH?? I'll explain this below
        pT->PrintClassName();
    } 
     void PrintClassName() { cout << "This is B1\n"; }
};
 
class D1 : public B1<D1>
{
    // No overridden functions at all
};
 
class D2 : public B1<D2>
{
public:
    void PrintClassName() { cout << "This is D2\n"; }
};

int main()
{
    D1 d1;
    D2 d2;
 
    d1.SayHi();    // prints "This is B1"
    d2.SayHi();    // prints "This is D2"
    
   
    return 0;
}
