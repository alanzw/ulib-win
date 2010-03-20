/*
 * =====================================================================================
 *
 *       Filename:  console_life.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/20/2010 03:25:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::flush;

#include "ulife_kernel.h"


bool user_say_yes()
{
    int c;
    bool initial_response = true;
    do {
        if (initial_response)
        {
            cout << " (y,n?) " << flush;
        }
        else
        {
            cout << "Response with either y or n: " << flush;
        }

        do {
            c = cin.get();
        } while (c == '\n' || c == ' ' || c == '\t');

        initial_response = false;

    } while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');

    return (c == 'y' || c == 'Y');
}

int main()
{
    ULifeBoard config;
    //
    config.initialize();
    config.print();

    cout << "Continue viewing new generation? " << endl;
    while (user_say_yes())
    {
        config.update();
        config.print();
        cout << "Continue viewing new generation? " << endl;
    }

    return 0;
}

