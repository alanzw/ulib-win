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

using std::cout;
using std::endl;

void print(int a[])
{
    for (int i = 0; i < 9; i++)
    {
        cout << a[i] << "-";
    }
    cout << a[9];
    cout << endl;
}

int main()
{
    int a[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    print(a);

    int pos = huys::ADT::Search::sequence_search<int>(a, 0, 9, 6);
    cout << "find " << 6 << " at " << pos << endl;
    getchar();
    return 0;
}

