#ifndef U_MAC_PROCESSBAR_H
#define U_MAC_PROCESSBAR_H

#define IDT_INDETERMINATE       100
#define IND_BAND_WIDTH          20

class UMacProgressBar : public UProgressBar
{
public:
    UMacProgressBar(HWND hParent, UINT nID, HINSTANCE hInst)
        : UProgressBar(hParent, nID, hInst)
    {
        m_bIndeterminate = FALSE;
        m_nIndOffset = 0;
    }

    BOOL getIndeterminate()
    { return m_bIndeterminate;}

    void setIndeterminate(BOOL bIndeterminate = TRUE)
    { m_bIndeterminate=bIndeterminate;}

    virtual BOOL create()
    {
        return UProgressBar::create() && this->subclassProc();
    }

    virtual BOOL onPaint()
    {
        UPaintDC dc(m_hSelf);

        huys::URectL rectCtrl;
        huys::URectL rectClient;

        this->getWindowRect(rectCtrl);
        this->getClientRect(rectClient);

        huys::URectL rect(1, 1, rectCtrl.width()+1, rectCtrl.height()+1);

        dc.setBrushColor(huys::pink);
        dc.rectangle(rect);

        int rangeLow = this->getLowLimit();
        int rangeHigh = this->getHighLimit();

        int x = getPBPos();
        int y;

        if(x < rangeLow || x > rangeHigh)
            return FALSE;

        // Calculate the width

        x -= rangeLow;
        x *= 100;
        x /= (rangeHigh - rangeLow);

        y = rect.width();
        y *= x;
        y /= 100;
        y += rect.left();

        huys::URectL rectBar(rect.left(), rect.top(), rect.left()+y, rect.bottom());

        //dc.setBrushColor(::GetSysColor(COLOR_3DFACE));
        //dc.rectangle(rectBar);

        dc.setBrushColor(m_crColorLightest);
        if (m_bIndeterminate)
        {
            dc.rectangle(rect);
        }
        else
        {
            dc.rectangle(rectBar);
        }
        DrawHorizontalBar(dc, rectBar);

        return FALSE;
    }

    void setColors(huys::Color clr)
    {
        m_crColor = clr;
        m_crColorLight = LightenColor(m_crColor, 51);
        m_crColorLighter = LightenColor(m_crColorLight, 51);
        m_crColorLightest = LightenColor(m_crColorLighter, 51);
        m_crColorDark = DarkenColor(m_crColor, 51);
        m_crColorDarker = DarkenColor(m_crColorDark, 51);
        m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
        m_crLiteShadow = ::GetSysColor(COLOR_3DSHADOW);

        // Get a color halfway between COLOR_3DDKSHADOW and COLOR_3DSHADOW
        BYTE byRed3DDkShadow = GetRValue(m_crDkShadow);
        BYTE byRed3DLiteShadow = GetRValue(m_crLiteShadow);
        BYTE byGreen3DDkShadow = GetGValue(m_crDkShadow);
        BYTE byGreen3DLiteShadow = GetGValue(m_crLiteShadow);
        BYTE byBlue3DDkShadow = GetBValue(m_crDkShadow);
        BYTE byBlue3DLiteShadow = GetBValue(m_crLiteShadow);

        m_crShadow = RGB(byRed3DLiteShadow + ((byRed3DDkShadow - byRed3DLiteShadow) >> 1),
            byGreen3DLiteShadow + ((byGreen3DDkShadow - byGreen3DLiteShadow) >> 1),
            byBlue3DLiteShadow + ((byBlue3DDkShadow - byBlue3DLiteShadow) >> 1));

    }

    virtual BOOL onEraseBkgnd(HDC hdc) {return TRUE;}
private:
    int m_nIndOffset;
    BOOL m_bIndeterminate;

