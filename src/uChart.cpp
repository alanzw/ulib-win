#include "stdafx.h"

#include "uChart.h"

#include "ugdi.h"
#include "colors.h"

IMPLEMENT_DYNAMIC(UChart, CStatic)

UChart::UChart()
{
    
}

BEGIN_MESSAGE_MAP(UChart, CStatic)
ON_WM_PAINT()
END_MESSAGE_MAP()

void UChart::OnPaint()
{
    CPaintDC dc(this); 

    RECT rc;
    this->GetClientRect(&rc);
	//dc.SetMapMode(MM_LOENGLISH);
	//dc.SetViewportOrg(0, rc.bottom-rc.top);

	this->setRange(0, 0, rc.right-rc.left, rc.bottom-rc.top);

    huys::URectangle urect(rc);
    urect.setFilledColor(huys::blueviolet);
    urect.setFilledStyle(BS_SOLID);
    urect.Draw(&dc);
    
    //uline.setLineColor(huys::green);
    //uline.setLneWidth(2);
    //uline.Draw(&dc);
	RECT rcUpper = {rc.left+20, rc.top, rc.right, rc.top+20};
	this->MapRect(&rcUpper);
    huys::URectangle upper(rcUpper);
	
	upper.setFilledColor(huys::gray);
    upper.setFilledStyle(BS_SOLID);
    upper.Draw(&dc);

	RECT rcLeft = {rc.left, rc.top+20, rc.left+20, rc.bottom};
	this->MapRect(&rcLeft);
    huys::URectangle uleft(rcLeft);
    uleft.setFilledColor(huys::gray);
    uleft.setFilledStyle(BS_SOLID);
    uleft.Draw(&dc);

    CString str = _T("10");
    CRect rcText(rc.left+40, rc.bottom-18, rc.left+70, rc.bottom);
//	this->MapRect((RECT *)&rcText);
	dc.SetTextColor(huys::white);
    dc.SetBkColor(huys::gray);
    dc.DrawText(str, &rcText, DT_LEFT);

    for (int ii=0; ii<rc.right-rc.left-120; ii+=40)
    {
        rcText.OffsetRect(40, 0);
        dc.DrawText(str, &rcText, DT_LEFT);
    }

    rcText.SetRect(rc.left+2, rc.top+40, rc.left+20, rc.top+70);
    dc.DrawText(str, &rcText, DT_LEFT);
    for (int kkk=0; kkk<rc.bottom-rc.top-120; kkk+=40)
    {
        rcText.OffsetRect(0, 40);
       dc.DrawText(str, &rcText, DT_LEFT);
    }

    huys::ULine uline(rc.left+40, rc.top+20, rc.left+40, rc.bottom);
    uline.setLineColor(huys::gray);
    //uline.setFilledColor(huys::blueviolet);
    uline.setLineStyle(PS_DOT);
    // Filling in the Gaps.
    dc.SetBkMode(TRANSPARENT);

    uline.Draw(&dc);
    for (int i=0; i<rc.right-rc.left-80; i+=20)
    {
        uline.offsetX(20);
        uline.Draw(&dc);
    }

    uline.SetStartPnt(rc.left+20, rc.top+40);
    uline.SetEndPnt(rc.right, rc.top+40);
    uline.Draw(&dc);
    for (int j=0; j<rc.bottom-rc.top-80; j+=20)
    {
        uline.offsetY(20);
        uline.Draw(&dc);
    }

    //
    POINT pts[] = {
        {0, 40},
        {10, 50},
        {60, 70},
        {80, 90}
    };

    for (int k=0; k<4; k++)
    {
        pts[k].x = pts[k].x * (rc.right-rc.left-40) / 100;
        pts[k].y = pts[k].y * (rc.bottom-rc.top-40) / 100;
    }

    huys::UPolyLine upl(pts, 4);
    upl.setLineColor(huys::red);
    upl.Draw(&dc);

    huys::URectangle markercc(0, 0, 10, 10);
    markercc.setFilledColor(huys::red);
    markercc.setFilledStyle(BS_SOLID);

    for (int kk=0; kk<4; kk++)
    {
        markercc.moveCenterTo(pts[kk].x, pts[kk].y);
        markercc.Draw(&dc);
    }
}

BOOL UChart::attachPoints( POINT *pts, int num )
{
	return TRUE;	
}

void UChart::setRange( int x1, int y1, int x2, int y2 )
{
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
}

// Real coordinate to MM_TEXT
void UChart::MapPoint( POINT *pt )
{
    RECT rc;
    this->GetClientRect(&rc);
	
	int cx = rc.right - rc.left;
	int cy = rc.bottom - rc.top;

	pt->x = pt->x * m_x2/cx;
	pt->y = cy - pt->y * m_y2/cy;	
}

void UChart::MapRect( RECT *rect )
{
    RECT rc;
    this->GetClientRect(&rc);
	
	int cx = rc.right - rc.left;
	int cy = rc.bottom - rc.top;
	
	rect->left = rect->left * m_x2/cx;
	rect->top = cy - rect->top * m_y2/cy;	
	rect->right = rect->right *  m_x2/cx;
	rect->bottom =  cy - rect->bottom * m_y2/cy;

}