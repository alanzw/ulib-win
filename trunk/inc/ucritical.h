#ifndef U_CRITICAL_SECTION_H
#define U_CRITICAL_SECTION_H

#include "ulib.h"

/// Class UCriticalSection
/**
 * A critical section can be used only by the threads of a single process.
 */

class ULIB_API UCriticalSection
{
public:
    UCriticalSection();
    ~UCriticalSection();
    //
    bool lock();
    bool unlock();
private:
    CRITICAL_SECTION m_cs;
};

#endif // U_CRITICAL_SECTION_H
