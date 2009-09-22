#include <iostream>

#include "adt/uvector.h"

int main()
{
    using std::cout;
    using std::endl;

    typedef huys::ADT::UVector<int> UVec;
    UVec v(10, 33);

    cout << "=========================" << endl;
    UVec::const_iterator it;
    for( it = v.begin(); it != v.end(); ++it)
    {
        cout << "     " << *it << endl;
    }

    cout << "=========================" << endl;
    v.push_back(10);
    for( it = v.begin(); it != v.end(); ++it)
    {
        cout << "     " << *it << endl;
    }

    return 0;
}

