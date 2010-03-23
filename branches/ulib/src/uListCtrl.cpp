#include "StdAfx.h"

#include "uListCtrl.h"
//#include "mymacro.h"

UCellEdit::UCellEdit()
{
	
}

UCellEdit::~UCellEdit()
{
	
}

BEGIN_MESSAGE_MAP(UCellEdit, CEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYUP()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL UCellEdit::PreTranslateMessage( MSG* pMsg )
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || 
            pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_TAB || 
            pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || GetKeyState (VK_CONTROL))
        {
            ::TranslateMessage (pMsg);
            ::DispatchMessage (pMsg);
            return TRUE;		    	// DO NOT process further
        }
    }
    
    return CEdit::PreTranslateMessage (pMsg);    
}

void UCellEdit::OnKillFocus(CWnd* pNewWnd)
{  
    CEdit::OnKillFocus(pNewWnd);
    // TODO: Add your message handler code here
    //AfxMessageBox("kill focus!");
    CString strText;
    this->GetWindowText(strText);
    UMixedListCtrl *pParent = (UMixedListCtrl *)this->GetParent();
    pParent->updateItemText(strText);

   //DestroyWindow();
}

void UCellEdit::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    switch (nChar)
    {
    case VK_NEXT :
        {

            return;
        }
    case VK_PRIOR :
        {

            return;
        }
    }
    
    CEdit::OnKeyUp (nChar, nRepCnt, nFlags);    
}

void UCellEdit::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    BOOL Control = GetKeyState (VK_CONTROL) < 0;
    switch (nChar)
    {
    case VK_UP :
        return;
    case VK_DOWN :
        return;
    case VK_HOME :
        return;
    case VK_END :
        return;
    }
    
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);    
}

void UCellEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    BOOL Shift = GetKeyState (VK_SHIFT) < 0;
    switch (nChar)
    {
    case VK_ESCAPE :
        GetParent()->SetFocus();
        return;
    case VK_RETURN:
        OnKillFocus(NULL);
        ((UMixedListCtrl *)this->GetParent())->endCellEdit();
        return;
    case VK_TAB:
        return;
    }
    
    CEdit::OnChar (nChar, nRepCnt, nFlags);    
}

UCellCombo::UCellCombo()
{
	
}

UCellCombo::~UCellCombo()
{
	
}

BEGIN_MESSAGE_MAP(UCellCombo, CComboBox)
    ON_WM_KILLFOCUS()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()

void UCellCombo::OnKillFocus(CWnd* pNewWnd)
{  
    //CEdit::OnKillFocus(pNewWnd);
    // TODO: Add your message handler code here
    //AfxMessageBox("kill focus!");
    CString strText;
    this->GetLBText(this->GetCurSel(), strText);
    UMixedListCtrl *pParent = (UMixedListCtrl *)this->GetParent();
    pParent->updateItemText(strText);
}

UMixedListCtrl::UMixedListCtrl()
{
	m_pCellEdit = NULL;
	m_pCellCombo = NULL;
	m_row = 0;
	m_col = -1;
    lstEditCols.RemoveAll();
    lstCmboCols.RemoveAll();
}

UMixedListCtrl::~UMixedListCtrl()
{
// 	if (NULL != m_pCellEdit)
// 	{
// 		delete m_pCellEdit;
// 	}
// 	if (NULL != m_pCellCombo)
// 	{
// 		delete m_pCellCombo;
// 	}
// 	m_pCellEdit = NULL;
// 	m_pCellCombo = NULL;	
}

