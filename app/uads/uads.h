#ifndef U_ADS_H
#define U_ADS_H

#include "ustatic.h"

class UADSample : public UStatic
{
public:
    UADSample(HWND hParent, UINT nID, HINSTANCE hInst);
    UADSample(int port);
    UADSample(UBaseWindow *pWndParent, UINT nID = -1);
    ~UADSample();

    void setPort(int port);
private:
    int _nPort;
};

#endif // U_ADS_H
