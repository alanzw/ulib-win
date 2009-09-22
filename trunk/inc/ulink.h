#ifndef U_LINK_H
#define U_LINK_H

#include "ucontrol.h"


class ULIB_API ULink : public UControl
{
public:
    ULink( HWND hParent, UINT nResource, HINSTANCE hInst);
    ~ULink();
    virtual BOOL create(const TCHAR *lpText);
protected:
private:
};

#endif // U_LINK_H
