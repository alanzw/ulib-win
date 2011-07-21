/*
 * =====================================================================================
 *
 *       Filename:  uwinapp.cpp
 *
 *    Description:  UWinApp implement
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:10:12
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

#include "uwinapp.h"
#include "ubasewindow.h"

UWinApp::UWinApp()
: m_hInst(NULL),
  m_pMainWindow(NULL)
{}


UWinApp::~UWinApp()
{
    CHECK_PTR(m_pMainWindow);
}

bool UWinApp::init( HINSTANCE hInst /*= NULL*/ )
{
    if (NULL == m_pMainWindow)
    {
        m_pMainWindow = new UBaseWindow(NULL, hInst);
    }

    if (NULL == m_pMainWindow->getHandle())
    {
        m_pMainWindow->create();
    }

    return true;
}


DWORD UWinApp::run()
{
    m_pMainWindow->show();
    m_pMainWindow->update();

    MSG  msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}


