#ifndef HW_EVENT_H
#define HW_EVENT_H

#include "hwDefs.h"

typedef int hwEventType;

class hwEventHandler;

class HW_API hwEvent
{
private:
    hwEvent & operator=(const hwEvent &);
protected:
    hwEvent(const hwEvent &);
public:

protected:
    hwEventType m_eventType;

};

class HW_API hwEventHandler
{
public:
    hwEventHandler();
    virtual ~hwEventHandler();
};

#endif // HW_EVENT_H
