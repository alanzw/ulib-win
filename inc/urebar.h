#ifndef U_REBAR_H
#define U_REBAR_H

#include "ucontrol.h"
#include "colors.h"

class ULIB_API URebar : public UControl
{
public:
    URebar(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~URebar();
    //
    BOOL create();

    //
    BOOL initBandInfo();
    //
    BOOL addChild(HWND hChild, LPTSTR lpText, UINT cxMinChild, UINT cyMinChild, UINT cx);

	//
	huys::Color getBkColor();
	huys::Color getTextColor();
	huys::Color setBkColor(huys::Color clr);
	huys::Color setTextColor(huys::Color clr);
protected:
private:
    REBARBANDINFO m_bandinfo;
};

#endif // U_REBAR_H

