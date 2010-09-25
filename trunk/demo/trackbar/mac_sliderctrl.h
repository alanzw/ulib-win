#ifndef U_MAC_SLIDERCTRL_H
#define U_MAC_SLIDERCTRL_H

class UMacSliderCtrl : public UTrackBar
{
public:
    UMacSliderCtrl(HWND hParent, UINT nID, HINSTANCE hInst)
    : UTrackBar(hParent, nID, hInst)
    {
        m_crThumb = ::GetSysColor(COLOR_3DFACE);
        m_crChannel = ::GetSysColor(COLOR_WINDOW);
        m_crSelection = ::GetSysColor(COLOR_HIGHLIGHT);
    }
    
    virtual BOOL create()
    {
        return UTrackBar::create() && this->subclassProc();
    }

    BOOL doCustomDraw(LPNMCUSTOMDRAW lpcd)
    {
        //LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)lParam;

        // CDDS_PREPAINT is at the beginning of the paint cycle. You 
        // implement custom draw by returning the proper value. In this 
        // case, we're requesting item-specific notifications.
        if (lpcd->dwDrawStage == CDDS_PREPAINT)
        {
            // Request prepaint notifications for each item.
            return CDRF_NOTIFYITEMDRAW;
        }
        
        // Because we returned CDRF_NOTIFYITEMDRAW in response to
        // CDDS_PREPAINT, CDDS_ITEMPREPAINT is sent when the control is
        // about to paint an item.
        if (lpcd->dwDrawStage == CDDS_ITEMPREPAINT)
        {	
            USmartDC dc(lpcd->hdc);
            huys::URectL rect(&lpcd->rc);
            int nSavedDC = dc.saveDC();

            if (lpcd->dwItemSpec == TBCD_TICS)
            {
                return CDRF_DODEFAULT;
            }	// if drawing tics
            else if (lpcd->dwItemSpec ==  TBCD_THUMB)
            {
                // If the slider has been clicked on (selected) darken
                // the thumb. Save the thumb color, in case it is changed.
                COLORREF crSavedThumb = m_crThumb;

                if (lpcd->uItemState & CDIS_SELECTED)
                {
                    m_crThumb = DarkenColor(m_crThumb, 51);
                    GetColors();
                    CreatePens();
                }

                if (rect.height() > rect.width())
                DrawHorizontalThumb(dc, rect);
                else
                DrawVerticalThumb(dc, rect);

                // Restore the thumb color.
                if (lpcd->uItemState & CDIS_SELECTED)
                {
                    m_crThumb = crSavedThumb;
                    GetColors();
                    CreatePens();
                }
            }	// if drawing thumb
            else if (lpcd->dwItemSpec == TBCD_CHANNEL)
            {
                // If the slider is disabled, change the thumb
                // to the current face color.
                COLORREF crSavedChannel = m_crChannel;
                if (!IsWindowEnabled(*this))
                {
                    m_crChannel = ::GetSysColor(COLOR_3DFACE);
                }

                huys::URectL rcThumb;
                this->getThumbRect(rcThumb);
                DrawChannel(dc, rect, rcThumb.height() > rcThumb.width());

                m_crChannel = crSavedChannel;
            }	// if drawing channel

            dc.restoreDC(nSavedDC);

            return CDRF_SKIPDEFAULT;
        }
        return FALSE;
    }
    
    BOOL onNotifyReflect(WPARAM wParam, LPARAM lParam)
    {
        //LPNMHDR pNMHDR = (LPNMHDR)lParam;
        if ((UINT)NM_CUSTOMDRAW == ((LPNMHDR)lParam)->code)
        {
            return doCustomDraw((LPNMCUSTOMDRAW) lParam);
        }
        return UTrackBar::onNotifyReflect(wParam, lParam);
    }
public:
    void setChannelColor(huys::Color crColor)
    {
        m_crChannel = crColor;
    }
    
    huys::Color getChannelColor()
    {
        return m_crChannel;
    }
    
    void setThumbColor(huys::Color crColor)
    {
        m_crThumb = crColor;
    }
    
    huys::Color getThumbColor()
    {
        return m_crThumb;
    }
    
    void setSelectionColor(huys::Color crColor)
    {
        m_crSelection = crColor;
    }
    
