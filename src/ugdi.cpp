#include "StdAfx.h"
#include "ugdi.h"
#include "colors.h"

namespace huys
{

//IMPLEMENT_SERIAL(UShape, CObject, 1)

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

void UShape::setLneWidth( int width )
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

void UShape::Serialize( CArchive &ar )
{
    CObject::Serialize(ar);
    if (ar.IsStoring())
    {
        ar >> LinePen.lopnWidth.x >> LinePen.lopnColor >> LinePen.lopnStyle
           >> FilledBrush.lbColor >> FilledBrush.lbHatch >> FilledBrush.lbStyle;
    }
    else
    {
        ar << LinePen.lopnWidth.x << LinePen.lopnColor << LinePen.lopnStyle
           << FilledBrush.lbColor << FilledBrush.lbHatch << FilledBrush.lbStyle;
    }
}
//

IMPLEMENT_SERIAL(ULine, CObject, 1)
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

//
void ULine::Serialize(CArchive &ar)
{
    UShape::Serialize(ar);
    if (ar.IsStoring())
    {
        ar >> m_nStartX >> m_nStartY >> m_nEndX >> m_nEndY;
    }
    else
    {
        ar << m_nStartX << m_nStartY << m_nEndX << m_nEndY;
    }
}

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

void ULine::Draw(CDC *pDC)
{
    CPen cNewPen;
    cNewPen.CreatePenIndirect(&(this->LinePen));
    CPen *pOldPen = pDC->SelectObject(&cNewPen);

    CBrush cNewBrush;
    cNewBrush.CreateBrushIndirect(&(this->FilledBrush));
    CBrush *poldBrush = pDC->SelectObject(&cNewBrush);

    pDC->MoveTo(m_nStartX, m_nStartY);
    pDC->LineTo(m_nEndX, m_nEndY); 

    pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldBrush);
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
IMPLEMENT_SERIAL(URectangle, CObject, 1)
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

URectangle::URectangle( RECT rc )
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

//
void URectangle::Serialize(CArchive &ar)
{
	UShape::Serialize(ar);
	if (ar.IsLoading())
	{
		ar >> m_nStartX >> m_nStartY >> m_nEndX >> m_nEndY;
	}
	else
	{
		ar << m_nStartX << m_nStartY << m_nEndX << m_nEndY;
	}
}

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

void URectangle::Draw(CDC *pDC)
{
	CPen cNewPen;
	cNewPen.CreatePenIndirect(&(this->LinePen));
	CPen *pOldPen = pDC->SelectObject(&cNewPen);

	CBrush cNewBrush;
	cNewBrush.CreateBrushIndirect(&(this->FilledBrush));
	CBrush *poldBrush = pDC->SelectObject(&cNewBrush);

	pDC->Rectangle(m_nStartX, m_nStartY, m_nEndX, m_nEndY); 

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldBrush);
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
//
IMPLEMENT_SERIAL(UAgngleArc, CObject, 1)

UAgngleArc::UAgngleArc( int centerX/*=0*/, int centerY/*=0*/, DWORD radius/*=1*/, 
						FLOAT startAngle/*=0.0f*/, FLOAT sweepAngle/*=10.0f*/ )
					   : UShape() 
{
	m_nCenterX = centerX;
	m_nCenterY = centerY;
	m_dwRadius = radius;
	m_fStartAngle = startAngle;
	m_fSweepAngle = sweepAngle;
}

UAgngleArc::UAgngleArc( POINT center, DWORD radius/*=1*/, 
					   FLOAT startAngle/*=0.0f*/,  FLOAT sweepAngle/*=10.0f*/ )
					   : UShape()
{
	m_nCenterX = center.x;
	m_nCenterY = center.y;
	m_dwRadius = radius;
	m_fStartAngle = startAngle;
	m_fSweepAngle = sweepAngle;	
}

UAgngleArc::~UAgngleArc()
{}

void UAgngleArc::Serialize( CArchive &ar )
{
	UShape::Serialize(ar);
	if (ar.IsLoading())
	{
		ar >> m_nCenterX >> m_nCenterY >> m_dwRadius 
		   >> m_fStartAngle >> m_fSweepAngle;
	}
	else
	{
		ar << m_nCenterX << m_nCenterY << m_dwRadius 
		   << m_fStartAngle << m_fSweepAngle;
	}
}

void UAgngleArc::Draw( CDC *pDC )
{
	CPen cNewPen;
	cNewPen.CreatePenIndirect(&(this->LinePen));
	CPen *pOldPen = pDC->SelectObject(&cNewPen);

	CBrush cNewBrush;
	cNewBrush.CreateBrushIndirect(&(this->FilledBrush));
	CBrush *poldBrush = pDC->SelectObject(&cNewBrush);

    pDC->BeginPath();

    pDC->MoveTo(m_nCenterX, m_nCenterY);

	pDC->AngleArc(m_nCenterX, m_nCenterY, m_dwRadius,
		m_fStartAngle, m_fSweepAngle);

    pDC->LineTo(m_nCenterX, m_nCenterY);

    pDC->EndPath();

    pDC->StrokeAndFillPath();

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldBrush);
}

