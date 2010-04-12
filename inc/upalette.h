#ifndef U_PALETTE_H
#define U_PALETTE_H

#include "uobject.h"

class ULIB_API UPalette : public UGDIObject
{
public:
    UPalette();
    ~UPalette();

	//
	BOOL createPalette(const LPLOGPALETTE lpLogPalette);

	//
    UINT realizePalette(HDC hdc);

    //
    HPALETTE selectPalette(HDC hdc, BOOL bForceBackground); 
    HPALETTE restorePalette(HDC hdc, BOOL bForceBackground);
	//
	BOOL animatePalette( UINT nStartIndex,
						 UINT nNumEntries,
		                 LPPALETTEENTRY lpPaletteColors ); 
};

#endif // U_PALETTE_H
