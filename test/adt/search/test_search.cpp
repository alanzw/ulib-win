/*
 * =====================================================================================
 *
 *       Filename:  test_search.cpp
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

#include "adt/usearch.h"
#include "adt/usort.h"

using std::cout;
using std::endl;

void print(int a[], int n)
{
    for (int i = 0; i < n-1; i++)
    {
        cout << a[i] << "-";
    }
    cout << a[n-1];
    cout << endl;
}

int main()
{
    int a[] = { 1, 9, 0, 5, 6, 7, 8, 2, 4, 3 };
    print(a, sizeof(a)/sizeof(int));

    int pos = -1;
    pos = huys::ADT::Search::sequence_search<int>(a, 0, 9, 6);
    cout << "find " << 6 << " at " << pos << endl;

    pos = -1;
    pos = huys::ADT::Search::binary_search<int>(a, 0, 9, 6);
    cout << "find " << 6 << " at " << pos << endl;

    huys::ADT::Sort::selection_sort<int>(a, sizeof(a)/sizeof(int));
    print(a, sizeof(a)/sizeof(int));
    pos = -1;
    pos = huys::ADT::Search::fibonacci_search<int>(a, 0, sizeof(a)/sizeof(int)-1, 6);
    cout << "find " << 6 << " at " << pos << endl;

    int data[] = {
        1, 4, 5, 7, 9, 11, 13, 16, 18, 20, 25, 27, 30, 32, 33, 36, 39, 41, 44, 47, 51, 53, 55
    };
    print(data, sizeof(data)/sizeof(int));
    pos = -1;
    pos = huys::ADT::Search::fibonacci_search<int>(data, 0, sizeof(data)/sizeof(int)-1, 30);
    cout << "find " << 30 << " at " << pos << endl;

    pos = -1;
    pos = huys::ADT::Search::interpolation_search<int>(data, 0, sizeof(data)/sizeof(int)-1, 30);
    cout << "find " << 30 << " at " << pos << endl;
    getchar();
    return 0;
}

