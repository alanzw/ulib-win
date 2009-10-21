#include <iostream>
#include <cmath>
#include "adt/ustring.h"

int CheckPrime(int i)
{
    bool isPrimeNumberFound = true;
    bool isNextPrimeNumberFound = false;

    //Get Square root of number and iterate, start from 2
    int check = (int)sqrt((double)(i));
    for (int j = 2; j <= check; j++)
    {
        if (i % j == 0)
        {
            isPrimeNumberFound = false;
            break;
        }
    }
    if (isPrimeNumberFound)
    {
        //MessageBox.Show(i + " is a prime number");
        return i;
    }
    else
    { 
        //MessageBox.Show(i + " is not a prime number"); 
    }

    i++;
    isPrimeNumberFound = true;
    // Increment current number to find next prime number
    while (isNextPrimeNumberFound == false)
    {
        check = (int)sqrt((double)(i));
        for (int j = 2; j <= check; j++)
        {
            if (i % j == 0)
            {
                isPrimeNumberFound = false;
                break;
            }
        }
        if (isPrimeNumberFound)
        { 
            isNextPrimeNumberFound = true;
        }
        else
        { 
            i++; 
            isPrimeNumberFound = true;
        }
    }

    //if (isNextPrimeNumberFound) 
    //{ 
        //MessageBox.Show(i + " is the next prime number");
    //}
    return i;
}



int main()
{
    using std::cout;
    using std::endl;


    typedef huys::ADT::UStringAnsi UString;

    cout << "=========================" << endl;

    UString s("1111");

    cout << s.c_str() << endl;

    cout << s.substr(1, 2).c_str() << endl;

    cout << "=========================" << endl;


    int m = 100;
    printf("Find primes up to: %d\n", m);

    //create prime list
    int prime[101];
    int i, j, k;

    //fill list with 0 - prime
    for(i = 2; i <= m; i++)
    {
        prime[i] = 0;
    }

    //set 0 and 1 as not prime
    prime[0]=1;
    prime[1]=1;

    //find primes then eliminate their multiples (0 = prime, 1 = composite)
    for(i=2; i <= (int)sqrt(m)+1; i++)
    {
        if(prime[i] == 0)
        {
            j = i;
            for(k = 2*j; k <= m+1; k = k+j)
            {
                prime[k] = 1;
            }
        }
    }

    //print primes
    for(i = 0; i < m+1; i++)
    {
        if(prime[i] == 0)
        {
            printf("%i\n", i);
        }
    }


    cout << "find next largest prime number of 100 : " << CheckPrime(100) << endl;
    return 0;
}
