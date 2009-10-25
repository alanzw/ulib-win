#ifndef U_SORT_H
#define U_SORT_H

#include <cstring>

namespace huys
{

namespace ADT
{

namespace Sort
{

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


template <class T>
void radix_sort(T *array, T *temp, int n)
{
	radix (0, n, array, temp);
	radix (1, n, temp, array);
	radix (2, n, array, temp);
	radix (3, n, temp, array);
}


}; // namespace Sort

}; // namespace ADT

}; // namespace huys

#endif // U_SORT_H
