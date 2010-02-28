#ifndef U_TABCHILD_H
#define U_TABCHILD_H

#include "utoolbar.h"
#include "ulistview.h"
#include "uusagebar.h"
#include "uusagegraph.h"

#include "usys.h"

class UTabChild : public UDialogBox
{
public:
    UTabChild(HINSTANCE hInst, UINT nID, HWND hParent)
    : UDialogBox(hInst, nID, UDialogBox::DefaultDlgProc, hParent)
    {}

    virtual BOOL DialogProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        if( WM_CTLCOLORDLG == message || WM_CTLCOLORSTATIC == message)
        {
            //return (BOOL)::GetStockObject(NULL_BRUSH);
            return (BOOL)::GetSysColorBrush(COLOR_3DHIGHLIGHT);
        }

        //if ( WM_MOVE == message )
        //{
            //::RedrawWindow(m_hDlg, NULL, NULL, RDW_UPDATENOW);
            //return redraw();
       // }

        return UDialogBox::DialogProc(message, wParam, lParam);
    }



    virtual BOOL onInit()
    {
        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDM_EXIT:
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    void setRedraw(BOOL bRedraw = TRUE)
    {
        //this->sendMsg(WM_SETREDRAW, (WPARAM)bRedraw);
        //::LockWindowUpdate(m_hDlg);
        //::RedrawWindow(m_hDlg,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_UPDATENOW);
        //this->redraw();
        UpdateWindow(m_hDlg);
        //this->sendMsg(WM_SETREDRAW, (WPARAM)bRedraw);
        //this->sendMsg(WM_PAINT);
    }
};

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    if (NULL != ::GetParent(hwnd))
    {
    }
    else if (WS_EX_TOOLWINDOW & ::GetWindowLong(hwnd, GWL_EXSTYLE))
    {
    }
    else if (!::IsWindowVisible(hwnd))
    {
    }
    else if (0 == ::GetWindowTextLength(hwnd))
    {
    }
    else
    {
        TCHAR title[512];
       if ( 0 == ::GetWindowText(hwnd, title, 512))
       {
           return FALSE;
       }

       if (0 != stricmp(title, "Program Manager"))
       {
            static int nCount = 1;
            HWND hListView = (HWND)lParam;
            LOG_STRING("Window Title: %s", title);
            UListView ulv;
            ulv.fromHandle(hListView);
            ulv.addItemTextImage(nCount, title, 0);
            nCount++;
       }
    }

    return TRUE;
}

class UTabChildApp : public UTabChild
{
public:
    UTabChildApp(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    virtual BOOL onInit()
    {
        HWND hListView;
        hListView = GetDlgItem(m_hDlg, IDC_LIST_APPLICATIONS);
        //
        UListView ulv;
        ulv.fromHandle(hListView);
        ulv.addColTextWidth(0, "Task", 175);
        ulv.addColTextWidth(1, "Status", 75);
        ulv.addColTextWidth(2, "Extra", 75);

        //
        addWindowTitles();

		ulv.setBKColor(RGB(0,192,192));
		ulv.setTextBKColor(RGB(0,192,192));
		ulv.setTextColor(huys::white);

        return TRUE;
    }

    void enumWindow()
    {
        //HDESK hDesk = ::GetThreadDesktop(::GetCurrentThreadId());
        HWND hListView = GetDlgItem(m_hDlg, IDC_LIST_APPLICATIONS);
        ::EnumDesktopWindows(NULL, &MyEnumWindowsProc, (LPARAM)hListView);
    }

    void addWindowTitles()
    {
        //HWND hListView = GetDlgItem(m_hDlg, IDC_LIST_APPLICATIONS);

  //      int iRow = 1;
  //
  //      HWND hShellTrayWnd = ::FindWindow("Shell_TrayWnd", NULL);
  //
        //HWND hRebarWindow = ::FindWindowEx(hShellTrayWnd, NULL, "ReBarWindow32", NULL);

        //HWND hTaskWindow = ::FindWindowEx(hRebarWindow, NULL, "MSTaskSwWClass", NULL);

        //HWND hToolbar = ::FindWindowEx(hRebarWindow, NULL, "ToolbarWindow32", NULL);
  //
        //if (NULL != hToolbar)
        //{
        //    ListView_SetItemText(hListView, 0, 0, "none");
        //}

        //UToolBar utl;
        //utl.fromHandle(hToolbar);
        //int nBnCount = utl.getButtonCount();
        //char buf[256];
        //wsprintf(buf, "%d", nBnCount);
        //ListView_SetItemText(hListView, 0, 1, buf);

        ////utl.getString(10, buf, 256);
        //TBBUTTON tbbn = {0};
        //char largeBuf[1024] = {0};
        //for (int i=0; i<nBnCount; ++i)
        //{
        //    ::ZeroMemory(&tbbn, sizeof(TBBUTTON));
        //    utl.getButton(i, &tbbn);
        //    utl.getButtonText(tbbn.idCommand, buf);
        //    wsprintf(largeBuf, "%s%d%d", largeBuf, tbbn.idCommand, tbbn.iString);
        //}

        //ListView_SetItemText(hListView, 0, 2, largeBuf);
        enumWindow();

    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_NEW_TASK:
            return onBnNewTask();
        default:
            return UDialogBox::onCommand(wParam, lParam);
        }
    }

