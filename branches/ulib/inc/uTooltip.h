#ifndef HU_TOOLTIP_H
#define HU_TOOLTIP_H

//#ifdef _WIN32_IE
//#undef _WIN32_IE
//#endif
//#define _WIN32_IE 0x050


// namespace huys
#include "colors.h"

/********************************************************************
	created:	2008/09/29
	created:	29:9:2008   4:05
	filename: 	uTooltip.h
	author:		hu
	
	purpose:	A customized tooltip.
*********************************************************************/
// TODO: Add methods of changing the drawing font.
// TODO: Get rid of CString.
// TODO: Add Hit action.

class CMyTooltip
{
public:

	CMyTooltip();

	virtual ~CMyTooltip(); 

	BOOL create(HWND hParent = NULL);

	BOOL show();

	BOOL setXY(int x, int y);

	BOOL setText(const TCHAR *szText);

	BOOL setMaxWidth(int width);

	BOOL setBKColor(huys::Color color);

    BOOL setTextColor(huys::Color color);

    BOOL setTitle(const TCHAR *szTitle);

    UINT getUID() const;

    LPSTR getText();

    BOOL isVisible();

    HWND getParent();

	CString m_strText;

private:
	BOOL m_bShow;
	int m_x;
	int m_y;

    CString m_strTitle;

	UINT m_uID;

	int m_nWidth;

	TOOLINFO m_ti;

	huys::Color m_clrBackground;
    huys::Color m_clrText;

	HWND m_hTT;
	HWND m_hParent;
};


#endif // HU_TOOLTIP_H
