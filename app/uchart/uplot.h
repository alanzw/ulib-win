#ifndef U_PLOT_H
#define U_PLOT_H

#include "adt/ustring.h"

typedef huys::ADT::UStringAnsi UTString;

template <typename T, int dim>
struct UPosition
{
    T val[dim];
};

template <typename TV>
struct UAxisData
{
    TV start;
    TV end;
    TV interval;
    //
};

struct UTitle
{
    UTString title;
    UPosition<long, 2> pos; 
};

struct ULabel
{
    long tick;
    long start;
};

template <typename T>
class Plot2D
{
public:

private:
    UAxisData<T> axis_x;
    UAxisData<T> axis_y;

    UTitle _title_x;
    UTitle _title_y;
};

#endif // U_PLOT_H
