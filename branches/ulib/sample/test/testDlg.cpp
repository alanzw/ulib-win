// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "ugdi.h"
#include "uLink.h"
#include "ChartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct THREADINFOSTRUCT {
	HWND hWnd;
	CString someData;	
} THREADINFOSTRUCT;


UINT MyThreadFunc(LPVOID lParam)
{
	THREADINFOSTRUCT *tis=(THREADINFOSTRUCT *)lParam;
	for (int i=0;i<100;i++) {
		PostMessage(tis->hWnd, WM_USER_THREAD_UPDATE_PROGRESS, i, 100);
		Sleep(100);
	}
	PostMessage(tis->hWnd,WM_USER_THREAD_FINISHED,0,0);
	delete tis;
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CMenuButton	m_ddddd;
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();

    huys::ULine line;
	huys::URectangle rect;
    huys::UAgngleArc arc;
    huys::UMarker marker;

	ULink m_linkHomePage;
};

CAboutDlg::CAboutDlg() 
: CDialog(CAboutDlg::IDD),
line(0,0,100,100),
rect(0,0,100,100),
arc(200, 200, 40, 0.0, 90.0),
marker(250, 200, 350, 300)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_linkHomePage.m_link = _T("http://www.huys.com");
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON1, m_ddddd);
	DDX_Control(pDX, IDC_LINK_HOMEPAGE, m_linkHomePage);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
    ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_CBTest = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	
	bBnCreated = false;
	bExtHiden  = false;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_CBString(pDX, IDC_COMBO1, m_CBTest);
	DDX_Control(pDX, IDC_LIST_TEST, m_lstTest);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BN_HIDE, OnBnHide)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_THREAD_FINISHED, OnThreadFinished)
	ON_MESSAGE(WM_USER_THREAD_UPDATE_PROGRESS, OnThreadUpdateProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{

	
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Add the toolbar.
//     if (!m_MainToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP
//         | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
//         ! m_MainToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
//     {
//         TRACE0("Failed to create toolbar\n");
//         return -1;
//     }

//    m_MainToolBar.EnableDocking(CBRS_ALIGN_ANY);
    //DockControlBar(&m_MainToolBar);

    m_MainToolBar.Create(this);
    m_MainToolBar.LoadToolBar(IDR_TOOLBAR_MAIN);


    //得出控件条大小.
    CRect rect;
    CRect rectNow;
    GetClientRect(rect);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0,reposQuery,rectNow);
    
    //放置控件条位置
    CPoint ptOffset(rectNow.left-rect.left,rectNow.top-rect.top);
    
    CRect rcChild;
    CWnd* pwndChild=GetWindow(GW_CHILD);
    while (pwndChild)
    {                              
        pwndChild->GetWindowRect(rcChild);
        ScreenToClient(rcChild);
        rcChild.OffsetRect(ptOffset);
        pwndChild->MoveWindow(rcChild,FALSE);
        pwndChild=pwndChild->GetNextWindow();
    }
    
    //调整对话框尺寸
    CRect rcWindow;
    GetWindowRect(rcWindow);
    rcWindow.right+=rect.Width()-rectNow.Width();
    rcWindow.bottom+=rect.Height()-rectNow.Height();
    MoveWindow(rcWindow, FALSE);
    
    //控件条定位
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);


    int index = 0;
    RECT rc_s;
    while(m_MainToolBar.GetItemID(index)!=ID_TB_LED)
        index++;
    
    //
    //m_MainToolBar.SetButtonInfo(index, ID_TB_LED, TBBS_SEPARATOR, 20);
    m_MainToolBar.GetItemRect(index, &rc_s);
    
    //
    //rc_s.top +=2;
    //rc_s.bottom += 200;
    m_MainToolBar.m_subCtrl.Create("", WS_CHILD|WS_VISIBLE,
        rc_s, &m_MainToolBar, ID_TB_LED);
    m_MainToolBar.m_subCtrl.SetLED(ID_SHAPE_ROUND);
    m_MainToolBar.m_subCtrl.ShowWindow(SW_SHOW);

	
	// TODO: Add extra initialization here
    //
    m_lstTest.changeHeight(24);
    m_lstTest.InsertColumn(0, _T("NO"));
    m_lstTest.InsertColumn(1, _T("Data"));
    m_lstTest.setEditable(1);
    
    CRect rc;
    m_lstTest.GetClientRect(&rc);
    m_lstTest.SetColumnWidth(0, rc.Width()/2);
    m_lstTest.SetColumnWidth(1, rc.Width()/2);
    
    m_lstTest.InsertItem(0, "1");
    m_lstTest.SetItemText(0, 1, "333");
    m_lstTest.InsertItem(1, "2");
    m_lstTest.SetItemText(1, 1, "333");

   	//EnableToolTips(TRUE);

    nVal = 0;
	SetTimer(1, 500, NULL);

	m_menu.AttachMenu(GetMenu()->GetSafeHmenu(),IDR_TOOLBAR_MAIN,CSize(32,30));
       
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
    if (!bBnCreated)
    {
		m_bnTest.Create(_T("ee"), BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE, CRect(100, 290, 300, 400), this, 7766);
 		m_bnTest.setText(_T("其他燃料特性"));
 		m_bnTest.m_mtt.setTitle(_T("燃料特性"));
 		m_bnTest.setTooltipText(_T("1.当量比：\r\n2.着火点：\r\n3.======== \
			\r\n4====================\r\n5====================\r\n6====================\r\n7===================="));
        m_bnTest.m_mtt.setBKColor(huys::orange);
        m_bnTest.m_mtt.setTextColor(huys::blue);
        bBnCreated = true;
    }

	THREADINFOSTRUCT *tis=new THREADINFOSTRUCT;

	tis->hWnd=m_hWnd;
	tis->someData="This is in a thread.";

	CWinThread *pThread = AfxBeginThread(MyThreadFunc,tis,THREAD_PRIORITY_NORMAL,0,0);
	//delete tis;

	
}


void CAboutDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages
	rect.setFilledColor(huys::red);
	rect.setFilledStyle(BS_HATCHED);
	rect.Draw(&dc);

    line.Draw(&dc);

    arc.setFilledColor(huys::green);
    arc.setFilledStyle(BS_SOLID);
    arc.Draw(&dc);

    marker.setLineColor(huys::red);
    marker.setLneWidth(3);
    marker.Draw(&dc);
}

void CTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if(nVal == 0)
    {
        m_MainToolBar.m_subCtrl.SetSwitchState(RED);
    }
    else if(nVal == 1)
    {
        m_MainToolBar.m_subCtrl.SetSwitchState(GREEN);
    }
    else if(nVal == 2)
    {
        m_MainToolBar.m_subCtrl.SetSwitchState(BLUE);
    }
    else
    {
        m_MainToolBar.m_subCtrl.SetSwitchState(YELLOW);
    }
    
    nVal ++;
    if(nVal > 3)
		nVal = 0;	
	CDialog::OnTimer(nIDEvent);
}

void CTestDlg::OnBnHide() 
{
	// TODO: Add your control notification handler code here
	CRect rcDlg;
	GetWindowRect(&rcDlg);
	if (!bExtHiden)
	{
		rcDlg.bottom = rcDlg.top + 520;
		GetDlgItem(IDC_BN_HIDE)->SetWindowText("SHOW >>");
	}
	else
	{
		rcDlg.bottom = rcDlg.top + 800;
		GetDlgItem(IDC_BN_HIDE)->SetWindowText("HIDE <<");
	}
	bExtHiden = !bExtHiden;
	MoveWindow(rcDlg);
}

void CTestDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	m_menu.DrawItem(lpDrawItemStruct);
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CTestDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	m_menu.MeasureItem(lpMeasureItemStruct);
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	UpdateData(FALSE); 

    CChartDlg dlg;
    dlg.DoModal();

}

LRESULT CTestDlg::OnThreadFinished( WPARAM wParam,LPARAM lParam )
{
	AfxMessageBox("Thread has exited");
	return 0;	
}

LRESULT CTestDlg::OnThreadUpdateProgress( WPARAM wParam,LPARAM lParam )
{
	m_progress.SetPos(100*(int)wParam/(int)lParam);
	return 0;	
}
