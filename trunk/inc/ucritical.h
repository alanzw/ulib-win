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

    class scoped_lock
    {
    public:
        scoped_lock(UCriticalSection &cs)
        : _cs(cs)
        {
            _cs.lock();
        }

        ~scoped_lock()
        {
            _cs.unlock();
        }
    private:
        UCriticalSection &_cs;
    };
private:
    CRITICAL_SECTION m_cs;
};

#endif // U_CRITICAL_SECTION_H
