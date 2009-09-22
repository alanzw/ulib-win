#ifndef U_TAB_CTRL_H
#define U_TAB_CTRL_H

#include "ucontrol.h"

class ULIB_API UTrackBar : public UControl
{
public:
    UTrackBar(HWND hParent, UINT nResource, HINSTANCE hInst);
    UTrackBar() {};
    virtual ~UTrackBar();
    virtual BOOL create();
protected:
private:

};

#endif // U_TAB_CTRL_H

