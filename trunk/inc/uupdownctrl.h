#ifndef U_UPDOWNCTRL_H
#define U_UPDOWNCTRL_H

#include "ucontrol.h"

class ULIB_API UUpDownCtrl : public UControl
{
public:
    UUpDownCtrl(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UUpDownCtrl();

    //
    virtual BOOL create();

    //
    virtual BOOL setBase(int nBase);
    virtual BOOL setBuddy(HWND hwndBuddy);
    virtual BOOL setPos(int nPos);
    virtual BOOL setRange(int nUpper, int nLower);
    //
#if (_WIN32_IE >= 0x0400)
    virtual BOOL setPos32(int nPos);
    virtual BOOL setRange32(int iLow, int iHigh);
#endif // _WIN32_IE >= 0x0400
    //
#if (_WIN32_IE >= 0x0500)
    virtual BOOL setUnicodeFormat(BOOL fUnicode);
#endif //  (_WIN32_IE >= 0x0500)
protected:
private:
};

#endif // U_UPDOWNCTRL_H

/*===================================================
 *   Up-Down Control Styles
 *===========================
 *   UDS_ALIGNLEFT
 *   UDS_ALIGNRIGHT
 *   UDS_ARROWKEYS
 *   UDS_AUTOBUDDY
 *   UDS_HORZ
 *   UDS_HOTTRACK
 *   UDS_NOTHOUSANDS
 *   UDS_SETBUDDYINT
 *   UDS_WRAP
 *====================================================*/

