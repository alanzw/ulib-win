/*
 * =====================================================================================
 *
 *       Filename:  test_templmethod.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/02/09 05:11:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <iostream>
using namespace std;

class Base
{
    void a()
    {
        cout << "a  ";
    }
    void c()
    {
        cout << "c  ";
    }
    void e()
    {
        cout << "e  ";
    }
    // 2. Steps requiring peculiar implementations are "placeholders" in base class
    virtual void ph1() = 0;
    virtual void ph2() = 0;
public:
    // 1. Standardize the skeleton of an algorithm in a base class "template method"
    void execute()
    {
        a();
        ph1();
        c();
        ph2();
        e();
    }
};

class One: public Base
{
    // 3. Derived classes implement placeholder methods
    /*virtual*/void ph1()
    {
        cout << "b  ";
    }
    /*virtual*/void ph2()
    {
        cout << "d  ";
    }
};

class Two: public Base
{
    /*virtual*/void ph1()
    {
        cout << "2  ";
    }
    /*virtual*/void ph2()
    {
        cout << "4  ";
    }
};

int main()
{
    Base *array[] =
    {
        &One(), &Two()
    };
    for (int i = 0; i < 2; i++)
    {
        array[i]->execute();
        cout << '\n';
    }
    return 0;
}

