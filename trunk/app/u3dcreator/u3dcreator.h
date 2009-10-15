#ifndef U_3DCREATOR_H
#define U_3DCREATOR_H

#include "ustatic.h"
#include "ubasewindow.h"

class U3DCreator : public UStatic
{
public:
    U3DCreator(HWND hParent, UINT nID, HINSTANCE hInst);
    U3DCreator();
    U3DCreator(UBaseWindow *pWndParent, UINT nID);
    ~U3DCreator();
private:

};

#endif // U_3DCREATOR_H
