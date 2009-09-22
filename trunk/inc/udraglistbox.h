#ifndef U_DRAGLISTBOX_H
#define U_DRAGLISTBOX_H

#include "ulistbox.h"

class ULIB_API UDragListBox : public UListBox
{
public:
    UDragListBox(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UDragListBox();
    //
    virtual BOOL create();
protected:
private:
};

#endif // U_DRAGLISTBOX_H

