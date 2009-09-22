/*
 * =====================================================================================
 *
 *       Filename:  upannel.cpp
 *
 *    Description:  implement
 *
 *        Version:  1.0
 *        Created:  2009-7-3 4:14:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <windows.h>
#include <tchar.h>

#include "upannel.h"
#include "umsg.h"

UPannel::UPannel(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst)
{
    m_dwStyles = WS_CHILD | WS_VISIBLE;
}

UPannel::~UPannel()
{

}

BOOL UPannel::create()
{
    return UControl::createEx(WS_EX_DLGMODALFRAME, _T("STATIC"), NULL);
}

bool UPannel::addControl( UControl *pUCtrl)
{

    pUCtrl->setParentWnd(m_hSelf);

    m_ctrls.push_back(pUCtrl);

    relayout();

    return false;
}

bool UPannel::relayout()
{

    ChildCtrlVector::size_type nCount = m_ctrls.size();

    if (nCount < 1)
    {
        return false;
    }


    RECT rcClient;

    ::GetClientRect(m_hSelf, &rcClient);

    rcClient.left += 2;
    rcClient.right -= 2;
    rcClient.top += 2;
    rcClient.bottom -= 2;

    LONG lAvgSize = (rcClient.right - rcClient.left)/nCount;

    RECT rc;

    rc.left = rcClient.left;
    rc.right = rc.left + lAvgSize;
    rc.top = rcClient.top;
    rc.bottom = rcClient.bottom;

    // ChildCtrlVector::iterator it = m_ctrls.begin();

    //for ( ; it != m_ctrls.end(); ++it)
    for (int i=0; i<nCount; ++i)
    {
        //*it->setPosition(&rc);
        m_ctrls.at(i)->setPosition(&rc);

        rc.left += lAvgSize;
        rc.right += lAvgSize;
    }

    return true;
}

bool UPannel::redirectMsg(HWND hWnd)
{
    m_hMsgWnd = hWnd;
    return this->subclassProc();
}

BOOL UPannel::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (WM_COMMAND == message)
    {
        return ::PostMessage(m_hMsgWnd, message, wParam, lParam);
    }

    return UControl::onMessage(message, wParam, lParam);
}
