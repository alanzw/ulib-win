#ifndef U_DATETIMEPICK_H
#define U_DATETIMEPICK_H

#include "ucontrol.h"

class ULIB_API UDateTimePick : public UControl
{
public:
    UDateTimePick(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UDateTimePick();

    virtual BOOL create();
    
    BOOL setFormat(LPCTSTR sFormat);
    BOOL setMax();
    BOOL setMin();
protected:
private:
};

#endif // U_DATETIMEPICK_H

