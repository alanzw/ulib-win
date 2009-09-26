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

int partition(int a[], int p, int r)
{
    int x = a[r];
    int j = p - 1;
    for (int i = p; i < r; i++)
    {

        if (x <= a[i])
        {
            j = j + 1;
            int temp = a[j];
            a[j] = a[i];
            a[i] = temp;
        }
    }
    a[r] = a[j + 1];
    a[j + 1] = x;

    return (j + 1);
}

void quickSort(int a[], int p, int r)
{
    if (p < r) {
        int q = partition(a, p, r);
        quickSort(a, p, q - 1);
        quickSort(a, q + 1, r);
    }
}

int main()
{
    int a[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    print(a);
    //quickSort(a, 0, 9);
    huys::ADT::quicksort_basic<int>(a, 0, sizeof(a)/sizeof(int)-1);
    print(a);
    huys::ADT::quicksort_partition<int>(a, 0, sizeof(a)/sizeof(int)-1);
    print(a);
    getchar();
    return 0;
}

