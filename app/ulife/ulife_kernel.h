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

//#include <exception>

class ULifeBoard
{
public:
    enum {
        maxrow = 10,
        maxcol = 10
    };
public:
    void initialize();
    void print();
    void update();

    int getState(int row, int col) const;
    int invertState(int row, int col);
private:
    int grid[maxrow+2][maxcol+2];
    int neighbor_count(int row, int col);
public:
 //   class ULifeException : public std::exception
 //   {
 //   public:
 //       ULifeException() {}
 //   };
};


#endif // U_LIFE_KERNEL_H

