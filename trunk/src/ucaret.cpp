#include <windows.h>

#include "ucaret.h"

UCaret::UCaret()
{}

UCaret::~UCaret()
{}

bool UCaret::create(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight)
{
    return TRUE == ::CreateCaret(hWnd, hBitmap, nWidth, nHeight);
}

bool UCaret::destroy()
{
    return TRUE == ::DestroyCaret();
}

void UCaret::setPos(int x, int y)
{
    ::SetCaretPos(x, y);
}

void UCaret::show(HWND hWnd)
{
    ::ShowCaret(hWnd);
}

void UCaret::hide(HWND hWnd)
{
    ::HideCaret(hWnd);
}