    BOOL onBnNewTask()
    {
        addWindowTitles();
        return FALSE;
    }
};

class UTabChildPrc : public UTabChild
{
public:
    UTabChildPrc(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    virtual BOOL onInit()
    {
        HWND hListView;
        hListView = GetDlgItem(m_hDlg, IDC_LIST_PROCESSES);
        //
        UListView ulv;
        ulv.fromHandle(hListView);
        ulv.addColTextWidth(0, "Image Name", 125);
        ulv.addColTextWidth(1, "PID", 75);
        ulv.addColTextWidth(2, "User Name", 75);

        ulv.addItemTextImage(0, "System Idle Process", 0);
        ulv.setItemText(0, 1, "0");
        ulv.setItemText(0, 2, "SYSTEM");

        ulv.addItemTextImage(1, "Notepad++", 0);
        ulv.setItemText(1, 1, "0");
        ulv.setItemText(1, 2, "SYSTEM");

		ulv.setBKColor(RGB(0,192,192));
		ulv.setTextBKColor(RGB(0,192,192));
		ulv.setTextColor(huys::white);


        return TRUE;
    }

    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case IDC_BUTTON_END_PRC:
            return onBnEndProcess();
        default:
            return UTabChild::onCommand(wParam, lParam);
        }
    }

private:
    BOOL onBnEndProcess()
    {
        if (isProgramUp(_T("notepad++.exe")))
        {
            stopProgram(_T("notepad++.exe"));
        }
        else
        {
            showMsg("Missing!");
        }
        return FALSE;
    }
};

