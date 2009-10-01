#include <windows.h>

#include "ucursor.h"

UCursor::UCursor()
: UUserObject()
{
    m_cursorInfo.cbSize = sizeof(CURSORINFO);
}

UCursor::~UCursor()
{}

bool UCursor::destroy()
{
    return ::DestroyCursor((HCURSOR)m_hObj);
}

bool UCursor::getCursorInfo()
{
    return ::GetCursorInfo(&m_cursorInfo);
}

bool UCursor::getCursorPos(LPPOINT lpPoint)
{
    return ::GetCursorPos(lpPoint);
}

HCURSOR UCursor::getCursor()
{
    return ::GetCursor();
}

bool UCursor::loadStdCursor(LPCTSTR lpCursorName)
{
    m_hObj = ::LoadCursor(NULL, lpCursorName);
    return (NULL != m_hObj);
}

bool UCursor::loadFromRes(HINSTANCE hInst, LPCTSTR lpCursorName)
{
    m_hObj = ::LoadCursor(hInst, lpCursorName);
    return (NULL != m_hObj);
}

bool UCursor::loadFromResID(HINSTANCE hInst, UINT uID)
{
    m_hObj = ::LoadCursor(hInst, MAKEINTRESOURCE(uID));
    return (NULL != m_hObj);
}

bool UCursor::loadFromFile(LPCTSTR lpFileName)
{
    m_hObj = ::LoadCursorFromFile(lpFileName);
    return (NULL != m_hObj);
}

bool UCursor::createCursor(
    HINSTANCE hInst,
    int xHotSpot,
    int yHotSpot,
    int nWidth,
    int nHeight,
    const VOID *pvANDPlane,
    const VOID *pvXORPlane )
{
    m_hObj = ::CreateCursor(hInst, xHotSpot, yHotSpot, nWidth, nHeight,
                            pvANDPlane, pvXORPlane);
    return (NULL != m_hObj);
}

bool UCursor::use()
{
    m_hOldCursor = ::SetCursor((HCURSOR)m_hObj);
    return (NULL != m_hOldCursor);
}

bool UCursor::restore()
{
    if (NULL == m_hOldCursor)
    {
        return false;
    }
    ::SetCursor(m_hOldCursor);
    return true;
}

bool UCursor::getClipCursor(LPRECT lpRect)
{
    return ::GetClipCursor(lpRect);
}

bool UCursor::clipCursor(const RECT *lpRect)
{
    return ::ClipCursor(lpRect);
}

bool UCursor::copy(HCURSOR hCursor)
{
    // !!! CopyCursor is a macro.
    m_hObj = CopyCursor(hCursor);
    return (NULL != m_hObj);
}

bool UCursor::setPos(int x, int y)
{
    return ::SetCursorPos(x, y);
}
