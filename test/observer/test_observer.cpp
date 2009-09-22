#include <iostream>

using std::cout;
using std::endl;

#include "adt/uvector.h"

class Subject
{
    typedef huys::ADT::UVector< class Observer *> UVec;

    // 1. "independent" functionality
private:
    UVec views; // 3. Coupled only to "interface"
    int value;
public:
    void attach(Observer *obs)
    {
        views.push_back(obs);
    }

    void setVal(int val)
    {
        value = val;
        notify();
    }

    int getVal()
    {
        return value;
    }
private:
    void notify();
};

class Observer
{
    // 2. "dependent" functionality
    Subject *model;
    int denom;
public:
    Observer(Subject *mod, int div)
    {
        model = mod;
        denom = div;
        // 4. Observers register themselves with the Subject
        model->attach(this);
    }
    virtual void update() = 0;
protected:
    Subject *getSubject()
    {
        return model;
    }

    int getDivisor()
    {
        return denom;
    }
};

void Subject::notify()
{
    // 5. Publisher broadcasts
    //for (int i = 0; i < views.size(); i++)
    //{
    //    views[i]->update();
    //}
    Subject::UVec::const_iterator iter;
    for (iter = views.begin(); iter != views.end(); ++iter)
    {
        (*iter)->update();
    }
}

class DivObserver: public Observer
{
public:
    DivObserver(Subject *mod, int div): Observer(mod, div){}

    void update()
    {
        // 6. "Pull" information of interest
        int v = getSubject()->getVal();
        int d = getDivisor();
        cout << v << " div " << d << " is " << v / d << '\n';
    }
};

class ModObserver: public Observer
{
public:
    ModObserver(Subject *mod, int div): Observer(mod, div){}
    void update()
    {
        int v = getSubject()->getVal();
        int d = getDivisor();
        cout << v << " mod " << d << " is " << v % d << '\n';
    }
};


int main()
{
    Subject subj;
    DivObserver divObs1(&subj, 4); // 7. Client configures the number and
    DivObserver divObs2(&subj, 3); //    type of Observers
    ModObserver modObs3(&subj, 3);
    subj.setVal(14);
    return 0;
}

