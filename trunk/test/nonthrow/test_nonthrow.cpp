#include <iostream>

using std::cout;
using std::endl;

namespace Orange
{

class String
{
    char *str;
public:
    void swap(String &s)
    {
        std::swap(this->str, s.str);
    }
};

void swap(String &s1, String &s2)
{
    s1.swap(s2);
}

}; // namespace Orange

namespace std
{

template <>
void swap(Orange::String &s1, Orange::String &s2)
{
    s1.swap(s2);
}

}


int main()
{
    return 0;
}

