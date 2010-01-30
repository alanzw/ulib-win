#ifndef U_SORT_H
#define U_SORT_H

#include <cstring>

namespace huys
{

namespace ADT
{

namespace Sort
{

/*
 * Bubble sort:  exchange & O(n^2)
 *
 */
template <class T>
void bubble_sort(T *array, int n)
{
    int lo = 0;
    int up = n - 1;
    int i, j;
    while (up>lo)
    {
        j = lo;
        for ( i=lo; i<up; i++ )
        {
            if ( array[i] > array[i+1] )
            {
                T temp = array[i+1];
                array[i+1] = array[i];
                array[i] = temp;
                j = i;
            }
        }

        up = j;
        for ( i=up; i>lo; i-- )
        {
            if ( array[i] < array[i-1] )
            {
                T temp = array[i-1];
                array[i-1] = array[i];
                array[i] = temp;
                j = i;
            }
        }
        lo = j;

     }
}

//
template <class T>
void bubble_sort2(T *array, int n)
{
    int i, j;
    T temp;
    int test; /*use this only if unsure whether the list is already sorted or not*/
    for(i = n-1; i>0; i--)
    {
        test=0;
        for(j=0; j<i; j++)
        {
            if(array[j] > array[j+1]) /* compare neighboring elements */
            {
                temp = array[j];    /* swap array[j] and array[j+1] */
                array[j] = array[j+1];
                array[j+1] = temp;
                test=1;
            }
        } /*end for j*/
        if(test==0) break; /*will exit if the list is sorted!*/
    } /*end for i*/

}/*end bubbleSort*/

//
template <class T>
void selection_sort(T *array, int n)
{
    int i, j, min;
    for (i = 0; i<(n-1); i++)
    {
        min = i;
        for (j=(i+1); j<n; j++)
        {
            if(array[j] < array[min])
            {
                min = j;
            }
        }
        if (i != min)
        {
            T temp = array[i];
            array[i] = array[min];
            array[min] = temp;
        }
    }
}

//
template <class T>
void insertion_sort(T *array, int n)
{
  for (int j = 2; j < n; j++)
  {
    for (int k = 0; k < j; k++)
    {
      if (array[j] < array[k])
      {
        T temp = array[k];
        array[k] = array[j];
        array[j] = temp;
      }
    }
  }
}

/*
 * radix sort is a sorting algorithm that sorts integers by processing
 * individual digits, by comparing individual digits sharing the same
 * significant position.
 *
 * Two classifications of radix sorts are:
 *   - least significant digit (LSD) radix sorts
 *   - most significant digit (MSD) radix sorts
 */
template <class T>
void radix (int byte, int n, T *source, T *dest)
{
  long count[256];
  long index[256];
  int i = 0;
  memset(count, 0, sizeof (count));
  for ( i=0; i<n; i++ ) count[((source[i])>>(byte*8))&0xff]++;

  index[0]=0;
  for ( i=1; i<256; i++ ) index[i]=index[i-1]+count[i-1];
  for ( i=0; i<n; i++ ) dest[index[((source[i])>>(byte*8))&0xff]++] = source[i];
}

//
template <class T>
void radix_sort(T *array, T *temp, int n)
{
    radix (0, n, array, temp);
    radix (1, n, temp, array);
    radix (2, n, array, temp);
    radix (3, n, temp, array);
}


//
template <class T>
void gnome_sort(T *array, int n)
{
    int i = 0;

    while (i < n)
    {
        if (i == 0 || array[i-1] <= array[i])
        {
            i++;
        }
        else
        {
            int tmp = array[i];
            array[i] = array[i-1];
            array[--i] = tmp;
        }
    }
}

//
template <class T>
void heap_sort(T *array, int n)
{
    int i, j;
    int lChild, rChild, mChild, root, temp;

    //
    root = (n-2)/2;
    //creating the heap
    for(j=root; j>=0; j--)
    {
        for(i=root; i>=0; i--)
        {
            lChild = (2*i)+1;
            rChild = (2*i)+2;

            if ((lChild <= n-1) && (rChild <= n-1))
            {
                if(array[rChild] >= array[lChild])
                    mChild = rChild;
                else
                    mChild = lChild;
            }
            else
            {
                if(rChild > n-1)
                    mChild = lChild;
                else
                    mChild = rChild;
            }

            //swap elements
            if (array[i] < array[mChild])
            {
                temp = array[i];
                array[i] = array[mChild];
                array[mChild] = temp;
            }
        }
    }

    // move the max element to the end of the array
    temp = array[0];
    array[0] = array[n-1];
    array[n-1] = temp;
}

//
template <class T>
void merge_sort(T * array, int n)
{
    // if the array has more than 1 element, we need to split it and merge the sorted halves
    if(n > 1)
    {
        // number of elements in sub-array 1
        // if odd, sub-array 1 has the smaller half of the elements
        // e.g. if 7 elements total, sub-array 1 will have 3, and sub-array 2 will have 4
        int elementsInA1 = n/2;
        // since we want an even split, we initialize the length of sub-array 2 to
        // equal the length of sub-array 1
        int elementsInA2 = elementsInA1;
        // if the array has an odd number of elements, let the second half take the extra one
        // see note (1)
        if((n % 2) == 1)
            elementsInA2 += 1;
        // declare and initialize the two arrays once we've determined their sizes
        T *arr1 = new T[elementsInA1];
        T *arr2 = new T[elementsInA2];
        // copy the first part of 'array' into 'arr1', causing arr1 to become full
        for(int i = 0; i < elementsInA1; i++)
            arr1[i] = array[i];
        // copy the remaining elements of 'array' into 'arr2', causing arr2 to become full
        for(int i = elementsInA1; i < elementsInA1 + elementsInA2; i++)
            arr2[i - elementsInA1] = array[i];
        // recursively call mergeSort on each of the two sub-arrays that we've just created
        // note: when mergeSort returns, arr1 and arr2 will both be sorted!
        // it's not magic, the merging is done below, that's how mergesort works :)
        merge_sort<T>(arr1, elementsInA1);
        merge_sort<T>(arr2, elementsInA2);

        // the three variables below are indexes that we'll need for merging
        // [i] stores the index of the main array. it will be used to let us
        // know where to place the smallest element from the two sub-arrays.
        // [j] stores the index of which element from arr1 is currently being compared
        // [k] stores the index of which element from arr2 is currently being compared
        int i = 0, j = 0, k = 0;
        // the below loop will run until one of the sub-arrays becomes empty
        // in my implementation, it means until the index equals the length of the sub-array
        while(elementsInA1 != j && elementsInA2 != k)
        {
            // if the current element of arr1 is less than or equal to current element of arr2
            if(arr1[j] <= arr2[k])
            {
                // copy the current element of arr1 into the final array
                array[i] = arr1[j];
                // increase the index of the final array to avoid replacing the element
                // which we've just added
                i++;
                // increase the index of arr1 to avoid comparing the element
                // which we've just added
                j++;
            }
            // if the current element of arr2 is less than current element of arr1
            else
            {
                // copy the current element of arr1 into the final array
                array[i] = arr2[k];
                // increase the index of the final array to avoid replacing the element
                // which we've just added
                i++;
                // increase the index of arr2 to avoid comparing the element
                // which we've just added
                k++;
            }
        }
        // at this point, one of the sub-arrays has been exhausted and there are no more
        // elements in it to compare. this means that all the elements in the remaining
        // array are the highest (and sorted), so it's safe to copy them all into the
        // final array.
        while(elementsInA1 != j)
        {
            array[i] = arr1[j];
            i++;
            j++;
        }
        while(elementsInA2 != k)
        {
            array[i] = arr2[k];
            i++;
            k++;
        }

        delete [] arr1;
        delete [] arr2;
    }
}

/// QuickSort
template <typename T>
T *quicksort_basic(T *array, int lo, int hi)
{
    if (hi <= lo)
    {
        return array;
    }

//void quicksort_basic (int[] a, int lo, int hi)
//{
//  lo is the lower index, hi is the upper index
//  of the region of array a that is to be sorted
    //int i=lo, j=hi, h;
    //int x=a[(lo+hi)/2];
    int i = lo;
    int j = hi;
    int h = 0;
    T x = array[(int)(lo/2+hi/2)];
    //  partition
    do
    {
        while (array[i]<x) i++;
        while (array[j]>x) j--;
        if (i<=j)
        {
            h=array[i];
            array[i]=array[j];
            array[j]=h;
            i++;
            j--;
        }
    } while (i<=j);

    //  recursion
    if (lo<j) quicksort_basic(array, lo, j);
    if (i<hi) quicksort_basic(array, i, hi);

    return array;
}


template <class T>
int partition(T *array, int p, int r)
{
    T x = array[r];
    int j = p - 1;
    for (int i=p; i<r; ++i)
    {
        if (x <= array[i])
        {
            ++j;
            std::swap(array[i], array[j]);
        }
    }
    array[r] = array[j+1];
    array[j+1] = x;

    return (j+1);
}


template <class T>
void quicksort_partition(T *array, int p, int r)
{
    if (p<r)
    {
        int q = partition<T>(array, p, r);
        quicksort_partition(array, p, q-1);
        quicksort_partition(array, q+1, r);
    }
}

}; // namespace Sort

}; // namespace ADT

}; // namespace huys

#endif // U_SORT_H
