#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uads.h"

UADSample::UADSample(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UADSample::UADSample(int port)
: _nPort(port)
{
}

UADSample::UADSample(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID)
{
    m_dwStyles &= ~SS_SIMPLE;
    m_nResourceID = nID;
}

UADSample::~UADSample()
{}

void UADSample::setPort(int port)
{
    _nPort = port;
}

