#include <iostream>

#include "unumerical.h"

using std::cout;
using std::endl;

int main()
{
    Vertex2D v(1.0f, 2.0f);

    cout << v.toString().c_str() << endl;

    return 0;
}
