#include <iostream>

#include "adt/uvector.h"

int main()
{
    using std::cout;
    using std::endl;
    using std::cin;

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

    cout << "========================= " << endl;
    int array[] = { 1, 2, 3, 4, 5};
    int size = sizeof(array)/sizeof(int);
    UVec v3(array, array+size);
    cout << v3;
    cout << "========================= " << endl;
    v3.resize(4);
    cout << v3;
    cout << "========================= " << endl;
    v3.resize(6, 3);
    cout << v3;
    cout << "========================= " << endl;
    v3.resize(5);
    cout << v3[4] << endl;
    
    v3[0] = 3;
    cout << "========================= " << endl;
    cout << "Size of v3 : " << v3.size() << endl;
    cout << v3;
    cout << "========================= " << endl;
    for ( UVec::iterator it = v3.begin(); it != v3.end(); )
    {
        if (3 == *it)
        {
            v3.erase(it);
        }
        else
        {
            ++it;
        }
    }
    cout << "Size of v3 : " << v3.size() << endl;    
    cout << v3;
    
    cin.ignore();

    return 0;
}