    huys::Color getSelectionColor()
    {
        return m_crSelection;
    }
private:
    //-------------------------------------------------------------------
    //
    void GetColors()
    //
    // Return Value:	None.
    //
    // Parameters	:	None.
    //
    // Remarks		:	Calculates the lighter and darker colors, as well as 
    //						the shadow colors.
    //
    {
        m_crThumbLight = LightenColor(m_crThumb, 51);
        m_crThumbLighter = LightenColor(m_crThumbLight, 51);
        m_crThumbDark = DarkenColor(m_crThumb, 51);
        m_crThumbDarker = DarkenColor(m_crThumbDark, 51);

        m_crHilight = ::GetSysColor(COLOR_3DHILIGHT);

        // Get a color halfway between COLOR_3DSHADOW AND COLOR_3DFACE,
        // and COLOR_3DDKSHADOW and COLOR_3DSHADOW.
        COLORREF crFace = ::GetSysColor(COLOR_3DFACE);
        COLORREF crShadow = ::GetSysColor(COLOR_3DSHADOW);
        COLORREF crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
        BYTE byRedFace = GetRValue(crFace);
        BYTE byRedShadow = GetRValue(crShadow);
        BYTE byRedDkShadow = GetRValue(crDkShadow);
        BYTE byGreenFace = GetGValue(crFace);
        BYTE byGreenShadow = GetGValue(crShadow);
        BYTE byGreenDkShadow = GetGValue(crDkShadow);
        BYTE byBlueFace = GetBValue(crFace);
        BYTE byBlueShadow = GetBValue(crShadow);
        BYTE byBlueDkShadow= GetBValue(crDkShadow);

        m_crShadow = RGB(byRedFace + ((byRedShadow - byRedFace) >> 1),
        byGreenFace + ((byGreenShadow - byGreenFace) >> 1),
        byBlueFace + ((byBlueShadow - byBlueFace) >> 1));

        m_crDkShadow = RGB(byRedShadow + ((byRedDkShadow - byRedShadow) >> 1),
        byGreenShadow + ((byGreenDkShadow - byGreenShadow) >> 1),
        byBlueShadow + ((byBlueDkShadow - byBlueShadow) >> 1));
    }	// GetColors

    //-------------------------------------------------------------------
    //
    void CreatePens()
    //
    // Return Value:	None.
    //
    // Parameters	:	None.
    //
    // Remarks		:	Deletes the pen objects, if necessary, and creates them.
    //
    {
        //DeletePens();

        m_penThumbLight.createPen(PS_SOLID, 1, m_crThumbLight);
        m_penThumbLighter.createPen(PS_SOLID, 1, m_crThumbLighter);
        m_penThumb.createPen(PS_SOLID, 1, m_crThumb);
        m_penThumbDark.createPen(PS_SOLID, 1, m_crThumbDark);
        m_penThumbDarker.createPen(PS_SOLID, 1, m_crThumbDarker);
        m_penDkShadow.createPen(PS_SOLID, 1, m_crDkShadow);
        m_penShadow.createPen(PS_SOLID, 1, m_crShadow);
        m_penHilight.createPen(PS_SOLID, 1, m_crHilight);
    }	// CreatePens

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

