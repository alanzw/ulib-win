#ifndef U_3DCREATOR_H
#define U_3DCREATOR_H

#include "ustatic.h"
#include "ubasewindow.h"

class UWDemo : public UStatic
{
public:
    UWDemo(HWND hParent, UINT nID, HINSTANCE hInst);
    UWDemo();
    UWDemo(UBaseWindow *pWndParent, UINT nID);
    ~UWDemo();
private:

};

#endif // U_3DCREATOR_H
