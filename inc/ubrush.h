#ifndef U_BRUSH_H
#define U_BRUSH_H

#include "uobject.h"
#include "colors.h"

class ULIB_API UBrush : public UGDIObject
{
public:
    UBrush();

    ~UBrush();
    
    operator HBRUSH()
    {
        return (HBRUSH)m_hObj;
    }
    
    HBRUSH createSolidBrush(huys::Color clr);
private:
    
};

#endif // U_BRUSH_H
