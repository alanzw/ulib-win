#ifndef U_Airfoil_H
#define U_Airfoil_H

#include "aui/aui_label.h"

class UAirfoil : public AUI::UTransLabel
{
    enum {
        ID_TIMER_UPDATE = 1111
    };
public:
    UAirfoil(HWND hParent, UINT nID, HINSTANCE hInst);
    UAirfoil();
    ~UAirfoil();
    UAirfoil(UBaseWindow *pWndParent, UINT nID = -1);

    //
    //BOOL onPaint();

private:

};

#endif // U_Airfoil_H
