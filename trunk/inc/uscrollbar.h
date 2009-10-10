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
    
    //
    BOOL enable();
    BOOL disable();
    
    //
    BOOL setRange(int nMin, int nMax);
    BOOL getRange(int *pMin, int *pMax);
    //
    int setScrollPos(int nPos);
	int getScrollPos();

	//
	int setScrollInfo(LPCSCROLLINFO lpsi);
	BOOL getScrollInfo(LPSCROLLINFO lpsi);
protected:
private:
};

#endif // U_SCROLLBAR_H

