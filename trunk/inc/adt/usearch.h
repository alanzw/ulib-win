/*
 * =====================================================================================
 *
 *       Filename:  usearch.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  10/23/2009 03:30:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_SEARCH_H
#define U_SEARCH_H

#include <climits>

namespace huys
{

namespace ADT
{

namespace Search
{

template <class T>
int sequence_search(const T *array, int first, int last, T target)
{
    for (int i=first; i<=last; ++i)
    {
        if (array[i] == target) return i;
    }
    return -1;
}

template <class T>
int binary_search(const T * array, int first, int last, T target)
{
    int mid;
    T midValue;

    while (first < last)
    {
        mid = (first + last)/2;
        midValue = array[mid];
        if (target == midValue)
            return mid;
        else if (target < midValue)
            last = mid;
        else
            first = mid+1;
    }
    return -1;
}

/*
F0 = 0, F1 = 1
Fn = Fn-1+Fn-2 for n>=2.
function fibonacci_search(item: integer; arr: sort_array)return index
is
l : index := arr'first;-- first element of array
u : index := arr'last;-- last element of array
m : index := (u+l)/2;
x,a,b : integer;
begin
a := (Fn-3);
b := (Fn-2)-(Fn-3);
discrete (f2,f1) := (Fn-2,Fn-3)
    new (f2,f1) := (f2-f1,2*f1-f2) | (a,b)
with i := u-l+1
    new i=i/2loop
loop
    if item < arr(m)then
    m := m-f1;-- compute new position of compared element
    f2 := f2-f1;
    f1 := f1-f2;
    elsif item > arr(m)then
    m := m+f1;-- compute new position of compared element
    x := f1;
    f1 := f2-f1;
    f2 := x;
    a := f2; b := f1;
else
    return m;-- return index of found item
end if;
    i := i/2;
end loop;
end fibonacci_search;
*/

/*
 * If val is found in arr, return the index of its location in arr.
 * Otherwise, return the index of the smallest element greater than val
 */
static int binsrch_geq(const int *arr, int n, int val)
{
    register int low, high, mid;
    int geq;

    low=0; high=n-1; geq=-1;

    /* binary search for finding the element with value val */
    while(low <= high)
    {
        mid=(low+high)>>1; //(low+high)/2;
        if(val<arr[mid])
        {
            high=mid-1;
            geq=mid;
        }
        else if(val>arr[mid])
        {
            low=mid+1;
        }
        else
        {
            return mid; /* found */
        }
    }

    return geq; /* not found */
}

template <class T>
int fibonacci_search(const T * array, int first, int last, T target)
{
    int n = last - first + 1;
    register int k, idx, offs;
    static int prevn=-1, prevk=-1;
    /*  Precomputed Fibonacci numbers F0 up to F46. This implementation assumes that the size n
     *  of the input array fits in 4 bytes. Note that F46=1836311903 is the largest Fibonacci
     *  number that is less or equal to the 4-byte INT_MAX (=2147483647). The next Fibonacci
     *  number, i.e. F47, is 2971215073 and is larger than INT_MAX, implying that it does not
     *  fit in a 4 byte integer. Note also that the last array element is INT_MAX rather than
     *  F47. This ensures correct operation for n>F46.
     */
    const static int Fib[47+1]={0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765,
        10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578,
        5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155, 165580141, 267914296,
        433494437, 701408733, 1134903170, 1836311903, INT_MAX};

    /* find the smallest fibonacci number that is greater or equal to n. Store this
     * number to avoid recomputing it in the case of repetitive searches with identical n.
     */
    if(n!=prevn){
#if 1
        k=(n>1)? binsrch_geq(Fib, sizeof(Fib)/sizeof(int), n) : 1;
#else /* the above binary search call can be substituted by the following code fragment: */
        {
            register int f0, f1, t;

            for(f0=0, f1=1, k=1; f1<n; t=f1, f1+=f0, f0=t, ++k)
            ;
        }
#endif
        prevk=k;
        prevn=n;
    }
    else
    {
        k=prevk;
    }

    /* If the sought value is larger than the largest Fibonacci number less than n,
     * care must be taken top ensure that we do not attempt to read beyond the end
     * of the array. If we do need to do this, we pretend that the array is padded
     * with elements larger than the sought value.
     */
    for(offs=0; k>0;  ){
        idx=offs+Fib[--k];

        /* note that at this point k  has already been decremented once */
        if(idx>=n || target < array[idx]) // index out of bounds or val in 1st part
        {
            continue;
        }
        else if (target > array[idx]) // val in 2nd part
        {
            offs=idx;
            --k;
        }
        else // val==arr[idx], found
        {
            return idx;
        }
    }

    return -1; // not found
}

template <class T>
int interpolation_search(const T * array, int first, int last, T target)
{
    int low = first;
    int high = last;

    int mid;

    while ( array[low] < target && array[high] >= target)
    {
        mid = low + ((target - array[low]) * (high - low)) / (array[high] - array[low]);

        if (array[mid] < target)
        {
            low = mid + 1;
        }
        else if (array[mid] > target)      // Repetition of the comparison code is forced by syntax limitations.
        {
            high = mid - 1;
        }
        else
        {
            return mid;
        }
    }

    if (array[low] == target)
    {
        return low;
    }
    return -1;
}

};

}; // namespace ADT

}; // namespace huys

#endif // U_SEARCH_H

