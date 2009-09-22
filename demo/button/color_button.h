/*
 * =====================================================================================
 *
 *       Filename:  color_button.h
 *
 *    Description:  Colorful Buttons
 *
 *        Version:  1.0
 *        Created:  2009-7-18 11:53:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_COLOR_BUTTON_H
#define U_COLOR_BUTTON_H

#include "ubutton.h"
#include "colors.h"

class UColorButton : public UOwnerDrawnButton
{
public:
    typedef enum tagButtonStatus {
        BNST_IN = 0x01,
        BNST_OUT = 0x02,
        BNST_BLACK_BORDER = 0x04
    } ButtonStatus;
public:
    UColorButton(HWND hParent, UINT nResource, HINSTANCE hInst);

    ~UColorButton();

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam);

    //
    void setColorWindowsDefault();
    void setColor(huys::Color clrText, huys::Color clrBk);

    //
protected:
private:
    //
    void drawFillRect(HDC hdc, RECT *rc, huys::Color clr);
    void drawFrame(HDC hdc, RECT *lprc, UINT bs);
    void drawButtonText(HDC hdc, RECT *lprc, const TCHAR *lpText, huys::Color clrText);
    //
    huys::Color m_clrText;
    huys::Color m_clrBkgnd;
    huys::Color m_clrDisableBk;
    huys::Color m_clrLight;
    huys::Color m_clrHighlight;
    huys::Color m_clrShadow;
    huys::Color m_clrDarkShadow;
};

#endif // U_COLOR_BUTTON_H

