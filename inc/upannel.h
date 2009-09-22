#ifndef U_PANNEL_H
#define U_PANNEL_H

#include <vector>

#include "ucontrol.h"

//
class ULIB_API UPannel : public UControl
{
public:
    enum Align {
        UPA_HORIZONTAL,
        UPA_VERTICAL,
        UPA_QUAD,
        UPA_ARRAY
    };
public:
    UPannel(HWND hParent, UINT nID, HINSTANCE hInst);
    ~UPannel();

    BOOL create();

    bool addControl(UControl *pUCtrl);

    BOOL onMessage(UINT,WPARAM,LPARAM);

    //
    bool redirectMsg(HWND hWnd);
private:
    Align m_align;

    typedef std::vector<UControl *> ChildCtrlVector;

    ChildCtrlVector m_ctrls;

    //
    bool relayout();

    HWND m_hMsgWnd;
};

#endif // U_PANNEL_H
