#ifndef U_ALGORITHM_H
#define U_ALGORITHM_H

#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

namespace huys
{

namespace ADT
{

using std::vector;
using std::string;
using std::find_if;
using std::string;
using std::equal;

// true if the argument is whitespace, false otherwise
bool space(char c)
{
    return isspace(c);
}

// false if the argument is whitespace, true otherwise
bool not_space(char c)
{
    return !isspace(c);
}

vector<string> split(const string& str)
{
    typedef string::const_iterator iter;
    vector<string> ret;

    iter i = str.begin();
    while (i != str.end()) {

        // ignore leading blanks
        i = find_if(i, str.end(), not_space);

        // find end of next word
        iter j = find_if(i, str.end(), space);

        // copy the characters in [i, j)
        if (i != str.end())
            ret.push_back(string(i, j));
        i = j;
    }
    return ret;
}

bool is_palindrome(const string& s)
{
    return equal(s.begin(), s.end(), s.rbegin());
}

bool not_url_char(char c)
{
    // characters, in addition to alphanumerics, that can appear in a URL
    static const string url_ch = "~;/?:@=&$-_.+!*'(),";

    // see whether c can appear in a URL and return the negative
    return !(isalnum(c) ||
             find(url_ch.begin(), url_ch.end(), c) != url_ch.end());
}

string::const_iterator
url_end(string::const_iterator b, string::const_iterator e)
{
    return find_if(b, e, not_url_char);
}

string::const_iterator
url_beg(string::const_iterator b, string::const_iterator e)
{
    static const string sep = "://";

    typedef string::const_iterator iter;

    // i marks where the separator was found
    iter i = b;

    while ((i = search(i, e, sep.begin(), sep.end())) != e) {

        // make sure the separator isn't at the beginning or end of the line
        if (i != b && i + sep.size() != e) {

            // beg marks the beginning of the protocol-name
            iter beg = i;
            while (beg != b && isalpha(beg[-1]))
                --beg;

            // is there at least one appropriate character before and after the separator?
            if (beg != i && !not_url_char(i[sep.size()]))
                return beg;
        }

        // the separator we found wasn't part of a URL advance i past this separator
        i += sep.size();
    }
    return e;
}

vector<string> find_urls(const string& s)
{
    vector<string> ret;
    typedef string::const_iterator iter;
    iter b = s.begin(), e = s.end();

    // look through the entire input
    while (b != e) {

        // look for one or more letters followed by ://
        b = url_beg(b, e);

        // if we found it
        if (b != e) {
            // get the rest of the URL
            iter after = url_end(b, e);

            // remember the URL
            ret.push_back(string(b, after));

            // advance b and check for more URLs on this line
            b = after;
        }
    }
    return ret;
}

/// bottom up method
int fibonacci_sequence(int n)
{
    int prev = 0;
    int cur = 1;

    int newV = 0;

    if (0 == n)
        return 0;
    else if (1 == n)
        return 1;

    for (int i = n; i > 1; --i)
    {
        newV = prev + cur;
        prev = cur;
        cur = newV;
    }

    return cur;
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

}; // namespace ADT

}; // namespace huys

#endif // U_ALGORITHM_H
