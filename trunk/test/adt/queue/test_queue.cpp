#include <iostream>

#include "adt/uqueue.h"

int main()
{
    using std::cout;
    using std::endl;

    typedef huys::ADT::UQueue<int> QueueInt;

    QueueInt q;

    std::cout << "Size: " << q.size() << std::endl;

    q.append(11);
    std::cout << "Size: " << q.size() << std::endl;

    q.serve();
    std::cout << "Size: " << q.size() << std::endl;

    return 0;
}

