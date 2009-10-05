/*
 * Barton-Nackman trick is a term coined by the C++ standardization committee (ISO/IEC JTC1 SC22 WG21)
 * to refer to an idiom introduced by John Barton and Lee Nackman as Restricted Template Expansion
 */

#include <iostream>

// A class template to express an equality comparison interface.
template<typename T> class equal_comparable
{
    friend bool operator==(T const &a, T const &b) { return  a.equal_to(b); }
    friend bool operator!=(T const &a, T const &b) { return !a.equal_to(b); }
};

class value_type
 // Class value_type wants to have == and !=, so it derives from
 // equal_comparable with itself as argument (which is the CRTP).
 : private equal_comparable<value_type>
{
  public:
    bool equal_to(value_type const& rhs) const
    {
        return false;
    }// to be defined
};


int main()
{
    value_type v1, v2;

    if (v1 == v2)
    {
        std::cout << "v1 == v2" << std::endl;
    }
    else
    {
         std::cout << "v1 != v2" << std::endl;
    }

    return 0;
}
