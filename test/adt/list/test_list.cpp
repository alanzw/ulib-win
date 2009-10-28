#include <iostream>


#include "adt/ulist.h"


int main()
{
    using std::cout;
    using std::endl;

    huys::ADT::UList<int> list;

	list.push_back(1);
	list.push_back(3);
	list.push_front(2);

	std::cout << list.front() << std::endl;
	std::cout << list.back() << std::endl;

    return 0;
}

