#ifndef U_IPADDRESS_H
#define  U_IPADDRESS_H

#include "ucontrol.h"

class ULIB_API UIPAddress : public UControl
{
public:
    UIPAddress(HWND hParent, UINT nID, HINSTANCE hInst);
    ~UIPAddress();

    virtual BOOL create();

    //
    BOOL setAddress(BYTE first, BYTE second, BYTE third, BYTE fourth);
    //
    BOOL isBlank();
    //
    void setFocus(int nField);
    BOOL setRange(int nField, BYTE low, BYTE high);

    //
    DWORD getAddress();
    //
    BYTE first();
    BYTE second();
    BYTE third();
    BYTE fourth();

    //
    void clear();
protected:
private:
};

#endif // U_IPADDRESS_H
