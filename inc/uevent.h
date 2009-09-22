#ifndef U_EVENT_H
#define U_EVENT_H

#include "uobject.h"

class ULIB_API UEvent : public UKernelObject
{
public:
    UEvent(LPCTSTR name);
    bool create();
    bool open();

    bool set();
    bool reset();
private:
    TCHAR m_sName[MAX_PATH];
};

#endif // U_EVENT_H
