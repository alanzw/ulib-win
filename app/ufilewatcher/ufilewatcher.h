#ifndef U_EDITPAD_H
#define U_EDITPAD_H

#include "ubasewindow.h"

class UFileWatcher : public UBaseWindow
{
public:
    UFileWatcher();
    UFileWatcher(UBaseWindow *pWndParent);
    ~UFileWatcher();

    BOOL onClose();
private:

};

#endif // U_EDITPAD_H
