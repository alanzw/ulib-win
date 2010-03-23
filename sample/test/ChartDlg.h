#if !defined(AFX_CHARTDLG_H__C3E7B314_3E86_4240_8F18_0E10A797A9F1__INCLUDED_)
#define AFX_CHARTDLG_H__C3E7B314_3E86_4240_8F18_0E10A797A9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartDlg.h : header file
//

#include "uChart.h"

/////////////////////////////////////////////////////////////////////////////
// CChartDlg dialog

class CChartDlg : public CDialog
{
// Construction
public:
	CChartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChartDlg)
	enum { IDD = IDD_CHART };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

    UChart uchart;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChartDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTDLG_H__C3E7B314_3E86_4240_8F18_0E10A797A9F1__INCLUDED_)