    void DrawHorizontalThumb(HDC hdc, LPRECT lpRect)
    {
        USmartDC dc(hdc);

        huys::URectL rect(lpRect);
        int nWidth = rect.width();
        int nMid = nWidth >> 1;

        UPen penFrame;
        penFrame.createSolidPen(::GetSysColor(COLOR_WINDOWFRAME));

        HPEN hOldPen = (HPEN)dc.selectObj(penFrame);	

        UBrush br;
        br.createSolidBrush(m_crThumb);
        HBRUSH hOldBrush = (HBRUSH)dc.selectObj(br);

        DWORD dwStyle = m_dwStyles;

        // No points on thumb.
        if (dwStyle & TBS_BOTH)
        {
            dc.roundRect(rect, huys::UPointL(2, 2));
            rect.deflate(1, 1, 1, 1);
            dc.draw3dRect(rect, m_crThumbLight, m_crThumbDark);

            dc.setPixel(rect.left(), rect.top(), m_crThumbLighter);
            dc.setPixel(rect.right() - 1, rect.top(), m_crThumb);
            dc.setPixel(rect.right() - 1, rect.bottom() - 1, m_crThumbDarker);
            dc.setPixel(rect.left(), rect.bottom() - 1, m_crThumb);
        }
        // Point on top.
        else if (dwStyle & TBS_TOP)
        {
            dc.moveTo(rect.left(), rect.top() + nMid - 1);
            dc.lineTo(rect.left(), rect.bottom() - 2);
            dc.lineTo(rect.left() + 1, rect.bottom() - 1);
            dc.lineTo(rect.right() - 2, rect.bottom() - 1);
            dc.lineTo(rect.right() - 1, rect.bottom() - 2);
            dc.lineTo(rect.right() - 1, rect.top() + nMid - 1);
            dc.lineTo(rect.right() - nMid, rect.top());
            dc.lineTo(rect.left() + nMid - 1, rect.top());
            dc.lineTo(rect.left(), rect.top() + nMid - 1);

            dc.floodFill(rect.left() + nMid, rect.top() + nMid, ::GetSysColor(COLOR_WINDOWFRAME));

            if (nWidth > 5)
            {
                dc.selectObj(m_penThumbLight);
                dc.moveTo(rect.left() + 1, rect.top() + nMid);
                dc.lineTo(rect.left() + 1, rect.bottom() - 2);
                dc.moveTo(rect.right() - nMid, rect.top() + 1);
                dc.lineTo(rect.right() - nMid - 2, rect.top() + 1);

                dc.selectObj(m_penThumbLighter);
                dc.moveTo(rect.left() + 1, rect.top() + nMid - 1);
                dc.lineTo(rect.left() + nMid, rect.top());


                dc.selectObj(m_penThumbDark);
                dc.moveTo(rect.left() + 2, rect.bottom() - 2);
                dc.lineTo(rect.right() - 2, rect.bottom() - 2);
                dc.lineTo(rect.right() - 2, rect.bottom() - 2);
                dc.lineTo(rect.right() - 2, rect.top() + nMid - 1);
                dc.lineTo(rect.right() - nMid, rect.top() + 1);
            }

            rect.deflate(1, 2, 1, 0);
        }	// if point at top of thumb
        // Point is on bottom.
        else
        {
            dc.moveTo(rect.left(), rect.top() + 1);
            dc.lineTo(rect.left(), rect.bottom() - nMid);
            dc.lineTo(rect.left() + nMid - 1, rect.bottom() - 1);
            dc.lineTo(rect.right() - nMid, rect.bottom() - 1);
            dc.lineTo(rect.right() - 1, rect.bottom() - nMid);
            dc.lineTo(rect.right() - 1, rect.top() + 1);
            dc.lineTo(rect.right() - 2, rect.top());
            dc.lineTo(rect.left(), rect.top());

            dc.floodFill(rect.left() + nMid, rect.top() + nMid, ::GetSysColor(COLOR_WINDOWFRAME));

            if (nWidth > 5)
            {
                dc.setPixel(rect.left() + 1, rect.top() + 1, m_crThumbLighter);

                dc.selectObj(m_penThumbLight);
                dc.moveTo(rect.left() + 1, rect.top() + 2);
                dc.lineTo(rect.left() + 1, rect.bottom() - nMid + 1);
                dc.moveTo(rect.left() + 2, rect.top() + 1);
                dc.lineTo(rect.right() - 2, rect.top() + 1);

                dc.selectObj(m_penThumbDark);
                dc.moveTo(rect.left() + 2, rect.bottom() - nMid + 1);
                dc.lineTo(rect.left() + nMid - 1, rect.bottom() - 2);
                dc.lineTo(rect.right() - nMid, rect.bottom() - 2);
                dc.moveTo(rect.right() - 2, rect.bottom() - nMid - 1);
                dc.lineTo(rect.right() - 2, rect.top() + 1);

                dc.selectObj(m_penThumbDarker);
                dc.moveTo(rect.right() - nMid, rect.bottom() - 2);
                dc.lineTo(rect.right() - 1, rect.bottom() - nMid - 1);
            }

            rect.deflate(1, 0, 1, 2);
        }	// if point at bottom of thumb

        // Only draw the gripper if the slider is enabled.
        if (::IsWindowEnabled(m_hSelf))
        {
            if (nWidth > 5)
            {
                dc.selectObj(m_penThumbLight);
                dc.moveTo(rect.left() + 1, rect.top() + 5);
                dc.lineTo(rect.left() + 1, rect.bottom() - 5);
                dc.moveTo(rect.left() + 3, rect.top() + 5);
                dc.lineTo(rect.left() + 3, rect.bottom() - 5);
                if (nWidth > 9)
                {
                    dc.moveTo(rect.left() + 5, rect.top() + 5);
                    dc.lineTo(rect.left() + 5, rect.bottom() - 5);
                    dc.moveTo(rect.left() + 7, rect.top() + 5);
                    dc.lineTo(rect.left() + 7, rect.bottom() - 5);
                }
            }

            if (nWidth > 5)
            {
                dc.selectObj(m_penThumbDarker);
                dc.moveTo(rect.left() + 2, rect.top() + 5);
                dc.lineTo(rect.left() + 2, rect.bottom() - 4);
                dc.moveTo(rect.left() + 4, rect.top() + 5);
                dc.lineTo(rect.left() + 4, rect.bottom() - 4);
                if (nWidth > 9)
                {
                    dc.moveTo(rect.left() + 6, rect.top() + 5);
                    dc.lineTo(rect.left() + 6, rect.bottom() - 4);
                }
            }

            if (nWidth > 5)
            {
                dc.setPixel(rect.left() + 1, rect.top() + 4, m_crThumbLighter);
                dc.setPixel(rect.left() + 3, rect.top() + 4, m_crThumbLighter);
                if (nWidth > 9)
                {
                    dc.setPixel(rect.left() + 5, rect.top() + 4, m_crThumbLighter);
                    dc.setPixel(rect.left() + 7, rect.top() + 4, m_crThumbLighter);
                }
            }
        }	// if (IsWindowEnabled())

        dc.selectObj(hOldPen);
        dc.selectObj(hOldBrush);
    }
    
