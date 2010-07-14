#ifndef U_PEN_H
#define U_PEN_H

#include "uobject.h"
#include "colors.h"

class ULIB_API UPen : public UGDIObject
{
public:
    UPen();

    ~UPen();

    operator HPEN()
    {
        return (HPEN)m_hObj;
    }

    HPEN createSolidPen(huys::Color clr);
    HPEN createPen(int nStyle, int nWidth, huys::Color clr);
/************************************************************************
BLACK_PEN
    Black pen.

DC_PEN
    Solid pen color. The default color is white.
    The color can be changed by using the SetDCPenColor function.
    For more information, see the Remarks section.

NULL_PEN
    NULL pen. The null pen draws nothing.

WHITE_PEN
    White pen.
************************************************************************/
    HPEN getStockPen(int fnObject);
private:

};

#endif // U_PEN_H
