#ifndef U_TIME_H
#define U_TIME_H

#include "ulib.h"

class ULIB_API UTimer
{
public:
    UTimer();
    ~UTimer();
    void begin();
    void stop();
    double getSeconds();
    void clear();

#ifdef WINVER
    void beginQuery();
    void stopQuery();
    double getSecondsQuery();
#endif
protected:
private:
    long start_count;
    long end_count;
    LONGLONG freqence;
};

#endif // U_TIME_H

