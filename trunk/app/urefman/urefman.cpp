#include "resource.h"

#define _WIN32_IE 0x0300

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "urefman.h"

URefMan::URefMan(HWND hParent, UINT nID, HINSTANCE hInst)
: UStatic(hParent, nID, hInst), m_mainCategory(0)
{}

URefMan::URefMan()
{}

URefMan::~URefMan()
{}

URefMan::URefMan(UBaseWindow *pWndParent, UINT nID)
: UStatic(pWndParent, nID), m_mainCategory(0)
{}

BOOL URefMan::create()
{
    BOOL bRet = UStatic::create();

    m_tv = new UTreeView(m_hSelf, 11111, m_hInstance);

    m_tv->setStyles(TVS_HASLINES| TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES);

    RECT rc;
    this->getClientRect(&rc);

    rc.right = rc.left+150;
    rc.bottom -= 5;

    m_tv->setRect(&rc);
    m_tv->create();

    m_img = new UImageList(IDB_IMAGES, m_hInstance);
    m_tv->setNormalImageList(m_img);

    char str[] = "My References";

    m_mainCategory = m_tv->addTextRoot(str);

    m_list = new UListView(m_hSelf, 11112, m_hInstance);
    m_list->setStyles(LVS_REPORT | LVS_EDITLABELS );

    this->getClientRect(&rc);

    rc.left = rc.left + 150;
    rc.right -= 5;
    rc.bottom -= 200;
    m_list->setRect(&rc);

    m_list->create();

    m_list->addColTextWidth(0, "Name", 100);
    m_list->addColTextWidth(1, "Size", 100);
    m_list->addColTextWidth(2, "Type", 100);
    m_list->addColTextWidth(3, "Date Modified", 300);

    rc.bottom += 200;
    rc.top = rc.bottom - 190;

    m_edt = new UEdit(m_hSelf, 11113, m_hInstance);
    m_edt->setRect(&rc);
    m_edt->create();

    return bRet;
}

BOOL URefMan::addCategory(LPCTSTR sCatName, int iImage /*= 0*/)
{
    m_tv->addTextChild(m_mainCategory, sCatName, iImage);
    return TRUE;
}
