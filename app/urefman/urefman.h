#ifndef U_EXPLORER_H
#define U_EXPLORER_H

#include "ustatic.h"
#include "utreeview.h"
#include "ulistview.h"
#include "uedit.h"
#include "uimagelist.h"
#include "adt/uautoptr.h"

class URefMan : public UStatic
{
public:
    URefMan(HWND hParent, UINT nID, HINSTANCE hInst);
    URefMan();
    URefMan(UBaseWindow *pWndParent, UINT nID);
    ~URefMan();

    virtual BOOL create();

    BOOL addCategory(LPCTSTR sCatName, int iImage = 0);
private:
    huys::ADT::UAutoPtr<UTreeView> m_tv;
    huys::ADT::UAutoPtr<UListView> m_list;
    huys::ADT::UAutoPtr<UEdit> m_edt;
    huys::ADT::UAutoPtr<UImageList> m_img;

    HTREEITEM m_mainCategory;
};

#endif // U_EXPLORER_H
