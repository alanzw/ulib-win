#if !defined(AFX_UBUTTON_H__7D3926B3_5243_4B41_9ED2_E0630B908369__INCLUDED_)
#define AFX_UBUTTON_H__7D3926B3_5243_4B41_9ED2_E0630B908369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// uButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuButton window

#include "colors.h"
#include "uTooltip.h"

class CMenuButton : public CButton
{
// Construction
public:
	CMenuButton();


// Attributes
public:
	CRect m_rcButton;
	CRect m_LRect;
	CRect m_RRect;

	huys::Color m_clrBackground;
	huys::Color m_clrForeground;

	CString m_strText;
	CMenu m_menu;
	int m_nMenuID;

	int m_nState;
	BOOL m_bMouseIn;
	BOOL m_bMouseClick;


	CMyTooltip m_mtt;
	BOOL m_bTooltipCreated;

// Operations
public:
	void draw(CDC *pDC);
	void attachMenu(UINT uMenuID);
	void setText(CString text);
	void setText(LPCSTR lpszText);
	void setForeColor(huys::Color color);
	void setBackColor(huys::Color color);
	//
	void setTooltipText(const TCHAR *lpszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //
    protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
    afx_msg BOOL OnTooltipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	LPSTR m_lpszTooltip;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UBUTTON_H__7D3926B3_5243_4B41_9ED2_E0630B908369__INCLUDED_)
