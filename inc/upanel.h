#ifndef U_PANEL_H
#define U_PANEL_H

#include <vector>

#include "ucontrol.h"

//
class ULIB_API UPanel : public UControl
{
public:
    enum Align {
        UPA_HORIZONTAL,
        UPA_VERTICAL,
        UPA_QUAD,
        UPA_ARRAY
    };
public:
    UPanel(HWND hParent, UINT nID, HINSTANCE hInst);
    ~UPanel();

    virtual BOOL create();

    bool addControl(UControl *pUCtrl);

    BOOL onMessage(UINT,WPARAM,LPARAM);

    //
    bool redirectMsg(HWND hWnd);

	//
	void setMsgWnd(HWND hWnd)
	{ m_hMsgWnd = hWnd; }
private:
    Align m_align;

    typedef std::vector<UControl *> ChildCtrlVector;

    ChildCtrlVector m_ctrls;

    //
    bool relayout();

    HWND m_hMsgWnd;
};

#endif // U_PANEL_H
