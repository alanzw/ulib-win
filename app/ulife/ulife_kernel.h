/*
 * =====================================================================================
 *
 *       Filename:  ulife_kernel.h
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

#ifndef U_LIFE_KERNEL_H
#define U_LIFE_KERNEL_H

class ULifeGame
{
    enum {
        maxrow = 20,
        maxcol = 60
    };
public:
    void initialize();
    void print();
    void update();
private:
    int grid[maxrow+2][maxcol+2];
    int neighbor_count(int row, int col);
};


#endif // U_LIFE_KERNEL_H

