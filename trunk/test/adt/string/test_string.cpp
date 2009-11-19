#include <iostream>

#include "adt/ustring.h"


int main()
{
    using std::cout;
    using std::endl;


    typedef huys::ADT::UStringAnsi UString;

    cout << "=========================" << endl;

    UString s("1111");

    cout << s.c_str() << endl;

    cout << s.substr(1, 2).c_str() << endl;

    UString s2("2222");

    cout << s2.length() << endl;

    s2 += s;

    cout << s2.length() << endl;

    cout << s2.c_str() << endl;

    cout << (s2+"3333").c_str() << endl;

    cout << (s2<<"4444").c_str() << endl;

    cout << "=========================" << endl;


    return 0;
}
