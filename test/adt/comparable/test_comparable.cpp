#include <iostream>

#include "adt/ucomparable.h"

class value_type : private huys::ADT::comparable<value_type>
{
public:
    bool equal_to(value_type const& rhs) const
    {
        return false;
    }// to be defined
    bool greater_than(value_type const& rhs) const
    {
        return true;
    }
};

int main()
{
    using std::cout;
    using std::endl;

    value_type v1, v2;
    
    if (v1 == v2)
    {
        cout <<  " equal" << endl;
    }
    
    if (v1 > v2)
    {
        cout <<  " greater than" << endl;
    }
    
    return 0;
}

