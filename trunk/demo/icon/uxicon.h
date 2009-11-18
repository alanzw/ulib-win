/*
 * =====================================================================================
 *
 *       Filename:  uxicon.h
 *
 *    Description:  Class UXIcon
 *
 *        Version:  1.0
 *        Created:  2009-11-19 0:07:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_XICON_H
#define U_XICON_H

#include "ubutton.h"
#include "ufont.h"
#include "colors.h"
#include "uicon.h"

class UXIcon : public UOwnerDrawnButton
{
public:
    UXIcon(HWND hParent, UINT nResource, HINSTANCE hInst);
    ~UXIcon();

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam);

    void setIcon(UINT nID);
    
    void setTextColor(huys::Color clr)
    { m_clrText = clr; }
private:
    huys::Color m_clrText;
    UIcon m_ico;
    HFONT m_hFont;

    int m_nXMargin;
    int m_nYMargin;

    int m_nIconSpacing;
    int m_nIconSize;

    int DrawIcon(HDC hdc, RECT &rect);
    int DrawText(HDC hdc, RECT &rect);
};


#endif // U_XICON_H

