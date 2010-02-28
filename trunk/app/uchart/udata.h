#ifndef U_CHART_DATA_H
#define U_CHART_DATA_H

#include "adt/uvector.h"

template <typename T>
class UChartData
{
public:
    UChartData()
    {}


    void add(long *pdata, int n)
    {
        _data.append(pdata, pdata+n);
    }
private:
    huys::ADT::UVector<T> _data;
};

#endif // U_CHART_DATA_H
