/*
 * =====================================================================================
 *
 *       Filename:  test_functional.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/13/2010 03:46:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <algorithm>
#include <iostream>
using namespace std;



void print(int x)
{
    cout << x << ' ';
}

bool less10(int x){
    return x < 10 ? true : false;
}


template <class _ForwardIter, class _Predicate>
_ForwardIter __partition(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    if (__first == __last) return __first;

    while (__pred(*__first))
        if (++__first == __last) return __first;

    _ForwardIter __next = __first;

    while (++__next != __last) {
        if (__pred(*__next)) {
            std::swap(*__first, *__next);
            ++__first;
        }
    }
    return __first;
}

int main(void)
{
    int iArray[10]={16, -1, 3, 11, 2, 5, 8, 20, 9, 4};
    int *result = partition(iArray, iArray+10, less10);
    cout << "按小于10进行分割" << endl;
    for_each(iArray, iArray+10, print);
    cout <<endl;
    cout << "partition number: " << *result <<endl;
    int iArray2[10]={16, -1, 3, 11, 2, 5, 8, 20, 9, 4};
    int *result2 = __partition(iArray2, iArray2+10, less10);
    cout << "按小于10进行分割" << endl;
    for_each(iArray2, iArray2+10, print);
    cout <<endl;
    cout << "partition number: " << *result2 <<endl;
}