    huys::Color m_crColor;
    huys::Color m_crColorLight;
    huys::Color m_crColorLighter;
    huys::Color m_crColorLightest;
    huys::Color m_crColorDark;
    huys::Color m_crColorDarker;
    huys::Color m_crDkShadow;
    huys::Color m_crShadow;
    huys::Color m_crLiteShadow;
private:
    //-------------------------------------------------------------------
    //
    huys::Color LightenColor(const huys::Color crColor, BYTE byIncreaseVal)
        //
        // Return Value:    lightened color in RGB.
        //
        // Parameters    :  crColor - References a huys::Color structure.
        //                  byIncreaseVal - The amount to increase the RGB values by.
        //
        // Remarks        : Lightens a color by increasing the RGB values by the given number.
        //
    {
        BYTE byRed = GetRValue(crColor);
        BYTE byGreen = GetGValue(crColor);
        BYTE byBlue = GetBValue(crColor);

        if ((byRed + byIncreaseVal) <= 255)
            byRed = BYTE(byRed + byIncreaseVal);
        if ((byGreen + byIncreaseVal)    <= 255)
            byGreen = BYTE(byGreen + byIncreaseVal);
        if ((byBlue + byIncreaseVal) <= 255)
            byBlue = BYTE(byBlue + byIncreaseVal);

        return RGB(byRed, byGreen, byBlue);
    }

    //-------------------------------------------------------------------
    //
    huys::Color DarkenColor(const huys::Color crColor, BYTE byReduceVal)
        //
        // Return Value:    darkened color in RGB.
        //
        // Parameters  :    crColor - References a huys::Color structure.
        //                  byReduceVal - The amount to reduce the RGB values by.
        //
        // Remarks     :    Darkens a color by reducing the RGB values by the given number.
        //
    {
        BYTE byRed = GetRValue(crColor);
        BYTE byGreen = GetGValue(crColor);
        BYTE byBlue = GetBValue(crColor);

        if (byRed >= byReduceVal)
            byRed = BYTE(byRed - byReduceVal);
        if (byGreen >= byReduceVal)
            byGreen = BYTE(byGreen - byReduceVal);
        if (byBlue >= byReduceVal)
            byBlue = BYTE(byBlue - byReduceVal);

        return RGB(byRed, byGreen, byBlue);
    }

