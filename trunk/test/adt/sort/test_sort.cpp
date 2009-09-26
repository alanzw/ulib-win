/*
 * =====================================================================================
 *
 *       Filename:  test_sort.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009/9/26 14:39:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <iostream>
#include <cstdio>

#include "adt/ualgo.h"

using std::cout;
using std::endl;

void print(int a[])
{
    for (int i = 0; i < 10; i++)
    {
        cout << a[i] << "-";
    }
    cout << endl;
}

int main()
{
    int a[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    print(a);
    huys::ADT::quicksort_basic<int>(a, 0, sizeof(a)/sizeof(int)-1);
    print(a);
    huys::ADT::quicksort_partition<int>(a, 0, sizeof(a)/sizeof(int)-1);
    print(a);
    getchar();
    return 0;
}

