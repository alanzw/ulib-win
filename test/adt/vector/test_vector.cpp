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
    //for( it = v.begin(); it != v.end(); ++it)
    //{
    //    cout << "     " << *it << endl;
    //}
    cout << v;


    cout << "========================= push_back(10)" << endl;
    v.push_back(10);
    //for( it = v.begin(); it != v.end(); ++it)
    //{
    //    cout << "     " << *it << endl;
    //}
    cout << v;

    cout << "========================= assign(10, 100)" << endl;
    v.assign(10, 100);
    //for( it = v.begin(); it != v.end(); ++it)
    //{
    //    cout << "     " << *it << endl;
    //}
    cout << v;

    cout << "========================= assign(start, end)" << endl;
    UVec v2(5, 31);
    v.assign(v2.begin(), v2.end());
    //for( it = v.begin(); it != v.end(); ++it)
    //{
    //    cout << "     " << *it << endl;
    //}
    cout << v;

    cout << "========================= " << endl;
    cout << v2;

    return 0;
}
