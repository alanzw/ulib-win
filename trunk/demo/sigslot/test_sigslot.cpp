#include "resource.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "usigslot.h"

class Light : public USigSlot::has_slots
{
public:
    Light(int aIndex)
    : m_nIndex(aIndex)
    {}

    void ToggleState() { std::cout << m_nIndex << " : Toggle State" << std::endl; };
    void TurnOn() { std::cout << m_nIndex << " : Turn on" << std::endl; };
    void TurnOff() {std::cout << m_nIndex << " : Turn off" << std::endl; };
private:
    int m_nIndex;
};


#include <setjmp.h>

struct __Signal {
   jmp_buf environment;
   struct __Slot *slot;
};

struct __Slot {
   jmp_buf environment;
   struct __Signal *signal;

}; 

void __signal_connect(struct __Signal *signal, struct __Slot *slot)
{
  signal->slot = slot;
  slot->signal = signal;
}

void __signal_emit(struct __Signal *signal)
{
    if (setjmp(signal->environment) == 0)
    {
        longjmp(signal->slot->environment, 1);
    }

}

class Switch
{
public:
    USigSlot::signal0 Clicked;
};

int main(int argc, char *argv[])
{
    Switch sw1;
    Light lp1(1), lp2(2);

    sw1.Clicked.connect(&lp1, &Light::ToggleState);
    sw1.Clicked.connect(&lp2, &Light::ToggleState);


    sw1.Clicked();

    USigSlot::signal0 sig;

    sig.connect(&lp1, &Light::TurnOn);

    sig.emit();

    sig.disconnect(&lp1);

    sig.connect(&lp2, &Light::TurnOff);

    sig.emit();
    
    struct {
        struct __Signal signal;
        int _1;
        float _2;

    } signal;
    
    struct __Slot slot;

    __signal_connect((struct __Signal *)&signal, &slot);
    if (setjmp(slot.environment) != 0)
    {
        printf("int=%d, float=%f\n", signal._1, signal._2);
        exit(0);
    }

    signal._1 = 5;
    signal._2 = 10.0;
    __signal_emit((struct __Signal *)&signal);  // 调用处只需要signal的信息 

    std::cin.ignore();
    return 0;
}
