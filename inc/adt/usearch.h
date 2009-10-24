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

};

}; // namespace ADT

}; // namespace huys

#endif // U_SEARCH_H

