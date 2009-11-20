#ifndef U_GLASS_BUTTON_H
#define U_GLASS_BUTTON_H

#include "ubutton.h"

class UGlassButton : public UOwnerDrawnButton
{
public:
    UGlassButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    : UOwnerDrawnButton(hParent, nResource, hInst)
    {

    }

    ~UGlassButton()
    {

    }

private:

};

#endif // U_GLASS_BUTTON_H
