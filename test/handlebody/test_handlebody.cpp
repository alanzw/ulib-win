#include <iostream>

using std::cout;
using std::endl;
using std::cin;


#include "xstring.h"

int main()
{
    String ss("xxxx");
    String s2 = "world";

    cout << "Before:" << endl;
    cout << ss << endl;
    cout << s2 << endl;

    ss = s2;

    cout << "After: " << endl;
    cout << ss << endl;
    cout << s2 << endl;

    cin.ignore();

    return 0;
}

