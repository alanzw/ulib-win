/*
 * =====================================================================================
 *
 *       Filename:  uwinapp.h
 *
 *    Description:  Class UWinApp
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:12:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_WINAPP_H
#define U_WINAPP_H

#include "ulib.h"

class UBaseWindow;

class ULIB_API UWinApp
{
public:
    UWinApp();
    ~UWinApp();

    bool init(HINSTANCE hInst = NULL);

    DWORD run();

    UBaseWindow *getMainWindow()
    { return m_pMainWindow; }

    void setMainWindow(UBaseWindow *pWin)
    {
        m_pMainWindow = pWin;
    }
protected:
    HINSTANCE m_hInst;
    UBaseWindow *m_pMainWindow;

private:
};

#endif // U_WINAPP_H

