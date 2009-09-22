#include <iostream>

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

int main()
{


    cout << "=========================" << endl;
    FuncPtr0 p = f();  // natural usage syntax
    p();

    huys::ADT::Func0<FuncPtr0_>::type pp = f();
    pp();

    return 0;
}

