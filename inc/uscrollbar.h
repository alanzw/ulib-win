#ifndef U_SCROLLBAR_H
#define U_SCROLLBAR_H

#include "ucontrol.h"

class ULIB_API UScrollBar : public UControl
{
public:
    UScrollBar(HWND hParent, UINT nID, HINSTANCE hInst);
    ~UScrollBar();
    //
    virtual BOOL create();
protected:
private:
};

#endif // U_SCROLLBAR_H

