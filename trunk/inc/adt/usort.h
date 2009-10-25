#ifndef U_SORT_H
#define U_SORT_H

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



}; // namespace Sort

}; // namespace ADT

}; // namespace huys

#endif // U_SORT_H
