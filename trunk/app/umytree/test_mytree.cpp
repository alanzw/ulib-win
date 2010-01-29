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

#define _WIN32_IE 0x0300

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "udlgapp.h"
#include "udialogx.h"

#include "umytree.h"

#include "adt/ubinary_tree.h"
#include "adt/uautoptr.h"

using huys::UDialogBox;

class UDialogCtrls : public UDialogBox
{
    enum {
        ID_TREECTRL = 13333
    };
public:
    UDialogCtrls(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {}

    BOOL onInit()
    {
        m_pTreeCtrl = new UMyTreeCtrl(m_hDlg, ID_TREECTRL, m_hInst);
        m_pTreeCtrl->setStyles(WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES );
        m_pTreeCtrl->setPos(50, 50, 600, 300);
        m_pTreeCtrl->create();

        HTREEITEM item = m_pTreeCtrl->addTextRoot(_T("root"));
        item = m_pTreeCtrl->addTextChild(item, _T("child1"));

        return TRUE;
    }

private:
    huys::ADT::UAutoPtr<UMyTreeCtrl> m_pTreeCtrl;

    huys::ADT::Binary_tree<int> btree;
};

UDLGAPP_T(UDialogCtrls, IDD_MYTREE);
