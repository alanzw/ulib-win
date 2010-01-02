#ifndef U_EXPLORER_H
#define U_EXPLORER_H

#include "ustatic.h"
#include "utreeview.h"
#include "ulistview.h"

class UExplorer : public UStatic
{
public:
    UExplorer(HWND hParent, UINT nID, HINSTANCE hInst);
    UExplorer();
    UExplorer(UBaseWindow *pWndParent, UINT nID);
    ~UExplorer();

    virtual BOOL create();
private:
    UTreeView *m_tv;
    UListView *m_list;
};

#endif // U_EXPLORER_H
