#include <windows.h>

#include "colors.h"
#include "ugdi.h"

namespace huys
{

UShape::UShape()
{
    bIsSelected = false;
    LinePen.lopnWidth.x = 1;
    LinePen.lopnColor = huys::black;
    LinePen.lopnStyle = PS_SOLID;
    FilledBrush.lbColor = huys::white;
    FilledBrush.lbHatch = HS_HORIZONTAL;
    FilledBrush.lbStyle = BS_NULL;
}

UShape::~UShape()
{}

void UShape::setLineColor(COLORREF color)
{
    this->LinePen.lopnColor = color;
}

void UShape::setLineWidth( int width )
{
    this->LinePen.lopnWidth.x = width;
}

void UShape::setLineStyle( UINT style )
{
    this->LinePen.lopnStyle = style;
}

void UShape::setFilledColor( COLORREF color )
{
    this->FilledBrush.lbColor = color;
}

void UShape::setFilledHatch( ULONG hatch )
{
    this->FilledBrush.lbHatch = hatch;
}

void UShape::setFilledStyle( UINT style )
{
    this->FilledBrush.lbStyle = style;
}

//
ULine::ULine(int x, int y, int x2, int y2)
: UShape()
{
    m_nStartX = x;
    m_nStartY = y;
    m_nEndX = x2;
    m_nEndY = y2;
}

ULine::ULine(POINT ptStart, POINT ptEnd)
: UShape()
{
    this->SetStartPnt(ptStart);
    this->SetEndPnt(ptEnd);
}

//
ULine::~ULine()
{
};

void ULine::SetStartPnt(POINT start)
{
    this->m_nStartX = start.x;
    this->m_nStartY = start.y;
}

void ULine::SetStartPnt(int x, int y)
{
    this->m_nStartX = x;
    this->m_nStartY = y;
}

void ULine::SetEndPnt(POINT end)
{
    this->m_nEndX = end.x;
    this->m_nEndY = end.y;
}

void ULine::SetEndPnt(int x, int y)
{
    this->m_nEndX = x;
    this->m_nEndY = y;
}

void ULine::Draw(HDC hdc)
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);

    ::MoveToEx(hdc, m_nStartX, m_nStartY, NULL);
    ::LineTo(hdc, m_nEndX, m_nEndY);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);
}

void ULine::offsetX( int x )
{
    this->m_nStartX += x;
    this->m_nEndX += x;
}

void ULine::offsetY( int y )
{
    this->m_nStartY += y;
    this->m_nEndY += y;
}
//
URectangle::URectangle(int x, int y, int x2, int y2)
: UShape()
{
    m_nStartX = x;
    m_nStartY = y;
    m_nEndX = x2;
    m_nEndY = y2;
}

URectangle::URectangle(POINT start, POINT end)
: UShape()
{
    this->SetStartPnt(start);
    this->SetEndPnt(end);
}

URectangle::URectangle( const RECT &rc )
: UShape()
{
    m_nStartX = rc.left;
    m_nStartY = rc.top;
    m_nEndX = rc.right;
    m_nEndY = rc.bottom;
}
//
URectangle::~URectangle()
{
};

void URectangle::SetStartPnt(POINT start)
{
    this->m_nStartX = start.x;
    this->m_nStartY = start.y;
}

void URectangle::SetStartPnt(int x, int y)
{
    this->m_nStartX = x;
    this->m_nStartY = y;
}

void URectangle::SetEndPnt(POINT end)
{
    this->m_nEndX = end.x;
    this->m_nEndY = end.y;
}

void URectangle::SetEndPnt(int x, int y)
{
    this->m_nEndX = x;
    this->m_nEndY = y;
}

void URectangle::SetRect(const RECT &rc)
{
    m_nStartX = rc.left;
    m_nStartY = rc.top;
    m_nEndX = rc.right;
    m_nEndY = rc.bottom;
}

void URectangle::Draw(HDC hdc)
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);

    ::Rectangle(hdc, m_nStartX, m_nStartY, m_nEndX, m_nEndY);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);
}

void URectangle::moveCenterTo( int x, int y )
{
    int dx = (m_nEndX-m_nStartX)/2;
    int dy = (m_nEndY-m_nStartY)/2;
    this->m_nStartX = x - dx;
    this->m_nEndX =  x + dx;
    this->m_nStartY = y - dy;
    this->m_nEndY =  y + dy;
}

void URectangle::shrinkTo( int nWidth, int nHeight )
{
    this->m_nEndX = this->m_nStartX + nWidth;
    this->m_nEndY = this->m_nStartY + nHeight;
}

void URectangle::offset( int nX, int nY /*= 0*/ )
{
    this->m_nStartX += nX;
    this->m_nEndX += nX;
    this->m_nStartY += nY;
    this->m_nEndY += nY;
}

UAngleArc::UAngleArc( int centerX/*=0*/,
                        int centerY/*=0*/,
                        DWORD radius/*=1*/,
                        FLOAT startAngle/*=0.0f*/,
                        FLOAT sweepAngle/*=10.0f*/ )
