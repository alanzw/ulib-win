#ifndef U_MAILSLOT_H
#define U_MAILSLOT_H

#include "uobject.h"

class ULIB_API UMailSlot : public UKernelObject
{
public:
    UMailSlot();
    ~UMailSlot();

    BOOL create(LPCTSTR sMailSlotName);
private:
    TCHAR m_sMailSlotName[256];
};

#endif // U_MAILSLOT_H