    void DrawVerticalThumb(HDC hdc, LPRECT lpRect)
    {
        USmartDC dc(hdc);

        huys::URectL rect(lpRect);

        int nHeight = rect.height();
        int nMid = nHeight >> 1;

        UPen penFrame;
        penFrame.createSolidPen(::GetSysColor(COLOR_WINDOWFRAME));
        
        HPEN hOldPen = (HPEN)dc.selectObj(penFrame);	
        
        UBrush br;
        br.createSolidBrush(m_crThumb);
        HBRUSH hOldBrush = (HBRUSH)dc.selectObj(br);
        
        DWORD dwStyle = m_dwStyles;


        // No points on the thumb.
        if (dwStyle & TBS_BOTH)
        {
            dc.roundRect(rect, huys::UPointL(2, 2));
            rect.deflate(1, 1, 1, 1);
            dc.draw3dRect(rect, m_crThumbLight, m_crThumbDark);

            dc.setPixel(rect.left(), rect.top(), m_crThumbLighter);
            dc.setPixel(rect.right() - 1, rect.top(), m_crThumb);
            dc.setPixel(rect.right() - 1, rect.bottom() - 1, m_crThumbDarker);
            dc.setPixel(rect.left(), rect.bottom() - 1, m_crThumb);
        }
        // Point is on left side.
        else if (dwStyle & TBS_TOP)
        {
            dc.moveTo(rect.left() + nMid - 1, rect.top());
            dc.lineTo(rect.left(), rect.top() + nMid - 1);
            dc.lineTo(rect.left(), rect.bottom() - nMid);
            dc.lineTo(rect.left() + nMid - 1, rect.bottom() - 1);
            dc.lineTo(rect.right() - 2, rect.bottom() - 1);
            dc.lineTo(rect.right() - 1, rect.bottom() - 2);
            dc.lineTo(rect.right() - 1, rect.top() + 1);
            dc.lineTo(rect.right() - 2, rect.top());
            dc.lineTo(rect.left() + nMid - 2, rect.top());

            dc.floodFill(rect.left() + nMid, rect.top() + nMid, ::GetSysColor(COLOR_WINDOWFRAME));

            dc.selectObj(m_penThumbLighter);
            dc.moveTo(rect.left() + 1, rect.top() + nMid - 1);
            dc.lineTo(rect.left() + nMid, rect.top());

            dc.selectObj(m_penThumbLight);
            dc.moveTo(rect.left() + 1, rect.top() + nMid + 1);
            dc.lineTo(rect.left() + 1, rect.top() + nMid - 1);
            dc.moveTo(rect.left() + nMid, rect.top() + 1);
            dc.lineTo(rect.right() - 2, rect.top() + 1);

            dc.selectObj(m_penThumbDark);
            dc.moveTo(rect.left() + 2, rect.bottom() - nMid + 1);
            dc.lineTo(rect.left() + nMid - 1, rect.bottom() - 2);
            dc.lineTo(rect.right() - 2, rect.bottom() - 2);
            dc.lineTo(rect.right() - 2, rect.top() + 1);

            rect.deflate(2, 1, 0, 0);
        }	// if point on left side
        // Point is on right side.
        else
        {
            dc.moveTo(rect.left(), rect.top() + 1);
            dc.lineTo(rect.left(), rect.bottom() - 2);
            dc.lineTo(rect.left() + 1, rect.bottom() - 1);
            dc.lineTo(rect.right() - nMid, rect.bottom() - 1);
            dc.lineTo(rect.right() - 1, rect.bottom() - nMid);
            dc.lineTo(rect.right() - 1, rect.top() + nMid - 1);
            dc.lineTo(rect.right() - nMid, rect.top());
            dc.lineTo(rect.left(), rect.top());

            dc.floodFill(rect.left() + nMid, rect.top() + nMid, ::GetSysColor(COLOR_WINDOWFRAME));

            dc.selectObj(m_penThumbLight);
            dc.moveTo(rect.left() + 1, rect.bottom() - 2);
            dc.lineTo(rect.left() + 1, rect.top() + 1);
            dc.lineTo(rect.right() - nMid + 1, rect.top() + 1);

            dc.setPixel(rect.left() + 1, rect.top() + 1, m_crThumbLighter);

            dc.selectObj(m_penThumbDark);
            dc.moveTo(rect.left() + 2, rect.bottom() - 2);
            dc.lineTo(rect.right() - nMid, rect.bottom() - 2);
            dc.moveTo(rect.right() - 2, rect.top() + nMid);
            dc.lineTo(rect.right() - 2, rect.top() + nMid - 1);
            dc.lineTo(rect.right() - nMid, rect.top() + 1);

            dc.selectObj(m_penThumbDarker);
            dc.moveTo(rect.right() - nMid, rect.bottom() - 2);
            dc.lineTo(rect.right() - 1, rect.bottom() - nMid - 1);

            rect.deflate(0, 1, 1, 0);
        }	// if point is on right side

        // Only draw the gripper if the slider is enabled.
        if (::IsWindowEnabled(m_hSelf))
        {
            if (nHeight > 5)
            {
                dc.selectObj(m_penThumbLight);
                dc.moveTo(rect.left() + 5, rect.top() + 1);
                dc.lineTo(rect.right() - 5, rect.top() + 1);
                dc.moveTo(rect.left() + 5, rect.top() + 3);
                dc.lineTo(rect.right() - 5, rect.top() + 3);
                if (nHeight > 9)
                {
                    dc.moveTo(rect.left() + 5, rect.top() + 5);
                    dc.lineTo(rect.right() - 5, rect.top() + 5);
                    dc.moveTo(rect.left() + 5, rect.top() + 7);
                    dc.lineTo(rect.right() - 5, rect.top() + 7);
                }
            }

            if (nHeight > 5)
            {
                dc.selectObj(m_penThumbDarker);
                dc.moveTo(rect.left() + 5, rect.top() + 2);
                dc.lineTo(rect.right() - 4, rect.top() + 2);
                dc.moveTo(rect.left() + 5, rect.top() + 4);
                dc.lineTo(rect.right() - 4, rect.top() + 4);
                if (nHeight > 9)
                {
                    dc.moveTo(rect.left() + 5, rect.top() + 6);
                    dc.lineTo(rect.right() - 4, rect.top() + 6);
                }
            }

            if (nHeight > 5)
            {
                dc.setPixel(rect.left() + 4, rect.top() + 1, m_crThumbLighter);
                dc.setPixel(rect.left() + 4, rect.top() + 3, m_crThumbLighter);
                if (nHeight > 9)
                {
                    dc.setPixel(rect.left() + 4, rect.top() + 5, m_crThumbLighter);
                    dc.setPixel(rect.left() + 4, rect.top() + 7, m_crThumbLighter);
                }
            }
        }	// if (IsWindowEnabled())

        dc.selectObj(hOldPen);
        dc.selectObj(hOldBrush); 
    }
    