class UTabChildSvc : public UTabChild
{
public:
    UTabChildSvc(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    virtual BOOL onInit()
    {
        HWND hListView;
        hListView = GetDlgItem(m_hDlg, IDC_LIST_SERVICES);

        UListView ulv;
        ulv.fromHandle(hListView);
        ulv.addColTextWidth(0, "Service Name", 125);
        ulv.addColTextWidth(1, "path", 200);
        ulv.addColTextWidth(2, "Status", 75);

        ulv.addItemTextImage(0, "Dummy", 0);
        ulv.setItemText(0, 1, "C:\\Windows\\system32\\service -d");
        ulv.setItemText(0, 2, "Started");

		ulv.setBKColor(RGB(0,192,192));
		ulv.setTextBKColor(RGB(0,192,192));
		ulv.setTextColor(huys::white);


        return TRUE;
    }
};

class UTabChildPrf : public UTabChild
{
    enum {
        ID_UUB_CPU = 30001,
        ID_UUB_PF = 30002
    };
public:
    UTabChildPrf(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    ~UTabChildPrf()
    {
        CHECK_PTR(m_pUsageBarCPU);
        CHECK_PTR(m_pUsageBarPF);
        CHECK_PTR(m_pUsageGraphCPU);
        CHECK_PTR(m_pUsageGraphPF);
    }

    virtual BOOL onInit()
    {
        //m_pUsageBarCPU = new UUsageBar(m_hDlg, ID_UUB_CPU, m_hInst);
        ////m_pUsageBarCPU->setPos(32, 20, 30, 60);
        //m_pUsageBarCPU->create();


        //RECT rcGBCPU;
        //HWND hGBCPU = this->getDlgItem(IDC_GB_CPU_USAGE);
        //::GetClientRect(hGBCPU, &rcGBCPU);
        //rcGBCPU.left += 10;
        //rcGBCPU.top += 20;
        //rcGBCPU.right += 30;
        //rcGBCPU.bottom += 15;
        //::ClientToScreen(hGBCPU, (LPPOINT)&rcGBCPU);
        //::ClientToScreen(hGBCPU, (LPPOINT)(&rcGBCPU+1));
        //::ScreenToClient(m_hDlg, (LPPOINT)&rcGBCPU);
        //::ScreenToClient(m_hDlg, (LPPOINT)(&rcGBCPU+1));
        //m_pUsageBarCPU->setPosition(&rcGBCPU);

        //UUsageBar usCPU;
        m_pUsageBarCPU = new UUsageBar;
        m_pUsageBarCPU->subclass(IDC_USAGE_BAR_CPU, m_hDlg);
        m_pUsageBarCPU->invalidate(TRUE);

        //m_pUsageBarPF = new UUsageBar(m_hDlg, ID_UUB_PF, m_hInst);
        //m_pUsageBarPF->create();

        //RECT rcGBPF;
        //HWND hGBPF = this->getDlgItem(IDC_GB_PF_USAGE);
        //::GetClientRect(hGBPF, &rcGBPF);
        //rcGBPF.left = 50;
        //rcGBPF.top = 200;
        //rcGBPF.right = rcGBPF.left+80;
        //rcGBPF.bottom = rcGBPF.top + 110;
        ////::ClientToScreen(hGBPF, (LPPOINT)&rcGBPF);
        ////::ClientToScreen(hGBPF, (LPPOINT)(&rcGBPF+1));
        ////::ScreenToClient(m_hDlg, (LPPOINT)&rcGBPF);
        ////::ScreenToClient(m_hDlg, (LPPOINT)(&rcGBPF+1));
        //m_pUsageBarPF->setPosition(&rcGBPF);

        m_pUsageBarPF = new UUsageBar;
        m_pUsageBarPF->subclass(IDC_USAGE_BAR_PF, m_hDlg);
        m_pUsageBarPF->invalidate(TRUE);

        m_pUsageGraphCPU = new UUsageGraph;
        m_pUsageGraphCPU->subclass(IDC_USAGE_GRAPH_CPU, m_hDlg);
        m_pUsageGraphCPU->invalidate(TRUE);


        m_pUsageGraphPF = new UUsageGraph;
        m_pUsageGraphPF->subclass(IDC_USAGE_GRAPH_PF, m_hDlg);
        m_pUsageGraphPF->invalidate(TRUE);

        return TRUE;
    }

    void setRedraw(BOOL bRedraw = TRUE)
    {
        //this->sendMsg(WM_SETREDRAW, (WPARAM)bRedraw);
        //::LockWindowUpdate(m_hDlg);
        //::RedrawWindow(m_hDlg,NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_UPDATENOW);
        //this->redraw();
        m_pUsageBarCPU->invalidate(TRUE);
        m_pUsageBarPF->invalidate(TRUE);
        UpdateWindow(m_hDlg);



        //this->sendMsg(WM_SETREDRAW, (WPARAM)bRedraw);
        //this->sendMsg(WM_PAINT);
    }
private:
    UUsageBar *m_pUsageBarCPU;
    UUsageBar *m_pUsageBarPF;
    UUsageGraph *m_pUsageGraphCPU;
    UUsageGraph *m_pUsageGraphPF;

};

class UTabChildNetwork : public UTabChild
{
public:
    UTabChildNetwork(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    ~UTabChildNetwork()
    {
        CHECK_PTR(m_pUsageGraphNet);
    }

    virtual BOOL onInit()
    {
        m_pUsageGraphNet = new UUsageGraph;
        m_pUsageGraphNet->subclass(IDC_NETWORK_UTILIZATION, m_hDlg);
        m_pUsageGraphNet->invalidate(TRUE);

        UListView ulv;
        ulv.fromHandle(::GetDlgItem(m_hDlg, IDC_LIST_ADAPTER));
        ulv.addColTextWidth(0, "Adapter Name", 125);
        ulv.addColTextWidth(1, "Network Utilization", 180);
        ulv.addColTextWidth(2, "Link Speed", 100);
        ulv.addColTextWidth(3, "State", 75);

        return TRUE;
    }
private:
    UUsageGraph *m_pUsageGraphNet;
};

class UTabChildUser : public UTabChild
{
public:
    UTabChildUser(HINSTANCE hInst, UINT nID, HWND hParent)
    : UTabChild(hInst, nID, hParent)
    {}

    virtual BOOL onInit()
    {
        UListView ulv;
        ulv.fromHandle(::GetDlgItem(m_hDlg, IDC_LIST_USER));
        ulv.addColTextWidth(0, "User", 125);
        ulv.addColTextWidth(1, "ID", 100);
        ulv.addColTextWidth(2, "Status", 100);
        ulv.addColTextWidth(3, "Client Name", 75);
        ulv.addColTextWidth(4, "Session", 75);

        TCHAR user[256];
        getUserName(user, 256);

        ulv.addItemTextImage(0, user, 0);
        //ulv.setItemText(0, 1, "0");
        //ulv.setItemText(0, 2, "SYSTEM");
        return TRUE;
    }
};

#endif // U_TABCHILD_H
