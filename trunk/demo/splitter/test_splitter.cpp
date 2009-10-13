//
//    This program demonstrates a simple "splitter window"
//
//    Written by J Brown
//

#include <windows.h>

//
//    Globals needed for main window
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Splitter_OnLButtonDown (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT Splitter_OnLButtonUp   (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT Splitter_OnMouseMove   (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

const char    szAppName[] = "Splitter";
HINSTANCE    hInstance;

HWND        hwndMain;
HWND        hwndChild1;
HWND        hwndChild2;

int            nSplitterPos = 100;
int            nSplitterBorder = 2;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEX    wc;
    MSG            msg;

    //
    //    Register our main window class
    //
    wc.cbSize            = sizeof(wc);
    wc.style            = 0;
    wc.lpfnWndProc        = WndProc;
    wc.cbClsExtra        = 0;
    wc.cbWndExtra        = 0;
    wc.hInstance        = hInst;
    wc.hIcon            = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor            = LoadCursor (NULL, IDC_SIZENS);
    wc.hbrBackground    = (HBRUSH)(COLOR_3DFACE+1);;
    wc.lpszMenuName        = 0;
    wc.lpszClassName    = szAppName;
    wc.hIconSm            = LoadIcon (NULL, IDI_APPLICATION);

    RegisterClassEx(&wc);

    hInstance = hInst;

    //
    //    Create the main window. This window
    //  will host two child controls.
    //
    hwndMain = CreateWindowEx(0,        // extended style (not needed)
                szAppName,                // window class name
                szAppName,                // window caption
                WS_OVERLAPPEDWINDOW|
                WS_CLIPCHILDREN,        // window style
                CW_USEDEFAULT,            // initial x position
                CW_USEDEFAULT,            // initial y position
                CW_USEDEFAULT,            // initial x size
                CW_USEDEFAULT,            // initial y size
                NULL,                    // parent window handle
                NULL,                    // use window class menu
                hInst,                    // program instance handle
                NULL);                    // creation parameters

    ShowWindow(hwndMain, nShowCmd);

    while(GetMessage(&msg, NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void SizeWindowContents(int nWidth, int nHeight)
{
    //We will get SetScrollInfo's called by the listview, because
    //it has been changed in size
    MoveWindow(hwndChild1, 0, 0, nWidth, nSplitterPos, TRUE);

    MoveWindow(hwndChild2, 0, nSplitterPos + nSplitterBorder, nWidth, nHeight - nSplitterPos - nSplitterBorder, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:

        hwndChild1 = CreateWindowEx(WS_EX_CLIENTEDGE,
            "EDIT", "",
            WS_VISIBLE|WS_CHILD,
            0,0,0,0,hwnd, 0, hInstance, 0);

        hwndChild2 = CreateWindowEx(WS_EX_CLIENTEDGE,
            "EDIT", "",
            WS_VISIBLE|WS_CHILD,
            0,0,0,0,hwnd, 0, hInstance, 0);

        return 0;

    case WM_SIZE:

        SizeWindowContents(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_LBUTTONDOWN:
        Splitter_OnLButtonDown(hwnd, msg, wParam, lParam);
        return 0;

    case WM_LBUTTONUP:
        Splitter_OnLButtonUp(hwnd, msg, wParam, lParam);
        return 0;

    case WM_MOUSEMOVE:
        Splitter_OnMouseMove(hwnd, msg, wParam, lParam);
        return 0;

    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
//    Here is the splitter functions
//

static int  oldy = -4;
static BOOL fMoved = FALSE;
static BOOL fDragMode = FALSE;

void DrawXorBar(HDC hdc, int x1, int y1, int width, int height)
{
    static WORD _dotPatternBmp[8] =
    {
        0x00aa, 0x0055, 0x00aa, 0x0055,
        0x00aa, 0x0055, 0x00aa, 0x0055
    };

    HBITMAP hbm;
    HBRUSH  hbr, hbrushOld;

    hbm = CreateBitmap(8, 8, 1, 1, _dotPatternBmp);
    hbr = CreatePatternBrush(hbm);

    SetBrushOrgEx(hdc, x1, y1, 0);
    hbrushOld = (HBRUSH)SelectObject(hdc, hbr);

    PatBlt(hdc, x1, y1, width, height, PATINVERT);

    SelectObject(hdc, hbrushOld);

    DeleteObject(hbr);
    DeleteObject(hbm);
}

LRESULT Splitter_OnLButtonDown(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt;
    HDC hdc;
    RECT rect;

    pt.x = (short)LOWORD(lParam);  // horizontal position of cursor
    pt.y = (short)HIWORD(lParam);

    GetWindowRect(hwnd, &rect);

    //convert the mouse coordinates relative to the top-left of
    //the window
    ClientToScreen(hwnd, &pt);
    pt.x -= rect.left;
    pt.y -= rect.top;

    //same for the window coordinates - make them relative to 0,0
    OffsetRect(&rect, -rect.left, -rect.top);

    if(pt.y < 0) pt.y = 0;
    if(pt.y > rect.bottom-4)
    {
        pt.y = rect.bottom-4;
    }

    fDragMode = TRUE;

    SetCapture(hwnd);

    hdc = GetWindowDC(hwnd);
    DrawXorBar(hdc, 1,pt.y - 2, rect.right-2,4);
    ReleaseDC(hwnd, hdc);

    oldy = pt.y;

    return 0;
}


LRESULT Splitter_OnLButtonUp(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    RECT rect;

    POINT pt;
    pt.x = (short)LOWORD(lParam);  // horizontal position of cursor
    pt.y = (short)HIWORD(lParam);

    if(fDragMode == FALSE)
        return 0;

    GetWindowRect(hwnd, &rect);

    ClientToScreen(hwnd, &pt);
    pt.x -= rect.left;
    pt.y -= rect.top;

    OffsetRect(&rect, -rect.left, -rect.top);

    if(pt.y < 0) pt.y = 0;
    if(pt.y > rect.bottom-4)
    {
        pt.y = rect.bottom-4;
    }

    hdc = GetWindowDC(hwnd);
    DrawXorBar(hdc, 1,oldy - 2, rect.right-2,4);
    ReleaseDC(hwnd, hdc);

    oldy = pt.y;

    fDragMode = FALSE;

    //convert the splitter position back to screen coords.
    GetWindowRect(hwnd, &rect);
    pt.x += rect.left;
    pt.y += rect.top;

    //now convert into CLIENT coordinates
    ScreenToClient(hwnd, &pt);
    GetClientRect(hwnd, &rect);
    nSplitterPos = pt.y;

    //position the child controls
    SizeWindowContents(rect.right,rect.bottom);

    ReleaseCapture();

    return 0;
}

LRESULT Splitter_OnMouseMove(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    RECT rect;

    POINT pt;

    if(fDragMode == FALSE) return 0;

    pt.x = (short)LOWORD(lParam);  // horizontal position of cursor
    pt.y = (short)HIWORD(lParam);

    GetWindowRect(hwnd, &rect);

    ClientToScreen(hwnd, &pt);
    pt.x -= rect.left;
    pt.y -= rect.top;

    OffsetRect(&rect, -rect.left, -rect.top);

    if(pt.y < 0) pt.y = 0;
    if(pt.y > rect.bottom-4)
    {
        pt.y = rect.bottom-4;
    }

    if(pt.y != oldy && wParam & MK_LBUTTON)
    {
        hdc = GetWindowDC(hwnd);
        DrawXorBar(hdc, 1,oldy - 2, rect.right-2,4);
        DrawXorBar(hdc, 1,pt.y - 2, rect.right-2,4);

        ReleaseDC(hwnd, hdc);

        oldy = pt.y;
    }

    return 0;
}
