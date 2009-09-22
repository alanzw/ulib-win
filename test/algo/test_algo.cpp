#include <iostream>

#include "adt/ualgo.h"

int main()
{
    using std::cout;
    using std::endl;

    typedef huys::ADT::string UString;
    typedef huys::ADT::vector<UString> UVector;

    UString str = "123 huys cccc";

    cout << "=========================" << endl;
    cout << "origin : " << str << endl;

    UVector vector = huys::ADT::split(str);

    UVector::iterator iter;

    cout << "=========================" << endl;
    cout << "split : " << endl;
    for (iter = vector.begin(); iter != vector.end(); ++iter)
    {
        cout << "        " << *iter << endl;
    }


    cout << "fib(6) : " << huys::ADT::fibonacci_sequence(6) << endl;

    return 0;
}

