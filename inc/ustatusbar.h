#ifndef U_STATUSBAR_H
#define U_STATUSBAR_H

#include "ucontrol.h"

class ULIB_API UStatusBar : public UControl
{
public:
    UStatusBar(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UStatusBar();
    //
    virtual BOOL create();
    //
    BOOL setText(int nPart, const TCHAR *lpText);
    //
    BOOL setIcon(int nPart, HICON hIcon);
    HICON setIcon(int nPart);
    //
    BOOL setParts(int nCount, int *aWidths);
    BOOL getParts(int nCount, int *aWidths);
    //
    BOOL getPartRect(int nPart, LPRECT lpRect);
    //
    BOOL setMinHeight(int nHeight);
    //
    BOOL simplify(BOOL bSimple = FALSE);
    BOOL isSimple();
    //
    BOOL resize();
protected:
private:
};

#endif // U_STATUSBAR_H

