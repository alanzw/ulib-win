/*
 * =====================================================================================
 *
 *       Filename:  ugo_kernel.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/20/2010 03:01:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_GO_KERNEL_H
#define U_GO_KERNEL_H

//#include <exception>

class UGoBoard
{
public:
    enum {
        maxrow = 19,
        maxcol = 19
    };
public:
    void initialize();
    void print();
    void update();

    int getState(int row, int col) const;
    int invertState(int row, int col);
    int setState(int nState, int row, int col);
private:
    int grid[maxrow+2][maxcol+2];
};


#endif // U_GO_KERNEL_H

