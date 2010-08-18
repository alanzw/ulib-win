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
/************************************************************************
BLACK_BRUSH
    Black brush.

DKGRAY_BRUSH
    Dark gray brush.

DC_BRUSH
    Solid color brush. The default color is white.
    The color can be changed by using the SetDCBrushColor function.
    For more information, see the Remarks section.

GRAY_BRUSH
    Gray brush.

HOLLOW_BRUSH
    Hollow brush (equivalent to NULL_BRUSH).

LTGRAY_BRUSH
    Light gray brush.

NULL_BRUSH
    Null brush (equivalent to HOLLOW_BRUSH).

WHITE_BRUSH
    White brush.
************************************************************************/
    HBRUSH getStockBrush(int fnObject);
private:

};

#endif // U_BRUSH_H
