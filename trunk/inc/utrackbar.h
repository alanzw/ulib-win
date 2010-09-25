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

    BOOL setRange(int min, int max, BOOL bRedraw = TRUE);
    BOOL setRangeSize(int nSize);
    BOOL setTicFreq(int nFreq);
    BOOL setTPos(int nNewPos, BOOL bRedraw = TRUE);

    int getRangeMin();
    int getRangeMax();

    void getSelection(int &nMin, int &nMax);

    void getThumbRect(LPRECT lpRect);
protected:
private:

};

#endif // U_TAB_CTRL_H

