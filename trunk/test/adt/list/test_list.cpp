#include <iostream>


#include "adt/ulist.h"

#include "adt/uarraylist.h"

void print(int *p)
{
    std::cout << *p << std::endl;
}

int main()
{
    using std::cout;
    using std::endl;

    huys::ADT::UList<int> list;

    list.push_back(1);
    list.push_front(2);

    std::cout << "front: " << list.front() << std::endl;
    std::cout << "back:  " << list.back() << std::endl;
    std::cout << "size:  " << list.size() << std::endl;

    std::cout << "------------------------------------------" << std::endl;

    list.push_front(22);
    list.push_back(3);
    list.remove(22);
    std::cout << "front: " << list.front() << std::endl;
    std::cout << "back:  " << list.back() << std::endl;
    std::cout << "size:  " << list.size() << std::endl;

    std::cout << "------------------------------------------" << std::endl;

    huys::ADT::UList<const char *> slist;
    slist.push_back("111111");
    slist.push_back("xxxxxx");
    slist.push_back("122222");
    slist.remove("122222");
    std::cout << "back:  " << slist.back() << std::endl;
	slist.push_back("122222");
	std::cout << "content : " << std::endl;

	std::cout << slist;
	
    slist.clear();
    std::cout << "size:  " << slist.size() << std::endl;
    
    std::cout << "------------------------------------------" << std::endl;
    huys::ADT::UArrayList<int> aList;

    aList.insertEnd(32);
    aList.insertEnd(2);
    aList.print();

    aList.replaceAt(0, 1);
    aList.print();

    huys::ADT::UArrayList<int> otherList(aList);
    otherList.print();
    
    std::cout << "------------------------------------------" << std::endl;
    otherList.traverse(&print);
    return 0;
}

