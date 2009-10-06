#ifndef U_HEADER_H
#define U_HEADER_H

#include "ucontrol.h"

class ULIB_API UHeaderCtrl : public UControl
{
public:
    UHeaderCtrl(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UHeaderCtrl();
    virtual BOOL create();

    BOOL layout();
    int insertItem(int iInsertAfter, int nWidth, LPSTR lpsz);
protected:
private:
};

#endif // U_HEADER_H
