#include <iostream>

#include "adt/ustack.h"

int main()
{
    using std::cout;
    using std::endl;

    typedef huys::ADT::UStack<int> UStackI;
    UStackI s;

    s.push(1);

    cout << "=========================" << endl;
    cout << "     " << s.top() << endl;

    s.push(2);

    cout << "=========================" << endl;
    cout << "     " << s.top() << endl;

    s.pop();


    cout << "=========================" << endl;
    cout << "     " << s.top() << endl;

    return 0;
}

