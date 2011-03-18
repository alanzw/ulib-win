#ifndef U_CTRLMSG_H
#define U_CTRLMSG_H

#include "utooltip.h"

class UCtrlMsgBar : public UToolTip
{
public:
    UCtrlMsgBar() {}

    ~UCtrlMsgBar()
    {
    }

    //
    BOOL attach(HWND hWnd)
    {
        return FALSE;
    }

    //
    HWND detach()
    {
        return NULL;
    }

    //
private:
    HWND hTarget;
};



#endif // U_CTRLMSG_H
