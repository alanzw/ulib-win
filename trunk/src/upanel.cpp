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

#include "upanel.h"
#include "umsg.h"

UPanel::UPanel(HWND hParent, UINT nID, HINSTANCE hInst)
: UControl(hParent, nID, hInst),
  m_align(UPA_HORIZONTAL)
{
    m_dwStyles = WS_CHILD | WS_VISIBLE;
}

UPanel::~UPanel()
{

}

BOOL UPanel::create()
{
    return UControl::createEx(WS_EX_DLGMODALFRAME, _T("STATIC"), NULL);
}

bool UPanel::addControl( UControl *pUCtrl)
{

    pUCtrl->setParentWnd(m_hSelf);

    m_ctrls.push_back(pUCtrl);

    relayout();

    return false;
}

bool UPanel::relayout()
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

    if (UPA_HORIZONTAL == m_align)
    {
        LONG lAvgSize = (rcClient.right - rcClient.left)/nCount;

        RECT rc;

        rc.left = rcClient.left;
        rc.right = rc.left + lAvgSize;
        rc.top = rcClient.top;
        rc.bottom = rcClient.bottom;

        // ChildCtrlVector::iterator it = m_ctrls.begin();

        //for ( ; it != m_ctrls.end(); ++it)
        for (unsigned int i=0; i<nCount; ++i)
        {
            //*it->setPosition(&rc);
            m_ctrls.at(i)->setPosition(&rc);
            rc.left += lAvgSize;
            rc.right += lAvgSize;
        }
    }
    else if (UPA_VERTICAL == m_align)
    {
        LONG lAvgSize = (rcClient.bottom - rcClient.top)/nCount;

        RECT rc;

        rc.left = rcClient.left;
        rc.right = rcClient.right;
        rc.top = rcClient.top;
        rc.bottom = rc.top + lAvgSize;

        // ChildCtrlVector::iterator it = m_ctrls.begin();

        //for ( ; it != m_ctrls.end(); ++it)
        for (unsigned int i=0; i<nCount; ++i)
        {
            //*it->setPosition(&rc);
            m_ctrls.at(i)->setPosition(&rc);
            rc.top += lAvgSize;
            rc.bottom += lAvgSize;
        }
    }

    return true;
}

bool UPanel::redirectMsg(HWND hWnd)
{
    m_hMsgWnd = hWnd;
    return TRUE == this->subclassProc();
}

BOOL UPanel::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (WM_COMMAND == message)
    {
        return ::PostMessage(m_hMsgWnd, message, wParam, lParam);
    }

    return UControl::onMessage(message, wParam, lParam);
}
