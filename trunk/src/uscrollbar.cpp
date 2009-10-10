#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uscrollbar.h"

UScrollBar :: UScrollBar(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst)
{}

UScrollBar::~UScrollBar()
{}

BOOL UScrollBar::create()
{
    return UControl::create(_T("ScrollBar"));
}

BOOL UScrollBar::enable()
{
    return ::EnableScrollBar(m_hSelf, SB_CTL, ESB_ENABLE_BOTH);
}

BOOL UScrollBar::disable()
{
    return ::EnableScrollBar(m_hSelf, SB_CTL, ESB_DISABLE_BOTH);
}

BOOL UScrollBar::setRange(int nMin, int nMax)
{
    return ::SetScrollRange(m_hSelf, SB_CTL, nMin, nMax, TRUE);
}

BOOL UScrollBar::getRange(int *pMin, int *pMax)
{
	return ::GetScrollRange(m_hSelf, SB_CTL, pMin, pMax);
}

int UScrollBar::setScrollPos(int nPos)
{
    return ::SetScrollPos(m_hSelf, SB_CTL, nPos, TRUE);
}

int UScrollBar::getScrollPos()
{
	return ::GetScrollPos(m_hSelf, SB_CTL);
}

int UScrollBar::setScrollInfo(LPCSCROLLINFO lpsi)
{
	return ::SetScrollInfo(m_hSelf, SB_CTL, lpsi, TRUE);
}

BOOL UScrollBar::getScrollInfo(LPSCROLLINFO lpsi)
{
	return ::GetScrollInfo(m_hSelf, SB_CTL, lpsi);
}

