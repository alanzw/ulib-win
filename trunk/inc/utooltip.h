/*
 * =====================================================================================
 *
 *       Filename:  utooltip.h
 *
 *    Description:  The Class for tooltip control
 *
 *        Version:  1.0
 *        Created:  2009-2-15 3:31:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys03@gmail.com
 *        Company:  none
 *
 * =====================================================================================
 */

#ifndef U_TOOLTIP_H
#define U_TOOLTIP_H

#include "ucontrol.h"
#include "colors.h"

class ULIB_API UToolTip : public UControl
{
public:
    enum {
        MAX_TITLE_LENGTH  = 512,
        MAX_TEXT_LENGTH   = 512,
        DEFAULT_MAX_WIDTH = 100
    };
    enum TipTextMode {
        TPTM_TEXT,
        TPTM_CALLBACK
    };
public:
    UToolTip(HWND hParent, UINT nID, HINSTANCE hInst);
    UToolTip() {}
    ~UToolTip();
    //
    virtual BOOL create();
    //
    virtual BOOL activate();
    virtual BOOL deactivate();
    //
    BOOL setBKColor(huys::Color color);

    BOOL setTextColor(huys::Color color);

    BOOL setTitle(const TCHAR *sTitle);

    BOOL setText(const TCHAR *sText);

    BOOL setMaxWidth(int nWidth);

    void setXY(WORD x, WORD y);
    //
    BOOL trackPosition();
    //
    BOOL showTip();
protected:
    TOOLINFO m_ti;
private:
    TCHAR m_sTitle[MAX_TITLE_LENGTH];
    huys::Color m_clrBk;
    huys::Color m_clrText;
    int m_nMaxWidth;

    WORD m_wX;
    WORD m_wY;

    TCHAR m_sText[MAX_TEXT_LENGTH];
private:
    //
    BOOL setTipTitle(const TCHAR *sTitle);
    BOOL setTipBkColor(huys::Color clr);
    BOOL setTipTextColor(huys::Color clr);
    BOOL setTipMaxWidth(int nWidth);
    //
    BOOL addTool();
};

#endif // U_TOOLTIP_H