    void DrawHorizontalBar(HDC hdc, LPRECT lpRect)
    {
        int nLeft = lpRect->left;
        int nTop = lpRect->top;
        int nBottom = lpRect->bottom;

        huys::URectL rect(lpRect);

        USmartDC dc(hdc);

        dc.setPenColor(m_crColorLight);

        if (m_bIndeterminate)
        {
            dc.setPenColor(m_crColor);
            int nNumBands = (rect.width() / IND_BAND_WIDTH) + 2;
            int nHeight = rect.height() + 1;

            int nAdjust = nLeft - IND_BAND_WIDTH + m_nIndOffset;
            int nXpos = 0;
            //int nYpos1 = nTop + 1;
            //int nYpos2 = nBottom - 2;

            for (int i = 0; i < nNumBands; i++)
            {
                nXpos = nAdjust + (i * IND_BAND_WIDTH);

                dc.setPenColor(m_crColorDarker);
                dc.moveTo(nXpos + 1, nTop);
                dc.lineTo(nXpos + nHeight, nBottom);

                dc.setPenColor(m_crColorDark);
                dc.moveTo(nXpos + 2, nTop);
                dc.lineTo(nXpos + nHeight + 1, nBottom);
                dc.moveTo(nXpos + 10, nTop);
                dc.lineTo(nXpos + nHeight + 9, nBottom);

                dc.setPenColor(m_crColor);
                dc.moveTo(nXpos + 3, nTop);
                dc.lineTo(nXpos + nHeight + 2, nBottom);
                dc.moveTo(nXpos + 9, nTop);
                dc.lineTo(nXpos + nHeight + 8, nBottom);

                dc.setPenColor(m_crColorLight);
                dc.moveTo(nXpos + 4, nTop);
                dc.lineTo(nXpos + nHeight + 3, nBottom);
                dc.moveTo(nXpos + 8, nTop);
                dc.lineTo(nXpos + nHeight + 7, nBottom);

                dc.setPenColor(m_crColorLighter);
                dc.moveTo(nXpos + 5, nTop);
                dc.lineTo(nXpos + nHeight + 4, nBottom);
                dc.moveTo(nXpos + 7, nTop);
                dc.lineTo(nXpos + nHeight + 6, nBottom);
            }   // for the number of bands
        }   // if indeterminate
        else
        {
            int nRight = rect.right();

            dc.moveTo(nLeft + 2, nBottom - 4);
            dc.lineTo(nRight - 2, nBottom - 4);
            dc.moveTo(nLeft + 2, nTop + 2);
            dc.lineTo(nRight - 2, nTop + 2);
            dc.setPixel(nLeft + 1, nBottom - 3, m_crColorLight);
            dc.setPixel(nLeft + 1, nTop + 1, m_crColorLight);

            dc.setPenColor(m_crColorLighter);
            dc.moveTo(nLeft + 2, nBottom - 5);
            dc.lineTo(nRight - 3, nBottom - 5);
            dc.lineTo(nRight - 3, nTop + 3);
            dc.lineTo(nLeft + 1, nTop + 3);
            dc.setPixel(nLeft + 1, nBottom - 4, m_crColorLighter);
            dc.setPixel(nLeft + 1, nTop + 2, m_crColorLighter);

            dc.setPenColor(m_crColor);
            dc.moveTo(nLeft, nBottom - 1);
            dc.lineTo(nLeft, nTop);
            dc.lineTo(nLeft + 2, nTop);
            dc.setPixel(nLeft + 1, nBottom - 2, m_crColor);
            dc.moveTo(nLeft + 2, nBottom - 3);
            dc.lineTo(nRight - 2, nBottom - 3);
            dc.moveTo(nLeft + 2, nTop + 1);
            dc.lineTo(nRight - 1, nTop + 1);

            dc.setPenColor(m_crColorDark);
            dc.moveTo(nLeft + 2, nBottom - 2);
            dc.lineTo(nRight - 2, nBottom - 2);
            dc.lineTo(nRight - 2, nTop + 1);
            dc.moveTo(nLeft + 2, nTop);
            dc.lineTo(nRight, nTop);
            dc.setPixel(nLeft + 1, nBottom - 1, m_crColorDark);

            dc.setPenColor(m_crColorDarker);
            dc.moveTo(nLeft + 2, nBottom - 1);
            dc.lineTo(nRight - 1, nBottom - 1);
            dc.lineTo(nRight - 1, nTop);

            dc.setPenColor(m_crShadow);
            dc.moveTo(nRight, nTop);
            dc.lineTo(nRight, nBottom);

            dc.setPenColor(m_crLiteShadow);
            dc.moveTo(nRight + 1, nTop);
            dc.lineTo(nRight + 1, nBottom);
        }   // if not indeterminate

    }

