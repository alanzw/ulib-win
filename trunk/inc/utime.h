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

    WORD year() const
    {
        return m_st.wYear;
    }

    WORD month() const
    {
        return m_st.wMonth;
    }

    WORD day() const
    {
        return m_st.wDay;
    }

    WORD hour() const
    {
        return m_st.wHour;
    }

    WORD minute() const
    {
        return m_st.wMinute;
    }

    WORD second() const
    {
        return m_st.wSecond;
    }
protected:
private:
    SYSTEMTIME m_st;
};

#endif // U_TIME_H