: UShape()
{
    m_nCenterX = centerX;
    m_nCenterY = centerY;
    m_dwRadius = radius;
    m_fStartAngle = startAngle;
    m_fSweepAngle = sweepAngle;
}

UAngleArc::UAngleArc( POINT center,
                      DWORD radius/*=1*/,
                      FLOAT startAngle/*=0.0f*/,
                      FLOAT sweepAngle/*=10.0f*/ )
: UShape()
{
    m_nCenterX = center.x;
    m_nCenterY = center.y;
    m_dwRadius = radius;
    m_fStartAngle = startAngle;
    m_fSweepAngle = sweepAngle;
}

UAngleArc::~UAngleArc()
{}

void UAngleArc::Draw( HDC hdc )
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);

    ::BeginPath(hdc);

    ::MoveToEx(hdc, m_nCenterX, m_nCenterY, NULL);

    ::AngleArc(hdc, m_nCenterX, m_nCenterY, m_dwRadius,
        m_fStartAngle, m_fSweepAngle);

    ::LineTo(hdc, m_nCenterX, m_nCenterY);

    ::EndPath(hdc);

    ::StrokeAndFillPath(hdc);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);
}

void UAngleArc::setCenter( int x, int y )
{
    m_nCenterX = x;
    m_nCenterY = y;
}

void UAngleArc::setRadius( DWORD radius )
{
    m_dwRadius = radius;
}

void UAngleArc::setStartAngle( FLOAT angle )
{
    m_fStartAngle = angle;
}

void UAngleArc::setSweepAngle( FLOAT angle )
{
    m_fSweepAngle = angle;
}

UMarker::UMarker( int left, int top, int right, int bottom )
: UShape()
{
    m_nLeft     = left;
    m_nTop      = top;
    m_nRight    = right;
    m_nBottom   = bottom;
}

UMarker::UMarker( POINT leftTop, POINT rightBottom )
: UShape()
{
    m_nLeft   = leftTop.x;
    m_nTop    = leftTop.y;
    m_nRight  = rightBottom.x;
    m_nRight  = rightBottom.y;
}

UMarker::~UMarker()
{}

void UMarker::Draw( HDC hdc )
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);

    ::MoveToEx(hdc, m_nLeft, m_nTop, NULL);
    ::LineTo(hdc, m_nRight, m_nBottom);
    ::MoveToEx(hdc, m_nRight, m_nTop, NULL);
    ::LineTo(hdc, m_nLeft, m_nBottom);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);

}


//
UPolyLine::UPolyLine( POINT *pts, int num )
{
    m_pts = pts;
    m_num = num;
}

UPolyLine::~UPolyLine()
{

}

void UPolyLine::Draw( HDC hdc  )
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);
    ::Polyline(hdc, m_pts, m_num);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);
}

BOOL UPolyLine::setPoints( POINT *pts, int num )
{
    m_pts = pts;
    m_num = num;
    return TRUE;
}


UTriangle::UTriangle( int x1, int y1, int x2, int y2, int x3, int y3 )
{
    m_upts[0].moveTo(x1, y1);
    m_upts[1].moveTo(x2, y2);
    m_upts[2].moveTo(x3, y3);
}

UTriangle::UTriangle( const LPPOINT p1, const LPPOINT p2, const LPPOINT p3 )
{
    m_upts[0] = p1;
    m_upts[1] = p2;
    m_upts[2] = p3;
}

void UTriangle::Draw( HDC hdc )
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);

    POINT pt[3] = {
        m_upts[0].x(), m_upts[0].y(),
        m_upts[1].x(), m_upts[1].y(),
        m_upts[2].x(), m_upts[2].y()
    };

    ::Polygon(hdc, pt, 3);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);
}

UCircle::UCircle( int centerX/*=0*/,
                  int centerY/*=0*/,
                   DWORD radius/*=1*/)
: UShape()
{
    m_nCenterX = centerX;
    m_nCenterY = centerY;
    m_dwRadius = radius;
}

void UCircle::Draw( HDC hdc )
{
    HPEN hNewPen;
    hNewPen = ::CreatePenIndirect(&(this->LinePen));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hNewPen);

    HBRUSH hNewBrush;
    hNewBrush = ::CreateBrushIndirect(&(this->FilledBrush));
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hNewBrush);

    ::Ellipse(hdc, m_nCenterX-m_dwRadius, m_nCenterY-m_dwRadius, 
                   m_nCenterX+m_dwRadius, m_nCenterY+m_dwRadius);

    ::SelectObject(hdc, hOldPen);
    ::SelectObject(hdc, hOldBrush);

    ::DeleteObject(hNewPen);
    ::DeleteObject(hNewBrush);
}

void UCircle::setCenter( int x, int y )
{
    m_nCenterX = x;
    m_nCenterY = y;
}

void UCircle::setRadius( DWORD radius )
{
    m_dwRadius = radius;
}

}; // namespace huys
