/*
 * =====================================================================================
 *
 *       Filename:  ugo_kernel.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/20/2010 03:07:11 PM
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
using std::endl;

#include "ugo_kernel.h"

void UGoBoard::update()
{
    int row, col;
    int new_grid[maxrow+2][maxcol+2];
    for (row=1; row<=maxrow; ++row)
    {
        for (col=1; col<=maxcol; ++col)
        {
            new_grid[row][col] = 0; // Cell is now dead.
        }
    }

    for (row=1; row<=maxrow; ++row)
    {
        for (col=1; col<=maxcol; ++col)
        {
            grid[row][col] = new_grid[row][col];
        }
    }

}

void UGoBoard::initialize()
{
    int row, col;
    for (row=0; row<=maxrow+1; ++row)
    {
        for (col=0; col<=maxcol+1; ++col)
        {
            grid[row][col] = 0;
        }
    }
}

void UGoBoard::print()
{
    int row, col;
    cout << "\nThe current Life configuration is :" << endl;

    for (row=1; row<=maxrow; ++row)
    {
        for (col=1; col<=maxcol; ++col)
        {
            if (1 == grid[row][col])
            {
                cout << '*';
            }
            else
            {
                cout << '-';
            }
        }
        cout << endl;
    }
    cout << endl;
}

int UGoBoard::getState(int row, int col) const
{
    if (row <= 0 || row > maxrow || col <=0 || col>maxcol)
    {
        throw 1111;
    }

    return grid[row][col];
}

int UGoBoard::invertState(int row, int col)
{
    if (row <= 0 || row > maxrow || col <=0 || col>maxcol)
    {
        throw 1111;
    }

    grid[row][col] = !grid[row][col];

    return grid[row][col];
}

int UGoBoard::setState(int nState, int row, int col)
{
    if (row <= 0 || row > maxrow || col <=0 || col>maxcol || nState < 0 || nState > 2)
    {
        throw 1111;
    }

    grid[row][col] = nState;

    return grid[row][col];
}

