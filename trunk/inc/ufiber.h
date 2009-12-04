#ifndef U_FIBER_H
#define U_FIBER_H

#include "ulib.h"

class ULIB_API UFiber
{
public:
    UFiber(LPFIBER_START_ROUTINE lpRoutine, SIZE_T dwStackSize = 0);
    UFiber();
    ~UFiber();
    
    bool create(LPFIBER_START_ROUTINE lpRoutine, SIZE_T dwStackSize = 0);
    
    bool createEx(LPFIBER_START_ROUTINE lpRoutine, SIZE_T dwStackCommitSize = 0, SIZE_T dwStackReserveSize = 0);
    
    bool convertThreadToFiber();
    
    void attach(LPVOID p);
    
    LPVOID detach();
    
    LPVOID getFiber();
    
    void run();
private:
    LPVOID _fiber;
};

#endif // U_FIBER_H
