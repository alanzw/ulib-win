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

    cout << "=========================" << endl;


    return 0;
}
