#ifndef U_EDITPAD_H
#define U_EDITPAD_H

#include "ubasewindow.h"

class UEditPad : public UBaseWindow
{
public:
    UEditPad();
    UEditPad(UBaseWindow *pWndParent);
    ~UEditPad();
    
    BOOL onClose();
private:

};

#endif // U_EDITPAD_H
