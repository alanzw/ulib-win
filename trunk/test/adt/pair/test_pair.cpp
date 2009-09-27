#include <iostream>


#include "adt/upair.h"


int main()
{
    using std::cout;
    using std::endl;

    huys::ADT::UPair<int, const char *> p(1, "string");
    huys::ADT::UPair<int, const char *> pc(2, "string");

    cout << p.first() << " : " << p.second() << endl;


    if (p == p)
    {
        cout << "p == p" << endl;
    }

    if (p != pc)
    {
        cout << "p != pc" << endl;
    }

    using huys::ADT::make_pair;
    if (make_pair(1, 1) == make_pair(1, 1))
    {
        cout << "p == p" << endl;
    }
    return 0;
}

