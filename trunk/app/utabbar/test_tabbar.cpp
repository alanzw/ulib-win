/*
 * =====================================================================================
 *
 *       Filename:  test_sysadm.cpp
 *
 *    Description:  A little kit for system admin
 *
 *        Version:  1.0
 *        Created:  2009-8-9 2:17:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udlgapp.h"
#include "udialogx.h"

using huys::UDialogBox;

class UDialogTabbar : public UDialogBox
{
public:
    UDialogTabbar(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    ~UDialogTabbar()
    {
    }

    BOOL onInit()
    {
        return TRUE;
    }

private:
};

UDLGAPP_T(UDialogTabbar, IDD_TABBAR);


