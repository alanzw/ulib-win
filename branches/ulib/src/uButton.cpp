// uButton.cpp : implementation file
//

#include "stdafx.h"
#include "uButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuButton

CMenuButton::CMenuButton()
{
	m_nMenuID = 0; //菜单ID
	m_bMouseIn = FALSE; //进入标志
	m_nState = 0; //初始状态
	m_bMouseClick = FALSE; //单击选择区标志
	m_bMouseClick = FALSE; //单击主体区标志
	m_strText = _T("Button"); //按钮文本
	m_clrForeground = huys::white; //文字颜色
    m_clrBackground = huys::tomato; //背景色

	EnableToolTips(TRUE);

	m_bTooltipCreated = FALSE;

	m_lpszTooltip = NULL;
	
}

CMenuButton::~CMenuButton()
{
	if (m_lpszTooltip)
	{
		delete[] m_lpszTooltip;
	}
	m_lpszTooltip = NULL;
}


BEGIN_MESSAGE_MAP(CMenuButton, CButton)
	//{{AFX_MSG_MAP(CMenuButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnTooltipNotify)
//    ON_WM_CLOSE()
    ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuButton message handlers

void CMenuButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	ModifyStyle( 0, BS_OWNERDRAW ); //
	
	CButton::PreSubclassWindow();
}

void CMenuButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
    m_rcButton = lpDrawItemStruct->rcItem; //获取按钮尺寸
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	
	draw(pDC); //绘制按钮
	
    pDC->RestoreDC( nSavedDC );
}

void CMenuButton::draw( CDC *pDC )
{
	// 
	m_LRect.SetRect( 
		m_rcButton.left, 
		m_rcButton.top,
		m_rcButton.right-21, 
		m_rcButton.bottom ); 
	
	//
	m_RRect.SetRect( 
		m_rcButton.right-20, 
		m_rcButton.top,
		m_rcButton.right, 
		m_rcButton.bottom ); 
	
	CPen newPen;
	newPen.CreatePen(PS_SOLID, 1, huys::skyblue );
	pDC->SelectObject( &newPen );
	
	pDC->FillSolidRect( m_rcButton, m_clrBackground ); 
	switch( m_nState ) //
	{
		case 0: //正常按钮
			pDC->DrawEdge( &m_LRect, BDR_RAISEDINNER, BF_RECT );
			pDC->DrawEdge( &m_RRect, BDR_RAISEDINNER, BF_RECT );
			break;
		case 1: //鼠标进入时的按钮
			pDC->DrawEdge( &m_LRect, BDR_RAISEDINNER, BF_RECT );
			pDC->DrawEdge( &m_RRect, BDR_RAISEDINNER, BF_RECT );
			pDC->MoveTo( m_rcButton.TopLeft() );
			pDC->LineTo( m_rcButton.right, m_rcButton.top );
			break;
		case 2: //单击按钮主体区时的按钮
			pDC->DrawEdge( &m_RRect, BDR_RAISEDINNER, BF_RECT );
			break;
		case 3: //单击按钮选择区时的按钮
			pDC->DrawEdge( &m_LRect, BDR_RAISEDINNER, BF_RECT );
			break;
	}
	
	POINT ptVertex[3], ptCentre; //箭头坐标(三个顶点)
	ptCentre = m_RRect.CenterPoint(); //选择区中点位置
	ptVertex[0].x = ptCentre.x-3; //计算箭头坐标
	ptVertex[0].y = ptCentre.y-2;
	ptVertex[1].x = ptCentre.x+4;
	ptVertex[1].y = ptCentre.y-2;
	ptVertex[2].x = ptCentre.x;
	ptVertex[2].y = ptCentre.y+2;
	
	pDC->SelectStockObject( BLACK_BRUSH ); //定义画刷（黑色）
	CRgn rgn;
	rgn.CreatePolygonRgn( ptVertex, 3, ALTERNATE );
	pDC->PaintRgn( &rgn ); //画选择区箭头
	
	pDC->SetTextColor( m_clrForeground ); //画主体区文字
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( 
		m_strText, 
		&m_LRect, 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS); 
}

void CMenuButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( !m_bMouseIn || GetCapture()!=this ) //鼠标进入按钮
	{
		m_bMouseIn = TRUE; //设置进入标志
		SetCapture(); //捕获鼠标
		m_nState = 1; //置按钮状态(1-当前按钮)
		if( m_bMouseClick ) //检测单击选择区标志
		{
// 			m_menu.Detach(); //清除打开的菜单
// 			m_menu.DestroyMenu();
			m_bMouseClick = FALSE;
		}
		Invalidate(); //重绘按钮
	}
	else
	{
		if ( !m_rcButton.PtInRect(point) ) //鼠标离开按钮
		{
			m_bMouseIn = FALSE; //清除进入标志
			ReleaseCapture(); //释放鼠标捕获
			m_bMouseClick = FALSE; //清除单击标志
			m_nState = 0; //置按钮状态(0-正常按钮)
			if( m_bMouseClick ) //检测单击选择区标志
			{
// 				m_menu.Detach(); //清除打开的菜单
// 				m_menu.DestroyMenu();
				m_mtt.show();
				m_bMouseClick = FALSE;
			}
			Invalidate(); //重绘按钮
		}
	}


	CButton::OnMouseMove(nFlags, point);
}

void CMenuButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_LRect.PtInRect(point) ) //单击按钮主体区
	{
		m_nState = 2; //置按钮状态(2-正常按钮)
		m_bMouseClick = TRUE; //设置单击按钮标志
		Invalidate(); //重绘按钮
		//m_mtt.show();
		
	}
	else if( m_RRect.PtInRect(point)) //单击选择区
	{
		m_nState = 3;
		m_bMouseClick = FALSE; //清除单击按钮标志
		Invalidate(); //重绘按钮
		m_bMouseClick = !m_bMouseClick; //单击选择区标志
// 		if( m_bMouseClick ) //一次单击，弹出菜单
// 		{
			CRect rect = m_RRect;
			ClientToScreen(rect); //转换为屏幕坐标
			point = rect.BottomRight();
			point.x -= rect.Width(); //设置弹出菜单的位置

			if (!m_bTooltipCreated)
			{
				m_mtt.create(this->GetSafeHwnd());
                m_bTooltipCreated = TRUE;
			}			
			m_mtt.setXY(point.x, point.y);
			m_mtt.show();

// 			if (m_menu.m_hMenu == NULL)
// 			{
// 				VERIFY(m_menu.LoadMenu(m_nMenuID)); //装入菜单资源
// 			}		
// 
// 			
// 			CMenu* pPopup = m_menu.GetSubMenu(0);
// 			ASSERT(pPopup != NULL);
// 			CWnd* pWndPopupOwner = this;
// 			
// 			while (pWndPopupOwner->GetStyle() & WS_CHILD)
// 				pWndPopupOwner = pWndPopupOwner->GetParent();
// 			
// 			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, pWndPopupOwner); //弹出菜单
// 		}
// 		else //再次单击，清除菜单
// 		{
// 			m_menu.Detach();
// 			m_menu.DestroyMenu();
// 		}
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CMenuButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nState = 0; //恢复为正常按钮
	Invalidate(); //重绘按钮	

	CButton::OnLButtonUp(nFlags, point);
}

void CMenuButton::attachMenu( UINT uMenuID )
{
	this->m_nMenuID = uMenuID;	
}

void CMenuButton::setText( CString text )
{
	this->m_strText = text;	
}

void CMenuButton::setText( LPCSTR lpszText )
{
	this->m_strText = lpszText;	
}

void CMenuButton::setForeColor( huys::Color color )
{
	this->m_clrForeground = color;	
}

void CMenuButton::setBackColor( huys::Color color )
{
	this->m_clrBackground = color;	
}

BOOL CMenuButton::PreTranslateMessage( MSG* pMsg )
{
    return CButton::PreTranslateMessage(pMsg);
}

BOOL CMenuButton::OnTooltipNotify( UINT id, NMHDR* pNMHDR, LRESULT* pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR; 
    UINT uID = pNMHDR->idFrom;

	if (m_lpszTooltip)
	{
		delete[] m_lpszTooltip;
		m_lpszTooltip = NULL;
	}
	
    int size = m_mtt.m_strText.GetLength()+10;
	m_lpszTooltip = new char[size];
#if _MFC_VER <= 0x0600
    strcpy(m_lpszTooltip, (LPCTSTR)m_mtt.m_strText );
#else
    strcpy_s(m_lpszTooltip, size, (LPCTSTR)m_mtt.m_strText);
#endif


    pTTT->lpszText=this->m_lpszTooltip;

    return TRUE; 

}

void CMenuButton::OnNcDestroy()
{
    CButton::OnNcDestroy();
    if (m_mtt.isVisible())
    {
        m_mtt.show();
    }
    // TODO: Add your message handler code here
}

void CMenuButton::setTooltipText( const TCHAR *lpszText )
{
	m_mtt.setText(lpszText);
}