    void DrawChannel(HDC hdc, LPRECT lpRect, BOOL bHoriz)
    {
        USmartDC dc(hdc);

        huys::URectL rect(lpRect);


        // If the slider has a selection, shrink the rect a little.
        int nRangeMin = getRangeMin();
        int nRangeMax = getRangeMax();
        if (nRangeMin || nRangeMax)
        rect.inflate(0, -1, 0, -1);

        // Another minor adjustment for the rect.
        if (bHoriz)
        rect.inflate(-1, 0, -1, 1);
        else
        rect.inflate(0, -1, 0, -1);

        
        HPEN hOldPen = (HPEN)dc.selectObj(m_penDkShadow);
        UBrush br;
        br.createSolidBrush(m_crChannel);
        HBRUSH hOldBrush = (HBRUSH)dc.selectObj(br);

        // If the height or width is > 3, draw the inner round rect.
        if ((bHoriz && rect.height() > 3) || (!bHoriz && rect.width() > 3))
        dc.roundRect(rect, huys::UPointL(2, 2));

        rect.inflate(1, 1, 1, 0);

        // Give it a sunken appearance.
        dc.selectObj(m_penShadow);
        dc.moveTo(rect.left(), rect.bottom() - 2);
        dc.lineTo(rect.left(), rect.top() + 2);
        dc.lineTo(rect.left() + 2, rect.top());
        dc.lineTo(rect.right() - 2, rect.top());

        dc.selectObj(m_penHilight);
        dc.moveTo(rect.left() + 1, rect.bottom() - 1);
        dc.lineTo(rect.left() + 2, rect.bottom());
        dc.lineTo(rect.right() - 2, rect.bottom());
        dc.lineTo(rect.right() - 2, rect.bottom() - 1);
        dc.lineTo(rect.right() - 1, rect.bottom() - 1);
        dc.lineTo(rect.right() - 1, rect.top() + 2);
        dc.lineTo(rect.right() - 3, rect.top());

        // Draw the selection, if there is one.
        int nSelMin, nSelMax;
        getSelection(nSelMin, nSelMax);
        if (nSelMin < nSelMax)
        {
            huys::URectL rcSel(rect);
            int nAdjust = (bHoriz ? rect.width() : rect.height());
            int nStart = int(((float)nAdjust * float(nSelMin - nRangeMin)) / float(nRangeMax - nRangeMin));
            int nEnd = int(((float)nAdjust * float(nSelMax - nRangeMin)) / float(nRangeMax - nRangeMin));

            if (bHoriz)
            {
                //rcSel.left += nStart;
                //rcSel.right = rcSel.left + (nEnd - nStart);
                rcSel.setLeft(rcSel.left()+nStart);
                rcSel.setWidth(nEnd - nStart);
            }
            else
            {
                //rcSel.top += nStart;
                //rcSel.bottom = rcSel.top + (nEnd - nStart);
                rcSel.setTop(rcSel.top()+nStart);
                rcSel.setHeight(nEnd - nStart);
            }

            if (bHoriz)
            rcSel.deflate(2, 2, 2, 1);
            else
            rcSel.deflate(1, 2, 1, 2);

            UBrush br2;
            br2.createSolidBrush(m_crSelection);
            dc.fillRect(rcSel, br2);
        }

        dc.selectObj(hOldBrush);
        dc.selectObj(hOldPen);
    }
private:
    UPen m_penThumb;
    UPen m_penThumbLight;
    UPen m_penThumbLighter;
    UPen m_penThumbDark;
    UPen m_penThumbDarker;
    UPen m_penDkShadow;
    UPen m_penShadow;
    UPen m_penHilight;

    
    huys::Color m_crSelection;
    huys::Color m_crChannel;
    huys::Color m_crThumb;
    huys::Color m_crThumbLight;
    huys::Color m_crThumbLighter;
    huys::Color m_crThumbDark;
    huys::Color m_crThumbDarker;
    huys::Color m_crDkShadow;
    huys::Color m_crShadow;
    huys::Color m_crHilight;
};

#endif // U_MAC_SLIDERCTRL_H
