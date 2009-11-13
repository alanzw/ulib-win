#ifndef U_DDP_H
#define U_DDP_H

#include "ucritical.h"
#include "usemaphore.h"

extern bool is_simulating;

enum {
    WM_STATE_CHANEG = WM_USER + 120
};

class UPhilosopher
{
    enum DDP_STATE {
        THINKING,
        HUNGRY,
        EATING
    };
public:
    UPhilosopher(UCriticalSection &cs, int index)
    : m_cs(cs), m_l(m_sem), _index(index)
    {
        m_sem.create(NULL, 1, 1, NULL);
    }

    ~UPhilosopher()
    {
    ;
    }
    
    int index() const
    { return _index; }

    void think()
    {
        Sleep(2000);
    }

    void eat()
    {
        Sleep(2000);
    }

    void test(UPhilosopher *p)
    {
        // SendMessage(STATE_CHANEG)
        if (p->isHungry() &&
            !p->left->isEating() &&
            !p->right->isEating())
        {
            p->m_state = EATING;
            // SendMessage(STATE_CHANEG)
            SendMessage(_hwnd, WM_STATE_CHANEG, (WPARAM)p, 0);
        }
        p->m_l.unlock();
    }
    
    void take_forks()
    {
        {
            UCriticalSection::scoped_lock lock(m_cs);
            m_state = HUNGRY;
            // SendMessage(STATE_CHANEG)
            SendMessage(_hwnd, WM_STATE_CHANEG, (WPARAM)this, 0);
            test(this);
        }
        m_l.lock();
    }

    void putdown_forks()
    {
        UCriticalSection::scoped_lock lock(m_cs);
        m_state = THINKING;
        // SendMessage(STATE_CHANEG)
        SendMessage(_hwnd, WM_STATE_CHANEG, (WPARAM)this, 0);
        test(left);
        test(right);
    }
    
    bool isHungry() const
    {
        return HUNGRY == m_state;
    }
    
    bool isEating() const
    {
        return EATING == m_state;
    }
    
    void set(UPhilosopher *l, UPhilosopher *r)
    {
        left = l;
        right = r;
    }
    
    void attach(HWND hwnd)
    {
        _hwnd = hwnd;
    }
private:
    UCriticalSection &m_cs;
    DDP_STATE m_state;
    USemaphore m_sem;
    USemaphore::Lock m_l;
    UPhilosopher *left;
    UPhilosopher *right;
    
    HWND _hwnd;
    int _index;
};


static void ThreadProc(LPVOID p)
{
    UPhilosopher *ph = reinterpret_cast<UPhilosopher *>(p);
    while (is_simulating)
    {
        ph->think();
        ph->take_forks();
        ph->eat();
        ph->putdown_forks();
    }
    //return 0;
}



#endif // U_DP_H
