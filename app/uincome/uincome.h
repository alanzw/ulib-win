#ifndef U_BUTTONST_H
#define U_BUTTONST_H

#include "ustatic.h"

class UIncome : public UStatic
{
public:
    UIncome(HWND hParent, UINT nID, HINSTANCE hInst);
    UIncome();
    ~UIncome();

    UIncome(UBaseWindow *pWndParent, UINT nID = -1);
private:

};

#endif // U_BUTTONST_H
