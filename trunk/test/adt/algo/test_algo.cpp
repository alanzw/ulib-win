#include <iostream>
#include <memory>
#include <cstring>

using std::cout;
using std::endl;
using std::cin;

#include "adt/ustack.h"

typedef huys::ADT::UStack<int> StackInt;

/*
 * the knapsack problem
 *
 * description:
 *   Given n items, each of integer size ki $(1 \le i \le n)$, and a knap-sack of size K. 
 *   1) determine whether there is a subset of the items that sums to K.
 *   2) Find such a subset.
 */
namespace AP_KnapSack
{

int w[10] = {1,3,2,4,5,6,7,8,9,10};

int knap(int s, int n)
{
    if( s == 0)
    {
        return 1;
    }
    else if (s<0 || s>0 && n <1)
    {
        return 0;
    }
    else if ( knap( s-w[n-1], n-1) == 1)
    {
        cout << n << endl;
        return 1;
    }
    else
    {
        return knap(s, n-1);
    }
}

int knap2(int T, int n)
{
    int length = n;
    int f;
    int i = n;
    int ans = 0;
    StackInt st;
    while(n>0)
    {
        for( ;i>0;i--)
        {
            if(T>=w[i-1])
            {
                st.push(i);
                T=T-w[i-1];
            }
        }

        //
        if( st.size()==length && T>0 || st.empty() && ans==0)
        {
            //cout << ans;
            cout << " no solution! \n";
            return -1;
        }

        if(st.empty())
            return -2;

        if(T==0)
        {
            ++ans;
            cout << "Solution: " << ans << endl;
            cout << st;
        }
        i = st.pop();
        T += w[i-1];
        if(i==n)
        {
            i=n=n-1;
        }
        else
        {
            --i;
        }
    }
    cout << "Total: " << ans << endl;
    return 0;
}

struct Item
{
    int size;
    int value;
} items[] = {
    {1, 2},
    {2, 3}
};

int knapSV(int cap, int N) 
{
    int i, space, max, t;

    /* N is the number of item classes */
    for (i = 0, max = 0; i < N; i++) 
    { 

      /* 
       * ... figure out how much space would be left if we packed this
       * class of item (current free space minus this item requirement)
       */
      space = cap - items[i].size;

      /* if the space is still positive (i.e. we can pack it legally)... */
      if (space >= 0)
      {

        /* ... then do it and recurse on the new (less) amount of free space */
        t = knapSV(space, N) + items[i].value;

        if (t > max) 
        {
          max = t;
        }
      }
    }

   /*
    * if we did not pack anything (i.e. nothing will fit) max = 0 and
    * return zero.  Else max is the ``most valuable'' way we can pack
    * at this point and we return its value to the previous layer.  If
    * the USING_ITEM_VALUES macro is not defined, all items are worth
    * one and we are trying to maximize the number of items we can fit,
    * not the value of items we can fit.
    */
    return max;
}

enum {
    UNKNOWN = 0,
    KNOWN   = 1
};

int max_known[20];
Item item_known[20]; 

int knapSVD(int cap, int N)
{
    int i, space, max, maxi, t;

    /* the first thing we do is see if we have an answer to how to pack
     * items into cap space maximizing the value of the items.  If so we
     * do not need to do anything but return!
     */
    if (max_known[cap] != UNKNOWN) 
    {
        return max_known[cap];
    }

    /* if we got here we have not yet solved this problem, so lets do
     * it!
     */
    for (i = 0, max = 0; i < N; i++) 
    {

      /* see comments on recursive solution -- understand it before you
       * read this 
       */
      space = cap - items[i].size;
      if (space >= 0) 
      {
        t = knapSVD(space, N) + items[i].value;
        if (t > max) 
        {
          max = t;
          maxi = i;
        }
      }
    }

    /* now that we have packed the required space as well as we can,
     * remember how well we could do it.  Thus, if we are ever required to
     * do it again we can just use this saved value...
     */

    max_known[cap] = max;
    item_known[cap] = items[maxi];

    return max;
}



void test()
{
    knap(50, 10);

    knap2(50, 10);
}

}; // AP_KnapSack

int floyd()
{

#define NV 5
#define MA 9999999

    int graph[NV][NV]=
    {
        MA, 1, 7, 2, 8,
        1, MA, 3, 2, 2,
        7, 3, MA, 4, 4,
        2, 2, 4, MA, 3,
        8, 2, 4, 3, MA
    };
    int num=NV;
    int path[NV][NV];
    int i,j,k;
    for(i=0;i<num;i++)
    {
        for(j=0;j<num;j++)
            printf("%8d ",graph[i][j]);
        printf("\n");
    }
    printf("\n");
    //system("pause");

    memset(path,0,sizeof(path));
    for(i=0;i<num;i++)
        for(j=0;j<num;j++)
            path[i][j]=MA;

    for(i=0;i<num;i++)
    {
        for(j=0;j<num;j++)
        {
            path[i][j]=graph[i][j];
        }
    }

    for(k=0;k<num;k++)
        for(i=0;i<num;i++)
            for(j=0;j<num;j++)
                if(path[i][j]>graph[i][k]+graph[k][j])
                    path[i][j]=graph[i][k]+graph[k][j];

    for(i=0;i<num;i++)
        for(j=0;j<num;j++)
            printf("Shortest distance between <%d,%d>: %d \n",i+1,j+1,path[i][j]);
    //system("pause");
    return 0;
}


int main()
{
    AP_KnapSack::test();

    floyd();

    cin.ignore();

    return 0;
}