void UAgngleArc::setCenter( int x, int y )
{
	m_nCenterX = x;
	m_nCenterY = y;	
}

void UAgngleArc::setRadius( DWORD radius )
{
	m_dwRadius = radius;
}

void UAgngleArc::setStartAngle( FLOAT angle )
{
	m_fStartAngle = angle;
}

void UAgngleArc::setSweepAngle( FLOAT angle )
{
	m_fSweepAngle = angle;
}

//
IMPLEMENT_SERIAL(UMarker, CObject, 1)

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

void UMarker::Serialize( CArchive &ar )
{
    UShape::Serialize(ar);
    if (ar.IsLoading())
    {
        ar >> m_nLeft >> m_nTop >> m_nRight 
            >> m_nBottom;
    }
    else
    {
        ar << m_nLeft << m_nTop << m_nRight 
            << m_nBottom;
	}    
}

void UMarker::Draw( CDC *pDC )
{
    CPen cNewPen;
    cNewPen.CreatePenIndirect(&(this->LinePen));
    CPen *pOldPen = pDC->SelectObject(&cNewPen);
    
    CBrush cNewBrush;
    cNewBrush.CreateBrushIndirect(&(this->FilledBrush));
	CBrush *poldBrush = pDC->SelectObject(&cNewBrush);

    pDC->MoveTo(m_nLeft, m_nTop); 
    pDC->LineTo(m_nRight, m_nBottom); 
    pDC->MoveTo(m_nRight, m_nTop); 
    pDC->LineTo(m_nLeft, m_nBottom);  
        
    pDC->SelectObject(pOldPen);
	pDC->SelectObject(poldBrush);
}


//
IMPLEMENT_SERIAL(UPolyLine, CObject, 1)
UPolyLine::UPolyLine( POINT *pts, int num )
{
    m_pts = pts;
    m_num = num;
}

UPolyLine::~UPolyLine()
{
    
}

void UPolyLine::Serialize( CArchive &ar )
{
    UShape::Serialize(ar);
    if (ar.IsLoading())
    {
    }
    else
    {
	}      
}

void UPolyLine::Draw( CDC *pDC )
{
    CPen cNewPen;
    cNewPen.CreatePenIndirect(&(this->LinePen));
    CPen *pOldPen = pDC->SelectObject(&cNewPen);
    
    CBrush cNewBrush;
    cNewBrush.CreateBrushIndirect(&(this->FilledBrush));
	CBrush *poldBrush = pDC->SelectObject(&cNewBrush);
    pDC->Polyline(m_pts, m_num);
}

BOOL UPolyLine::setPoints( POINT *pts, int num )
{
    m_pts = pts;
    m_num = num;
    return TRUE;
}

};