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
	m_nMenuID = 0; //�˵�ID
	m_bMouseIn = FALSE; //�����־
	m_nState = 0; //��ʼ״̬
	m_bMouseClick = FALSE; //����ѡ������־
	m_bMouseClick = FALSE; //������������־
	m_strText = _T("Button"); //��ť�ı�
	m_clrForeground = huys::white; //������ɫ
    m_clrBackground = huys::tomato; //����ɫ

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
    m_rcButton = lpDrawItemStruct->rcItem; //��ȡ��ť�ߴ�
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	
	draw(pDC); //���ư�ť
	
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
		case 0: //������ť
			pDC->DrawEdge( &m_LRect, BDR_RAISEDINNER, BF_RECT );
			pDC->DrawEdge( &m_RRect, BDR_RAISEDINNER, BF_RECT );
			break;
		case 1: //������ʱ�İ�ť
			pDC->DrawEdge( &m_LRect, BDR_RAISEDINNER, BF_RECT );
			pDC->DrawEdge( &m_RRect, BDR_RAISEDINNER, BF_RECT );
			pDC->MoveTo( m_rcButton.TopLeft() );
			pDC->LineTo( m_rcButton.right, m_rcButton.top );
			break;
		case 2: //������ť������ʱ�İ�ť
			pDC->DrawEdge( &m_RRect, BDR_RAISEDINNER, BF_RECT );
			break;
		case 3: //������ťѡ����ʱ�İ�ť
			pDC->DrawEdge( &m_LRect, BDR_RAISEDINNER, BF_RECT );
			break;
	}
	
	POINT ptVertex[3], ptCentre; //��ͷ����(��������)
	ptCentre = m_RRect.CenterPoint(); //ѡ�����е�λ��
	ptVertex[0].x = ptCentre.x-3; //�����ͷ����
	ptVertex[0].y = ptCentre.y-2;
	ptVertex[1].x = ptCentre.x+4;
	ptVertex[1].y = ptCentre.y-2;
	ptVertex[2].x = ptCentre.x;
	ptVertex[2].y = ptCentre.y+2;
	
	pDC->SelectStockObject( BLACK_BRUSH ); //���廭ˢ����ɫ��
	CRgn rgn;
	rgn.CreatePolygonRgn( ptVertex, 3, ALTERNATE );
	pDC->PaintRgn( &rgn ); //��ѡ������ͷ
	
	pDC->SetTextColor( m_clrForeground ); //������������
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( 
		m_strText, 
		&m_LRect, 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS); 
}

void CMenuButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( !m_bMouseIn || GetCapture()!=this ) //�����밴ť
	{
		m_bMouseIn = TRUE; //���ý����־
		SetCapture(); //�������
		m_nState = 1; //�ð�ť״̬(1-��ǰ��ť)
		if( m_bMouseClick ) //��ⵥ��ѡ������־
		{
// 			m_menu.Detach(); //����򿪵Ĳ˵�
// 			m_menu.DestroyMenu();
			m_bMouseClick = FALSE;
		}
		Invalidate(); //�ػ水ť
	}
	else
	{
		if ( !m_rcButton.PtInRect(point) ) //����뿪��ť
		{
			m_bMouseIn = FALSE; //��������־
			ReleaseCapture(); //�ͷ���겶��
			m_bMouseClick = FALSE; //���������־
			m_nState = 0; //�ð�ť״̬(0-������ť)
			if( m_bMouseClick ) //��ⵥ��ѡ������־
			{
// 				m_menu.Detach(); //����򿪵Ĳ˵�
// 				m_menu.DestroyMenu();
				m_mtt.show();
				m_bMouseClick = FALSE;
			}
			Invalidate(); //�ػ水ť
		}
	}


	CButton::OnMouseMove(nFlags, point);
}

void CMenuButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_LRect.PtInRect(point) ) //������ť������
	{
		m_nState = 2; //�ð�ť״̬(2-������ť)
		m_bMouseClick = TRUE; //���õ�����ť��־
		Invalidate(); //�ػ水ť
		//m_mtt.show();
		
	}
	else if( m_RRect.PtInRect(point)) //����ѡ����
	{
		m_nState = 3;
		m_bMouseClick = FALSE; //���������ť��־
		Invalidate(); //�ػ水ť
		m_bMouseClick = !m_bMouseClick; //����ѡ������־
// 		if( m_bMouseClick ) //һ�ε����������˵�
// 		{
			CRect rect = m_RRect;
			ClientToScreen(rect); //ת��Ϊ��Ļ����
			point = rect.BottomRight();
			point.x -= rect.Width(); //���õ����˵���λ��

			if (!m_bTooltipCreated)
			{
				m_mtt.create(this->GetSafeHwnd());
                m_bTooltipCreated = TRUE;
			}			
			m_mtt.setXY(point.x, point.y);
			m_mtt.show();

// 			if (m_menu.m_hMenu == NULL)
// 			{
// 				VERIFY(m_menu.LoadMenu(m_nMenuID)); //װ��˵���Դ
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
// 			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, pWndPopupOwner); //�����˵�
// 		}
// 		else //�ٴε���������˵�
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
	m_nState = 0; //�ָ�Ϊ������ť
	Invalidate(); //�ػ水ť	

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
