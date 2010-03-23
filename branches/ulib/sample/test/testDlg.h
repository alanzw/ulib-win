// testDlg.h : header file
//

#if !defined(AFX_TESTDLG_H__5BBF1CEA_E3C8_4EE1_AB9D_BF296C363641__INCLUDED_)
#define AFX_TESTDLG_H__5BBF1CEA_E3C8_4EE1_AB9D_BF296C363641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uListCtrl.h"
#include "uButton.h"
#include "uToolbar.h"
#include "uMenu.h"
/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DIALOG };
	CProgressCtrl	m_progress;
	CString	m_CBTest;
	//}}AFX_DATA
	UMixedListCtrl	m_lstTest;
	CMenuButton	m_bnTest;
	bool bBnCreated;

	UCoolMenu m_menu;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	bool bExtHiden;

// Implementation
protected:
	HICON m_hIcon;
    UToolBar m_MainToolBar;
    int nVal;
	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnHide();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButton2();
	//}}AFX_MSG
	afx_msg LRESULT OnThreadFinished(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnThreadUpdateProgress(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__5BBF1CEA_E3C8_4EE1_AB9D_BF296C363641__INCLUDED_)
