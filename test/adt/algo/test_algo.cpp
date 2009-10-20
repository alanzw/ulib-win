#include <iostream>
#include <memory>
#include <string.h>
using std::cout;
using std::endl;
using std::cin;

#include "adt/ustack.h"

typedef huys::ADT::UStack<int> StackInt;

int w[10]={1,3,2,4,5,6,7,8,9,10};

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
    int length=n;
    int f;
    int i=n;
    int ans=0;
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
    knap(50, 10);

    knap2(50, 10);

    floyd();
    
    cin.ignore();

    return 0;
}
