#include <windows.h>
#include <tchar.h>
#include <commctrl.h>



#include "uexplorer.h"

UExplorer::UExplorer(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst)
{
}

UExplorer::UExplorer()
{
}

UExplorer::~UExplorer()
{
    CHECK_PTR(m_tv);
}

UExplorer::UExplorer(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID)
{}

BOOL UExplorer::create()
{
    BOOL bRet = UStatic::create();

    m_tv = new UTreeView(m_hSelf, 11111, m_hInstance);

    m_tv->setStyles(TVS_HASLINES| TVS_LINESATROOT | TVS_HASBUTTONS);

    RECT rc;
    this->getClientRect(&rc);

    rc.right = rc.left+150;
    rc.bottom -= 5;

    m_tv->setRect(&rc);

    m_tv->create();

    char str[] = "My Computer";

    HTREEITEM item = m_tv->addTextRoot(str);

    char str2[] = "C:";
    m_tv->addTextChild(item, str2);

    char str3[] = "D:";
    m_tv->addTextChild(item, str3);

    m_list = new UListView(m_hSelf, 11112, m_hInstance);

    m_list->setStyles(LVS_REPORT | LVS_EDITLABELS);

    this->getClientRect(&rc);

    rc.left = rc.left + 150;
    rc.right -= 5;
    m_list->setRect(&rc);

    m_list->create();

    m_list->addColTextWidth(0, "Name", 100);
    m_list->addColTextWidth(1, "Size", 100);
    m_list->addColTextWidth(2, "Type", 100);
    m_list->addColTextWidth(3, "Date Modified", 300);

    return bRet;
}