BEGIN_MESSAGE_MAP(UMixedListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
    ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
    ON_WM_KILLFOCUS()
    ON_WM_ACTIVATE()
END_MESSAGE_MAP()

BOOL UMixedListCtrl::PreTranslateMessage( MSG *pMsg )
{
    
    
    return CListCtrl::PreTranslateMessage(pMsg);	
}

LRESULT UMixedListCtrl::beginCellEdit()
{
	
	if(!m_pCellEdit) //If Edit box does not exist create one
	{
		m_pCellEdit = new UCellEdit;
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL; 
		CRect rect;
		GetSubItemRect(m_row,m_col,LVIR_LABEL,rect);
		m_pCellEdit->Create(dwStyle,rect,this,1001);
		m_pCellEdit->ShowWindow(SW_SHOW);
		m_pCellEdit->SetWindowText(m_strItemText);
		m_pCellEdit->SetSel(0,-1);
		m_pCellEdit->SetFocus();
		if(m_pCellCombo) m_pCellCombo->ShowWindow(SW_HIDE);
	}
	else  //If Edit box exist just move it
	{
		CRect rect;
		GetSubItemRect(m_row,m_col,LVIR_LABEL,rect);
		//m_pedit->SetWindowPos(NULL,rect.left,rect.top,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
		m_pCellEdit->SetWindowText(m_strItemText);
		m_pCellEdit->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
		//m_pedit->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height(),1);
		m_pCellEdit->SetSel(0,-1);
		m_pCellEdit->SetFocus();
		if(m_pCellCombo) m_pCellCombo->ShowWindow(SW_HIDE);
	}

	return 	0;
}

LRESULT UMixedListCtrl::endCellEdit()
{
 	if (NULL != m_pCellEdit)
 	{
 		m_pCellEdit->ShowWindow(SW_HIDE);
		m_pCellEdit->DestroyWindow();
		delete m_pCellEdit;
 	}
 	m_pCellEdit = NULL;
	return 	0;	
}

LRESULT UMixedListCtrl::beginCellCombo()
{
	return 	0;	
}

LRESULT UMixedListCtrl::endCellCombo()
{
	return 	0;	
}

BOOL UMixedListCtrl::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT *pResult )
{
	switch (((NMHDR*)lParam)->code)
    {
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
		{
			*pResult = 0;//TRUE;                // disable tracking 
			if(m_pCellEdit && m_pCellEdit->IsWindowVisible() )
			{
				m_pCellEdit->ShowWindow(SW_HIDE); 		
			}
			
			if(m_pCellCombo && m_pCellCombo->IsWindowVisible())
			{	
				m_pCellCombo->ShowWindow(SW_HIDE);		
			}
			//return TRUE;                    // Processed message
		}
		
    }   
	
	
	return CListCtrl::OnNotify(wParam, lParam, pResult);	
}

void UMixedListCtrl::OnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	//
	//AfxMessageBox(_T("Got Clicking!"));
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem == -1 || pNMListView->iSubItem == -1)
	{
		endCellEdit();

		*pResult =0;
		return;
	}

	m_row = pNMListView->iItem;
    m_col = pNMListView->iSubItem;

    //GetParent()->SendMessage(m_clickMsg, 0, (LPARAM)m_row);

	m_strItemText = this->GetItemText(m_row, m_col);
    if (NULL != lstEditCols.Find(m_col))
    {
    	beginCellEdit();
    }
    else
    {
        endCellEdit();
    }


	*pResult = 0;
}

void UMixedListCtrl::OnColumnclick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//AfxMessageBox("Clicked On header");
	if(m_pCellEdit && m_pCellEdit->IsWindowVisible() )
	{
		m_pCellEdit->ShowWindow(SW_HIDE); 
		endCellEdit();
	}
	
	if(m_pCellCombo && m_pCellCombo->IsWindowVisible())
	{	
		m_pCellCombo->ShowWindow(SW_HIDE);		
	}
	*pResult = 0;	
}

BOOL UMixedListCtrl::updateItemText( const CString strText )
{
    if (NULL != lstEditCols.Find(m_col))
    {
        this->SetItemText(m_row, m_col, (LPCTSTR)strText);
    }

    return TRUE;
}

void UMixedListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
	if(m_pCellEdit && m_pCellEdit->IsWindowVisible() )
	{
		m_pCellEdit->ShowWindow(SW_HIDE); 
		endCellEdit();
	}

	if(m_pCellCombo && m_pCellCombo->IsWindowVisible())
	{	
		m_pCellCombo->ShowWindow(SW_HIDE);		
	}
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL UMixedListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(m_pCellEdit && m_pCellEdit->IsWindowVisible() )
	{
		m_pCellEdit->ShowWindow(SW_HIDE); 
		endCellEdit();
	}

	if(m_pCellCombo && m_pCellCombo->IsWindowVisible())
	{	
		m_pCellCombo->ShowWindow(SW_HIDE);		
	}
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void UMixedListCtrl::setEditable( int col )
{
    if (NULL == lstEditCols.Find(col))
    {
        lstEditCols.AddTail(col);
    }        
}

void UMixedListCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CListCtrl::OnActivate(nState, pWndOther, bMinimized);

    // TODO: Add your message handler code here
    if (WA_INACTIVE == nState)
    {
        endCellEdit();
    }
}

LRESULT UMixedListCtrl::changeHeight( int height )
{
    this->ModifyStyle(0,LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL);
    this->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES
        |LVS_EX_HEADERDRAGDROP);
    
    CImageList   m_l;
    m_l.Create(1, height, TRUE|ILC_COLOR32, 1, 0);
    this->SetImageList(&m_l,LVSIL_SMALL);
   
    return TRUE;
}