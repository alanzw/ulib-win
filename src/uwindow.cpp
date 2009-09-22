#include "uwindow.h"


MESSAGE_MAP_BEGIN(UWindow)
MESSAGE(WM_PAINT, UWindow::onPaint)
MESSAGE(WM_CLOSE, UWindow::onClose)
MESSAGE(WM_CHAR, UWindow::onChar)
MESSAGE_MAP_END()

//
void UWindow::onPaint()
{
    PAINTSTRUCT ps;
    HDC hdc;
    HBRUSH hbrush;
    HPEN hpen;
    hdc = BeginPaint(_hSelf, &ps);
    RECT rt;
    GetClientRect(_hSelf, &rt);
    rt.top = 5;
    DrawText(hdc, "Hello World!", strlen("Hello World!"), &rt, DT_SINGLELINE|DT_CENTER|DT_VCENTER );

    MoveToEx(hdc, 0, 0, NULL);
    LineTo(hdc, rt.right, rt.bottom);
    MoveToEx(hdc, rt.left, rt.bottom, NULL);
    LineTo(hdc, rt.right, rt.top);


    // Create an array of TRIVERTEX structures that describe
    // positional and color values for each vertex. For a rectangle,
    // only two vertices need to be defined: upper-left and lower-right.
    TRIVERTEX vertex[2] ;
    vertex[0].x     = 0;
    vertex[0].y     = 0;
    vertex[0].Red   = 0x0000;
    vertex[0].Green = 0x8000;
    vertex[0].Blue  = 0x8000;
    vertex[0].Alpha = 0x0000;

    vertex[1].x     = 300;
    vertex[1].y     = 80;
    vertex[1].Red   = 0x0000;
    vertex[1].Green = 0xd000;
    vertex[1].Blue  = 0xd000;
    vertex[1].Alpha = 0x0000;

    // Create a GRADIENT_RECT structure that
    // references the TRIVERTEX vertices.
    GRADIENT_RECT gRect;
    gRect.UpperLeft  = 0;
    gRect.LowerRight = 1;

    // Draw a shaded rectangle.
    ::GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);


    EndPaint(_hSelf, &ps);
}

//
void UWindow::onClose()
{
    PostQuitMessage(0);
}

//
void UWindow::onChar()
{
    MessageBox(_hSelf, "xx", "xx", MB_OK);
}


LRESULT CALLBACK WindowProcedure(HWND hWnd,
                              UINT nMsg,
                              WPARAM wParam,
                              LPARAM lParam)
{
    UWindow *pWnd = theApp->GetWnd();
    if(pWnd->getHWnd() == 0)
        return DefWindowProc(hWnd, nMsg, wParam, lParam);

    return pWnd->WindowProc(nMsg, wParam, lParam);
}

