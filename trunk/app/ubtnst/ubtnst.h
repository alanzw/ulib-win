#ifndef U_BUTTONST_H
#define U_BUTTONST_H

#include "ubutton.h"

class UButtonST : public UButton
{
public:
    
public:
    UButtonST(HWND hParent, UINT nID, HINSTANCE hInst);
    UButtonST();
    UButtonST(UBaseWindow *pWndParent, UINT nID);
    ~UButtonST();
public:
    
private:

};

#endif // U_BUTTONST_H
