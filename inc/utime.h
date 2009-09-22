#ifndef U_TIME_H
#define U_TIME_H

#include "ulib.h"

class ULIB_API UTime
{
public:
    UTime();
    ~UTime();
    //
    void getSysTime();
    void getLocalTime();
    //
    BOOL setSysTime();
    BOOL setLocalTime();
    //
    operator LPSYSTEMTIME ()
    { return &m_st;}
protected:
private:
    SYSTEMTIME m_st;
};

#endif // U_TIME_H

