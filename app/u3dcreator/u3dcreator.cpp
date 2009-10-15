#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "u3dcreator.h"

U3DCreator::U3DCreator(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

U3DCreator::U3DCreator()
{
}

U3DCreator::U3DCreator(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, NULL)
{
    m_nResourceID = nID;
}

U3DCreator::~U3DCreator()
{}

