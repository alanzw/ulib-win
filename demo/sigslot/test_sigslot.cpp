#include "resource.h"
#include <windows.h>
#include <iostream>

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

    std::cin.ignore();
    return 0;
}
