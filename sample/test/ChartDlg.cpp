// ChartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "ChartDlg.h"
#include "ugdi.h"
#include "colors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartDlg dialog


CChartDlg::CChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_STATIC_CHART, uchart);
}


BEGIN_MESSAGE_MAP(CChartDlg, CDialog)
	//{{AFX_MSG_MAP(CChartDlg)
	ON_BN_CLICKED(IDC_BN_GO, OnBnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartDlg message handlers

BOOL CChartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChartDlg::OnBnGo() 
{
	// TODO: Add your control notification handler code here
	CDC *pDC = GetDC();
	RECT rc;

//	pDC->SetMapMode(MM_LOENGLISH);
//	pDC->SetViewportOrg(0,0);
	SetGraphicsMode ( pDC->GetSafeHdc(), GM_ADVANCED );
	GetClientRect(&rc);

	int dx = (rc.right-rc.left)/5;
	int dy = (rc.bottom-rc.top)/5;
	rc.left += 20;
	rc.top += 20;
	rc.right = rc.left + dx;
	rc.bottom = rc.top + dy;

    pDC->DPtoLP ( (LPPOINT) &rc, 2 );

	huys::URectangle rect(rc);
	rect.setFilledColor(huys::red);
	rect.setFilledStyle(BS_SOLID);
	rect.Draw(pDC);

	XFORM xfm = {1.0f, 0.0f, 0.0f, 1.0f, (FLOAT)dx, (FLOAT)dy};
	SetWorldTransform(pDC->GetSafeHdc(), &xfm);	
	rect.setFilledColor(huys::green);
	rect.Draw(pDC);

	xfm.eDx = - (FLOAT)0.0;
	SetWorldTransform(pDC->GetSafeHdc(), &xfm);	
	rect.setFilledColor(huys::blue);
	rect.Draw(pDC);

	xfm.eDx = (FLOAT)dx;
	xfm.eDy = (FLOAT)0.0;
	SetWorldTransform(pDC->GetSafeHdc(), &xfm);	
	rect.setFilledColor(huys::white);
	rect.Draw(pDC);

}
