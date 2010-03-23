// StateLedCtrl.cpp : implementation file
//

#include "stdafx.h" 
#include "uLedCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Productor:qigao
//date"2005-09-20
//use:can used to describe the state of switch
//note:you can use it anywhere ,but I hope not to business way

/////////////////////////////////////////////////////////////////////////////
// CStateLedCtrl

ULedCtrl::ULedCtrl()
{ 
	m_itemColor = RED; 
}

ULedCtrl::~ULedCtrl()
{
}


BEGIN_MESSAGE_MAP(ULedCtrl, CStatic)
	//{{AFX_MSG_MAP(CStateLedCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateLedCtrl message handlers

void ULedCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
	// Get the Device Context 
	
	// Get the rectangle of the window where we are going to draw
	CRect rcClient;
	GetClientRect(&rcClient);

    rcClient.DeflateRect(3,3,3,3);

	// If the pen has been selected already, then we have to delete it
	// so that it doesnt throw an assertion

	if(m_PenBright.m_hObject!=NULL)
		m_PenBright.DeleteObject();

	if(m_BrushBright.m_hObject!=NULL)
		m_BrushBright.DeleteObject();

	if(m_PenDark.m_hObject!=NULL)
		m_PenDark.DeleteObject();

	if(m_BrushDark.m_hObject!=NULL)
		m_BrushDark.DeleteObject();	

	if(m_itemColor == RED)
	{
		// I'm creating a light shade of red here for displaying the bright
		// LED. You can change the values to any colour that you want
		m_PenBright.CreatePen(0, 1, RGB(250,0,0));
		m_BrushBright.CreateSolidBrush(RGB(250,0,0));

		// Here i'm creating a dark shade of red. You can play with the values to
		// see the effect on the LED control

		m_PenDark.CreatePen(0, 1, RGB(125,0,0));
		m_BrushDark.CreateSolidBrush(RGB(0,255,0));
	}
	else if(m_itemColor == GREEN)
	{
		// If the user has selected GREEN as the color of the LED

		m_PenBright.CreatePen(0, 1, RGB(0,250,0));
		m_BrushBright.CreateSolidBrush(RGB(0,250,0));

		m_PenDark.CreatePen(0, 1, RGB(0,125,0));
		m_BrushDark.CreateSolidBrush(RGB(255,0,0));
	}
	else if(m_itemColor == YELLOW)
	{
		// If the user has selected YELLOW as the color of the LED

		m_PenBright.CreatePen(0,1,RGB(255,255 ,0));
		m_BrushBright.CreateSolidBrush(RGB(255,255,0));

		m_PenDark.CreatePen(0,1,RGB(150,150,0));
		m_BrushDark.CreateSolidBrush(RGB(150,150,0));
	}
	else
	{
		// If the user has selected BLUE as the color of the LED

		m_PenBright.CreatePen(0,1,RGB(0,0,250));
		m_BrushBright.CreateSolidBrush(RGB(0,0,250));

		m_PenDark.CreatePen(0,1,RGB(0,0,150));
		m_BrushDark.CreateSolidBrush(RGB(0,0,150));
	}
 	
	dc.SelectObject(&m_PenBright);
	dc.SelectObject(&m_BrushBright);
	
	m_BrushCurrent.m_hObject = m_BrushBright.m_hObject; 

	// If the round shape has been selected for the control 
	if(m_nShape==ID_SHAPE_ROUND)
	{
		// Draw the actual colour of the LED
		dc.Ellipse(rcClient);

		// Draw a thick dark gray coloured circle
		CPen Pen;
		Pen.CreatePen(0,2,RGB(128,128,128));
		dc.SelectObject(&Pen);
		
		dc.Ellipse(rcClient);

		// Draw a thin light gray coloured circle
		Pen.DeleteObject();
		Pen.CreatePen(0,1,RGB(192,192,192));
		dc.SelectObject(&Pen);
		dc.Ellipse(rcClient);

		// Draw a white arc at the bottom
		Pen.DeleteObject();
		Pen.CreatePen(0,1,RGB(255,255,255));
		dc.SelectObject(&Pen);

		// The arc function is just to add a 3D effect for the control
		CPoint ptStart,ptEnd;
		ptStart = CPoint(rcClient.Width()/2,rcClient.bottom);
		ptEnd	= CPoint(rcClient.right,rcClient.top);

		dc.MoveTo(ptStart);
		dc.Arc(rcClient,ptStart,ptEnd);

		CBrush Brush;
		Brush.CreateSolidBrush(RGB(255,255,255));
		dc.SelectObject(&Brush);

		// Draw the actual ellipse
		dc.Ellipse(rcClient.left+10,rcClient.top+10,rcClient.left+12,rcClient.top+12);
	}
	else if(m_nShape==ID_SHAPE_SQUARE)
	{
		// If you have decided that your LED is going to look square in shape, then

		// Draw the actual rectangle
		dc.FillRect(rcClient,&m_BrushCurrent);

		// The  code below gives a 3D look to the control. It does nothing more

		// Draw the dark gray lines
		CPen Pen;
		Pen.CreatePen(0,1,RGB(128,128,128));
		dc.SelectObject(&Pen);

		dc.MoveTo(rcClient.left,rcClient.bottom);
		dc.LineTo(rcClient.left,rcClient.top);
		dc.LineTo(rcClient.right,rcClient.top);
		
		Pen.DeleteObject();

		// Draw the light gray lines
		Pen.CreatePen(0,1,RGB(192,192,192));
		dc.SelectObject(&Pen);

		dc.MoveTo(rcClient.right,rcClient.top);
		dc.LineTo(rcClient.right,rcClient.bottom);
		dc.LineTo(rcClient.left,rcClient.bottom);	
	} 
} 

void ULedCtrl::SetLED(UINT nIDShape)
{
	// Save the color,shape and the timer interval of the control as it 
	// will be used later when resetting the LED to its previous state. 
	m_nShape = nIDShape; 
}
 
void ULedCtrl::SetSwitchState(tagItemCOLOR itemColor)
{
	m_itemColor = itemColor;
	Invalidate();
}

tagItemCOLOR ULedCtrl::GetSwitchState()
{
	return m_itemColor;
}
