#include <iostream>

#include "adt/utable.h"

void print(int *p)
{
    std::cout << *p << std::endl;
}

int main()
{
    using std::cout;
    using std::endl;

    typedef huys::ADT::UTable<int, const char *> UTableIC;

    UTableIC ic;

    ic.add(1, "ccccc");
    ic.add(2, "ccccc");

    cout << ic << endl;

    if (ic.inTable(2))
    {
        cout << "2 is in table!" << endl;
    }

    return 0;
}

