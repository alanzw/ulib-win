#ifndef U_TREEVIEW_H
#define U_TREEVIEW_H

#include "ucontrol.h"

class ULIB_API UTreeView : public UControl
{
public:
    UTreeView(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UTreeView();

    UTreeView();
    
    UTreeView(UBaseWindow *pWndParent, UINT nID);
    //
    virtual BOOL create();

    //
    virtual HTREEITEM addItem(TVINSERTSTRUCT *ptvis);
    virtual HTREEITEM addItemRoot(TVITEM *ptvi);
    virtual HTREEITEM addItemChild(HTREEITEM hParent, TVITEM *ptvi);
    //
    virtual HTREEITEM addTextRoot(LPCTSTR lpText);
    virtual HTREEITEM addTextChild(HTREEITEM hParent, LPCTSTR lpText);
    //
    virtual BOOL setImageList(HIMAGELIST himl, int iImage);
    virtual BOOL setNormalImageList(HIMAGELIST himl);
    virtual BOOL setStateImageList(HIMAGELIST himl);
    virtual BOOL removeImageList();
    //
    virtual BOOL getItem(TVITEM *ptvi);
    /*
    virtual HTREEITEM getHitItem(TVITEM *ptvi);
    virtual LPSTR getHitText(LPSTR lpText);
    */
    virtual LPSTR getSelectedItem(LPSTR lpText);

    //
    virtual BOOL onLButtonDown(WPARAM,LPARAM);
protected:
    BOOL HitTest(LPTVHITTESTINFO lpht);
private:
};

#endif // U_TREEVIEW_H

