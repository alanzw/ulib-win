#include <iostream>

#include "dp/udelegate.h"
#include "adt/uvector.h"

typedef huys::dp::utility::Delegate * DelegatePtr;
typedef huys::ADT::UVector<DelegatePtr> UVec;

void Test(int iParam)
{
   std::cout << "Test Invoked\r\n";
}

class A
{
 public:
    void Test(int iParam)
    {
       std::cout << "A::Test Invoked\r\n";
    }
};


int main()
{
    typedef huys::dp::utility::NonTypeDelegate NTDelegate;
    typedef huys::dp::utility::TypeDelegate<A> TDelegate;

    NTDelegate ntDlt(Test, 1);

    A a;
    TDelegate tDlt(a, &A::Test, 2);

    UVec v;
    v.push_back(&ntDlt);
    v.push_back(&tDlt);

    UVec::const_iterator it;
        for( it = v.begin(); it != v.end(); ++it)
    {
        (*it)->invoke();
    }

    return 0;
}

