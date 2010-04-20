#ifndef U_PROGRESSBAR_H
#define U_PROGRESSBAR_H

#include "ucontrol.h"
#include "colors.h"

class ULIB_API UProgressBar : public UControl
{
public:
    UProgressBar(HWND hParent, UINT nResource, HINSTANCE hInst);
    ~UProgressBar();

    //
    virtual BOOL create();

    //
    virtual BOOL setRange(int min, int max);
    virtual int getLowLimit();
    virtual int getHighLimit();
    //
    virtual BOOL setStep(int nStepSize);
    virtual BOOL stepIt();
    virtual int getStep();

    //
    virtual BOOL setPBPos(int nNewPos);
    virtual BOOL deltaPBPos(int nIncrement);
    virtual UINT getPBPos();

#if (_WIN32_WINNT >= 0x0501)
    virtual BOOL setMarquee(BOOL bMarqueeModeOn, UINT nUpdateTimeInMs);
#endif // _WIN32_WINNT >= 0x0501

    virtual BOOL setState(int iState);
    virtual int getState();

    virtual BOOL setBKColor(huys::Color clr);
    virtual BOOL setBarColor(huys::Color clr);
    virtual huys::Color getBKColor();
    virtual huys::Color getBarColor();
protected:
private:
};

#if (_WIN32_WINNT >= 0x0501)
#define PBS_MARQUEE             0x08
#define PBM_SETMARQUEE          (WM_USER+10)
#endif      // _WIN32_WINNT >= 0x0501

#endif // U_PROGRESSBAR_H

