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


#define MAX 10
#define SHOWPASS

void radixsort(int *a,int n)
{
        int i,b[MAX],m=0,exp=1;
        for(i=0;i<n;i++)
        {
            if(a[i]>m)
                m=a[i];
        }

        while(m/exp>0)
        {
            int bucket[10]={0};
            for(i=0;i<n;i++)
                bucket[a[i]/exp%10]++;
            for(i=1;i<10;i++)
                bucket[i]+=bucket[i-1];
            for(i=n-1;i>=0;i--)
                b[--bucket[a[i]/exp%10]]=a[i];
            for(i=0;i<n;i++)
                a[i]=b[i];
            exp*=10;

#ifdef SHOWPASS
            printf("\nPASS : ");
            print(a, n);
#endif
        }
}


int main()
{
    int a[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    printf("%-25s", "origin: ");
    print(a, 10);

    huys::ADT::quicksort_basic<int>(a, 0, sizeof(a)/sizeof(int)-1);
    printf("%-25s", "quicksort_basic: ");
    print(a, 10);

    huys::ADT::quicksort_partition<int>(a, 0, sizeof(a)/sizeof(int)-1);
    printf("%-25s", "quicksort_partition: ");
    print(a, 10);

    huys::ADT::selection_sort<int>(a, sizeof(a)/sizeof(int));
    printf("%-25s", "selection_sort: ");
    print(a, 10);

    huys::ADT::insertion_sort<int>(a, sizeof(a)/sizeof(int));
    printf("%-25s", "insertion_sort: ");
    print(a, 10);

    int b[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    int temp[10] = {0};
    //print(b, 10);
    huys::ADT::Sort::radix_sort<int>(b, temp, 10);
    printf("%-25s", "radix_sort: ");
    print(b, 10);

    int c[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    radixsort(c, 10);

    int d[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    huys::ADT::Sort::gnome_sort<int>(d, 10);
    printf("%-25s", "gnome_sort: ");
    print(d, 10);

    int e[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    for (int i=10; i>1; i--) huys::ADT::Sort::heap_sort<int>(e, i-1);
    printf("%-25s", "heap_sort: ");
    print(d, 10);
    
    int f[] = {
        1, 9, 0, 5, 6, 7, 8, 2, 4, 3
    };
    huys::ADT::Sort::merge_sort<int>(f, 10);
    printf("%-25s", "merge_sort: ");
    print(d, 10);    
    getchar();
    return 0;
}

