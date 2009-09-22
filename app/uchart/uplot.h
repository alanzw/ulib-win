#ifndef U_PLOT_H
#define U_PLOT_H

template <typename T, int dim>
struct UPOSITION
{
    T val[dim];
};

template <typename TV>
struct UAxis
{
    TV start;
    TV end;
    TV interval;
    //
};

struct UTitle
{
    TCHAR *lpText;
};

template <typename T>
class Plot2D
{
public:
private:
    UAxis<T> axis_x;
    UAxis<T> axis_y;
};

#endif // U_PLOT_H
