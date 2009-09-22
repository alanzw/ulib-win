#ifndef U_DDP_H
#define U_DDP_H

#include "ucritical.h"

class UPhilosophers
{
    enum DDP_STATE {
        THINKING,
        HUNGRY,
        EATING
    };
public:
    UPhilosophers()
    {
    ;
    }

    ~UPhilosophers()
    {
    ;
    }

    void think()
    {
    ;
    }

    void eat()
    {
    }

    void take_forks()
    {

    }

    void putdown_forks()
    {


    }
private:
    UCriticalSection m_cs;
};

#endif // U_DP_H
