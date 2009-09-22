#include <windows.h>

class ZWindow;

ZWindow *g_pWnd = NULL;

#pragma pack(push,1)
struct _WndProcThunk
{
    DWORD   m_mov;          // mov dword ptr [esp+0x4], pThis (esp+0x4 is hWnd)
    DWORD   m_this;
    BYTE    m_jmp;          // jmp WndProc
    DWORD   m_relproc;      // relative jmp
};
#pragma pack(pop)

class WndProcThunk
{
public:
    _WndProcThunk thunk;

    void Init(WNDPROC proc, void* pThis)
    {
        thunk.m_mov = 0x042444C7;  //C7 44 24 04
        thunk.m_this = (DWORD)pThis;
        thunk.m_jmp = 0xe9;
        thunk.m_relproc = (int)proc - ((int)this+sizeof(_WndProcThunk));
        ::FlushInstructionCache(GetCurrentProcess(), &thunk, sizeof(thunk));
    }
};

class ZWindow
{
public:
    virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return FALSE;
    }

    HWND m_hWnd;
    WndProcThunk    m_thunk;

    ZWindow(HWND hWnd = 0) : m_hWnd(hWnd) { }

    inline void Attach(HWND hWnd)
    { m_hWnd = hWnd; }

    inline BOOL ShowWindow(int nCmdShow)
    { return ::ShowWindow(m_hWnd, nCmdShow); }

    inline BOOL UpdateWindow()
    {  return ::UpdateWindow(m_hWnd); }

    inline HDC BeginPaint(LPPAINTSTRUCT ps)
    {  return ::BeginPaint(m_hWnd, ps); }

    inline BOOL EndPaint(LPPAINTSTRUCT ps)
    {  return ::EndPaint(m_hWnd, ps); }

    inline BOOL GetClientRect(LPRECT rect)
    {  return ::GetClientRect(m_hWnd, rect); }

    BOOL Create(LPCTSTR szClassName,
               LPCTSTR szTitle,
               HINSTANCE hInstance,
               HWND hWndParent = 0,
               DWORD dwStyle = WS_OVERLAPPEDWINDOW,
                DWORD dwExStyle = 0,
                HMENU hMenu = 0,
                int x = CW_USEDEFAULT,
                int y = CW_USEDEFAULT,
                int nWidth = CW_USEDEFAULT,
                int nHeight = CW_USEDEFAULT)
    {
        m_hWnd = ::CreateWindowEx(
            dwExStyle,
            szClassName,
            szTitle,
            dwStyle,
            x,
            y,
            nWidth,
            nHeight,
            hWndParent,
            hMenu,
            hInstance,
            NULL);

        return m_hWnd != NULL;
    }

    static LRESULT CALLBACK StartWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        ZWindow *pThis = g_pWnd;
        pThis->m_hWnd = hWnd;

        // initilize the thunk code
        pThis->m_thunk.Init(WindowProc, pThis);

        // get the address of thunk code
        WNDPROC pProc = (WNDPROC)&(pThis->m_thunk.thunk);

        ::SetWindowLong(hWnd, GWL_WNDPROC, (LONG)pProc);

        return pProc(hWnd, uMsg, wParam, lParam);
    }

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        ZWindow* pThis = (ZWindow*)hWnd;

        if (uMsg == WM_NCDESTROY)
            ::PostQuitMessage(0);

        if (!pThis->ProcessWindowMessage(pThis->m_hWnd, uMsg, wParam, lParam))
            return ::DefWindowProc(pThis->m_hWnd, uMsg, wParam, lParam);
        else
            return 0;
    }
};

class ZDriveWindow1 : public ZWindow
{
public:
    virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT OnPaint(WPARAM wParam, LPARAM lParam)
    {
        HDC hDC;
        PAINTSTRUCT ps;
        RECT rect;

        hDC = BeginPaint(&ps);
        GetClientRect(&rect);
        ::SetBkMode(hDC, TRANSPARENT);
        ::DrawText(hDC, "ZDriveWindow1", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(&ps);

        return 0;
    }

    LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::MessageBox(NULL, "ZDriveWindow1::OnLButtonDown", "Msg", MB_OK);
        return 0;
    }
};

class ZDriveWindow2 : public ZWindow
{
public:
    virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnPaint(WPARAM wParam, LPARAM lParam)
    {
        HDC hDC;
        PAINTSTRUCT ps;
        RECT rect;

        hDC = BeginPaint(&ps);
        GetClientRect(&rect);
        ::SetBkMode(hDC, TRANSPARENT);
        ::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
        ::DrawText(hDC, "ZDriveWindow2", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(&ps);

        return 0;
    }

    LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        ::MessageBox(NULL, "ZDriveWindow2::OnLButtonDown", "Msg", MB_OK);
        return 0;
    }

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    char szAppName[] = "Hello world";
    MSG msg;
    WNDCLASS wnd;
    ZDriveWindow1 zwnd1;
    ZDriveWindow2 zwnd2;

    wnd.cbClsExtra        = NULL;
    wnd.cbWndExtra        = NULL;
    wnd.hbrBackground    = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wnd.hCursor            = ::LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon            = ::LoadIcon(NULL, IDI_APPLICATION);
    wnd.hInstance        = hInstance;
    wnd.lpfnWndProc        = ZWindow::StartWndProc;
    wnd.lpszClassName    = szAppName;
    wnd.lpszMenuName    = NULL;
    wnd.style            = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wnd))
    {
        ::MessageBox(NULL, "Can not register window class", "Error", MB_OK | MB_ICONINFORMATION);
        return -1;
    }

    g_pWnd = &zwnd1;
    zwnd1.Create(szAppName, "Hell world", hInstance);

    zwnd1.ShowWindow(nCmdShow);
    zwnd1.UpdateWindow();

    g_pWnd = &zwnd2;
    zwnd2.Create(szAppName, "Test", hInstance, zwnd1.m_hWnd,
        WS_VISIBLE | WS_CHILD | ES_MULTILINE, NULL, NULL, 0, 0, 150, 150);
    zwnd2.ShowWindow(nCmdShow);

    while (::GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

BOOL ZDriveWindow1::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        OnLButtonDown(wParam, lParam);
        break;

    case WM_PAINT:
        OnPaint(wParam, lParam);
        break;
    }

    return FALSE;
}

BOOL ZDriveWindow2::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        OnLButtonDown(wParam, lParam);
        break;

    case WM_PAINT:
        OnPaint(wParam, lParam);
        break;
    }

    return FALSE;
}