    void DrawVerticalBar(HDC hdc, LPRECT lpRect)
    {

        huys::URectL rect(lpRect);

        USmartDC dc(hdc);

		int nHeight = rect.height();

		int nLeft = rect.left();
		int nTop = rect.top();
		int nRight = rect.right();
		int nBottom = rect.bottom();

		dc.setPenColor(m_crColor);

		if (m_bIndeterminate)
		{
			int nNumBands = (nHeight / IND_BAND_WIDTH) + 2;
			int nHeight = rect.width() + 1;

			int nAdjust = nBottom - m_nIndOffset;
			int nXpos1 = nLeft;
			int nXpos2 = nRight + 1;
			int nYpos = nTop + 1;

			for (int i = 0; i < nNumBands; i++)
			{
				nYpos = nAdjust - (i * IND_BAND_WIDTH);

				dc.setPenColor(m_crColorDarker);
				dc.moveTo(nXpos1, nYpos);
				dc.lineTo(nXpos2, nYpos + nHeight);

				dc.setPenColor(m_crColorDark);
				dc.moveTo(nXpos1, nYpos + 1);
				dc.lineTo(nXpos2, nYpos + nHeight + 1);
				dc.moveTo(nXpos1, nYpos + 9);
				dc.lineTo(nXpos2, nYpos + nHeight + 9);

				dc.setPenColor(m_crColor);
				dc.moveTo(nXpos1, nYpos + 2);
				dc.lineTo(nXpos2, nYpos + nHeight + 2);
				dc.moveTo(nXpos1, nYpos + 8);
				dc.lineTo(nXpos2, nYpos + nHeight + 8);

				dc.setPenColor(m_crColorLight);
				dc.moveTo(nXpos1, nYpos + 3);
				dc.lineTo(nXpos2, nYpos + nHeight + 3);
				dc.moveTo(nXpos1, nYpos + 7);
				dc.lineTo(nXpos2, nYpos + nHeight + 7);

				dc.setPenColor(m_crColorLighter);
				dc.moveTo(nXpos1, nYpos + 4);
				dc.lineTo(nXpos2, nYpos + nHeight + 4);
				dc.moveTo(nXpos1, nYpos + 6);
				dc.lineTo(nXpos2, nYpos + nHeight + 6);
			}   // for the number of bands
		}   // if indeterminate
		else
		{
			if (nHeight > 3)
			{
				dc.moveTo(nLeft, nTop + 1);
				dc.lineTo(nLeft, nTop);
				dc.lineTo(nRight, nTop);
				dc.moveTo(nLeft + 1, nBottom - 2);
				dc.lineTo(nLeft + 1, nTop + 1);
				dc.moveTo(nRight - 3, nBottom - 3);
				dc.lineTo(nRight - 3, nTop + 1);
				dc.setPixel(nRight - 2, nTop + 1, m_crColor);

				dc.setPenColor(m_crColorLight);
				dc.moveTo(nLeft + 2, nBottom - 3);
				dc.lineTo(nLeft + 2, nTop + 1);
				dc.moveTo(nRight - 4, nBottom - 3);
				dc.lineTo(nRight - 4, nTop + 1);
				dc.setPixel(nLeft + 1, nTop + 1, m_crColorLight);
				dc.setPixel(nRight - 3, nTop + 1, m_crColorLight);

				dc.setPenColor(m_crColorLighter);
				dc.moveTo(nLeft + 3, nBottom - 3);
				dc.lineTo(nLeft + 3, nTop + 1);
				dc.moveTo(nRight - 5, nBottom - 3);
				dc.lineTo(nRight - 5, nTop + 1);
				dc.setPixel(nLeft + 2, nTop + 1, m_crColorLighter);
				dc.setPixel(nRight - 4, nTop + 1, m_crColorLighter);

				dc.setPenColor(m_crColorDark);
				dc.moveTo(nLeft, nBottom - 1);
				dc.lineTo(nLeft, nTop + 1);
				dc.moveTo(nLeft + 2, nBottom - 2);
				dc.lineTo(nRight - 2, nBottom - 2);
				dc.lineTo(nRight - 2, nTop + 1);
				dc.setPixel(nRight - 1, nTop + 1, m_crColorDark);

				dc.setPenColor(m_crColorDarker);
				dc.moveTo(nLeft + 1, nBottom - 1);
				dc.lineTo(nRight - 1, nBottom - 1);
				dc.lineTo(nRight - 1, nTop + 1);
			}
			else
			{
				dc.setPenColor(m_crColorDark);
				dc.rectangle(rect);
				dc.rectangle(rect);
			}
		}   // if not indeterminate

    }
};

#endif // U_MAC_PROCESSBAR_H
