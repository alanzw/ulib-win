//#define UNICODE

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <stdio.h>

#include "uwinapp.h"
#include "ubasewindow.h"
#include "ustatic.h"

using namespace Gdiplus;

// Fahrenheit To Celsius
static inline double F2C(double dF){return((dF - 32) / 1.8);}
// Celsius To Fahrenheit
static inline double C2F(double dC){return((dC * 1.8) + 32);}

class UThemoMeter : public UStatic
{
public:
    UThemoMeter(HWND hParent, UINT nID, HINSTANCE hInst)
    : UStatic(hParent, nID, hInst)
    {
        m_dwStyles &= ~SS_SIMPLE;

        m_bDrawTics    = TRUE;
        m_dRangeMin = -20;
        m_dRangeMax = 120;
        m_dVal = 0;
        m_iSmallTicFreq    = 5;
        m_iLargeTicFreq    = 20;
        m_bDisplayF = TRUE;
    }

    UThemoMeter(){}

    ~UThemoMeter()
    {}

    virtual BOOL create()
    {
        BOOL bRet = UControl::create(_T("STATIC"), NULL);
        this->subclassProc();
        return  bRet;
    }

    BOOL onPaint()
    {
        //!! BeginPaint will eat previous control text drawing or other actions
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hSelf, &ps);
        ////hdc = ::GetDC(m_hSelf);
        //
        onDraw(hdc);

        ////::ReleaseDC(m_hSelf, hdc);
        EndPaint(m_hSelf, &ps);
        return FALSE;
    }
    
    void onDraw(HDC hdc)
    {
        RECT rcClient = {0};
        this->getClientRect(&rcClient);

        Graphics gfx(hdc);
        ::InflateRect(&rcClient, -5, -5);

        int w = rcClient.right - rcClient.left;
        int h = rcClient.bottom - rcClient.top;

        Bitmap bmMem(w, h);

        Graphics gfxMem(&bmMem);
        gfxMem.SetSmoothingMode(SmoothingModeAntiAlias);

        // Erase background
        Color clrBackground;
        clrBackground.SetFromCOLORREF(::GetSysColor(COLOR_BTNFACE));
        gfxMem.Clear(clrBackground);

        // Create GDI+ objects
        Color clrFore, clrBack, clrScale, clrOutline;
        clrFore.SetFromCOLORREF(::GetSysColor(COLOR_3DDKSHADOW));
        clrBack.SetFromCOLORREF(::GetSysColor(COLOR_3DFACE));
        clrScale.SetFromCOLORREF(RGB(0,0,0));
        clrOutline.SetFromCOLORREF(RGB(64,0,0));
        Pen fgPen(clrFore);
        Pen    scalePen(clrScale);
        Pen outlinePen(clrOutline);
        SolidBrush blackBrush(Color(255, 0, 0, 0));
        SolidBrush fillBrush(clrBack);
        Font fntText(L"Arial", 10, FontStyleBold);
        StringFormat strfmtText;
        strfmtText.SetAlignment(StringAlignmentCenter);
        strfmtText.SetLineAlignment(StringAlignmentCenter);

        // Everything is drawn from center
        Point ptCenter(rcClient.left + w/2, rcClient.top + h/2);
        float fTmpWidth = (float)( w/ 5.0);

        // Get bulb dimensions
        RectF rBulb((float)ptCenter.X - (float)fTmpWidth,
            (float)rcClient.bottom - (float)((fTmpWidth*2) + 25),
            (float)fTmpWidth*2, (float)fTmpWidth*2);

        // Draw the bulb
        LinearGradientBrush brKnob(rBulb,
            OffsetColor(clrFore, 55),
            OffsetColor(clrFore, -55),
            LinearGradientModeHorizontal);
        gfxMem.FillEllipse(&brKnob, rBulb);
        gfxMem.DrawEllipse(&outlinePen, rBulb);

        // Get cylinder coordinates
        RectF rCylinder(
            (float)ptCenter.X - (float)fTmpWidth/2,
            (float)rcClient.top + (m_bDrawTics ? 25 : 10), // 25 pixels on top for F/C
            (float)fTmpWidth,
            (float)rBulb.GetTop() - rcClient.top - (m_bDrawTics ? 20 : 5)); // 5 pixel overlap over bulb

        // Make sure we have positive values to work with
        float fRange = (float)(m_dRangeMax - m_dRangeMin);
        float fVal = (float)m_dVal;
        if(m_dRangeMin < 0)
            fVal += (float)abs((int)m_dRangeMin);

        // Draw the cylinder
        FillCylinder(&gfxMem, rCylinder, &fillBrush, clrOutline);
        if(fVal > 0)
        {
            // Calculate full rectangle
            float fPctFull = ((fVal / fRange) * 100);
            float fPixFull = ((rCylinder.Height / 100) * fPctFull);
            RectF rFull(
                (float)rCylinder.GetLeft(),
                (float)rCylinder.GetBottom() - fPixFull,
                (float)rCylinder.Width, (float)fPixFull);
            FillCylinder(&gfxMem, rFull, &brKnob, clrOutline);
        }

        // Outline top (empty) plane
        RectF rEmptyTopPlane(rCylinder.X, rCylinder.Y - 5, rCylinder.Width, 5);
        gfxMem.DrawEllipse(&outlinePen, rEmptyTopPlane);

        if(m_bDrawTics)
        {
            Font fntMark(L"Arial", 7);//(fMarkFreq - 2));
            StringFormat strfmtMark;
            strfmtMark.SetAlignment(StringAlignmentFar);
            strfmtMark.SetLineAlignment(StringAlignmentCenter);
            wchar_t strDegree[10];
            Point ptStart, ptEnd;
            PointF ptText;

            // The range and the values are in fahrenheit
            float fPixPerDegree = rCylinder.Height / fRange;
            float fMarkFreq = fPixPerDegree * m_iLargeTicFreq;
            long lMarkVal = (long)m_dRangeMax;

            // Draw large marks and text
            float y;
            for(y = rCylinder.GetTop(); y <= rCylinder.GetBottom(); y += fMarkFreq)
            {
                ptStart = Point((int)rCylinder.GetRight() + 3, (int)y);
                ptEnd = Point((int)rCylinder.GetRight() + 10, (int)y);
                gfxMem.DrawLine(&scalePen, ptStart, ptEnd);
                swprintf(strDegree, L"%d", lMarkVal);
                ptText = PointF((float)rCylinder.GetRight() + 30, (float)y);
                gfxMem.DrawString(strDegree, (int)wcslen(strDegree),
                    &fntMark, ptText, &strfmtMark, &blackBrush);
                lMarkVal -= m_iLargeTicFreq;
            }

            // Draw small marks
            fMarkFreq = fPixPerDegree * m_iSmallTicFreq;
            for(y = rCylinder.GetTop(); y <= rCylinder.GetBottom(); y += fMarkFreq)
            {
                ptStart = Point((int)rCylinder.GetRight() + 3, (int)y);
                ptEnd = Point((int)rCylinder.GetRight() + 8, (int)y);
                gfxMem.DrawLine(&scalePen, ptStart, ptEnd);
            }

            // The range and the values are stored in fahrenheit but we must draw celsius too
            fRange = (float)(F2C(m_dRangeMax) - F2C(m_dRangeMin));
            fPixPerDegree = rCylinder.Height / fRange;
            fMarkFreq = fPixPerDegree * m_iLargeTicFreq;
            lMarkVal = (long)F2C(m_dRangeMax);
            // Round celsius value to a multiple of 10
            int iMod = lMarkVal % 10;
            //if(iMod != 0)
            //    iMod = (10 - iMod);
            lMarkVal -= iMod;
            // Get vertical pixel offset
            int iCYPix = (int)(fPixPerDegree * iMod);

            // Draw large marks and text
            for(float y = rCylinder.GetTop(); y <= rCylinder.GetBottom(); y += fMarkFreq)
            {
                int iCy = (int)y + iCYPix;
                if(iCy > rCylinder.GetBottom())
                    break;
                ptStart = Point((int)rCylinder.GetLeft() - 10, (int)iCy);
                ptEnd = Point((int)rCylinder.GetLeft() - 3, (int)iCy);
                gfxMem.DrawLine(&scalePen, ptStart, ptEnd);
                swprintf(strDegree, L"%d", lMarkVal);
                gfxMem.DrawString(strDegree, (int)wcslen(strDegree),
                    &fntMark, PointF((float)rCylinder.GetLeft() - 15,
                    (float)iCy), &strfmtMark, &blackBrush);
                lMarkVal -= m_iLargeTicFreq;
            }

            // Draw small marks every m_iSmallTicFreq degrees
            fMarkFreq = fPixPerDegree * m_iSmallTicFreq;
            for(y = rCylinder.GetTop(); y <= rCylinder.GetBottom(); y += fMarkFreq)
            {
                int iCy = (int)y + iCYPix;
                if(iCy > rCylinder.GetBottom())
                    break;
                ptStart = Point((int)rCylinder.GetLeft() - 8, (int)iCy);
                ptEnd = Point((int)rCylinder.GetLeft() - 3, (int)iCy);
                gfxMem.DrawLine(&scalePen, ptStart, ptEnd);
            }

            // Draw F/C
            RectF rText = RectF((float)ptCenter.X + 20,
                (float)rcClient.top, (float)20, (float)20);
            gfxMem.DrawString(L"F", 1, &fntText, rText, &strfmtText, &blackBrush);
            rText = RectF((float)ptCenter.X - 40,
                (float)rcClient.top, (float)20, (float)20);
            gfxMem.DrawString(L"C", 1, &fntText, rText, &strfmtText, &blackBrush);
        }

        // Draw the value
        RectF rText((float)rcClient.left, (float)rBulb.GetBottom() + 5,
            (float)w, (float)(rcClient.bottom - (rBulb.GetBottom() + 5)));
        wchar_t strValue[10];
        if(m_bDisplayF)
            swprintf(strValue, L"%.01f F", m_dVal);
        else
            swprintf(strValue, L"%.01f C", F2C(m_dVal));
        gfxMem.DrawString(strValue, (int)wcslen(strValue),
            &fntText, rText, &strfmtText, &blackBrush);

        // Update screen
        gfx.DrawImage(&bmMem, 0, 0);
    }


private:
    BOOL m_bDrawTics;
    // Properties
    double        m_dRangeMin;
    double        m_dRangeMax;
    double        m_dVal;
    int            m_iSmallTicFreq;
    int            m_iLargeTicFreq;
    BOOL        m_bDisplayF;

    void FillCylinder(Graphics* pGfx, RectF rcClient,
        Brush* pFillBrush, Color cOutlineColor)
    {
        RectF rTopPlane(rcClient.X, rcClient.Y - 5, rcClient.Width, 5);
        RectF rBottomPlane(rcClient.X, rcClient.GetBottom() - 5, rcClient.Width, 5);
        // Outline pen
        Pen penOutline(cOutlineColor);
        // Draw body
        GraphicsPath gfxPath;
        gfxPath.AddArc(rTopPlane, 0, 180);
        gfxPath.AddArc(rBottomPlane, 180, -180);
        gfxPath.CloseFigure();
        // Fill body
        pGfx->FillPath(pFillBrush, &gfxPath);
        // Outline body
        pGfx->DrawPath(&penOutline, &gfxPath);
        // Draw top plane
        gfxPath.Reset();
        gfxPath.AddEllipse(rTopPlane);
        // Fill top plane
        pGfx->FillPath(pFillBrush, &gfxPath);
        // Outline top plane
        pGfx->DrawPath(&penOutline, &gfxPath);
    }
    //=================================================
    // Helper Functions
    //=================================================
    Color OffsetColor(Color clr, short sOffset)
    {
        BYTE bRed = 0;
        BYTE bGreen = 0;
        BYTE bBlue = 0;
        short sOffsetR = sOffset;
        short sOffsetG = sOffset;
        short sOffsetB = sOffset;

        if((sOffset < -255) || (sOffset > 255))
            return clr;

        // Get RGB components of specified color
        bRed = clr.GetR();
        bGreen = clr.GetG();
        bBlue = clr.GetB();

        // Calculate max. allowed real offset
        if(sOffset > 0)
        {
            if((bRed + sOffset) > 255)
                sOffsetR = (255 - bRed);
            if((bGreen + sOffset) > 255)
                sOffsetG = (255 - bGreen);
            if((bBlue + sOffset) > 255)
                sOffsetB = (255 - bBlue);

            sOffset = min(min(sOffsetR, sOffsetG), sOffsetB);
        }
        else
        {
            if((bRed + sOffset) < 0)
                sOffsetR = -bRed;
            if((bGreen + sOffset) < 0)
                sOffsetG = -bGreen;
            if((bBlue + sOffset) < 0)
                sOffsetB = -bBlue;

            sOffset = max(max(sOffsetR, sOffsetG), sOffsetB);
        }

        return Color(clr.GetAlpha(), (BYTE)(bRed + sOffset),
            (BYTE)(bGreen + sOffset), (BYTE)(bBlue + sOffset));
    }
};

class GDIPlusWindow : public UBaseWindow
{
    enum {
        ID_THEMOMETER = 5112
    };
public:
    GDIPlusWindow(HINSTANCE hInst = ::GetModuleHandle(NULL))
    : UBaseWindow(NULL, hInst)
    {
        this->setTitle(_T("GDIPlus Window"));
    }

    ~GDIPlusWindow()
    {
        CHECK_PTR(m_pMeter);
    }

    virtual BOOL onCreate()
    {
        startGDIPlus();

        this->setIconBig(IDI_APP);

        m_pMeter = new UThemoMeter(*this, ID_THEMOMETER, this->getInstance());
        m_pMeter->setPos(100, 100, 120, 250);
        m_pMeter->create();
        return UBaseWindow::onCreate();
    }

    virtual BOOL onDestroy()
    {
        stopGDIPlus();
        return UBaseWindow::onDestroy();
    }

    virtual void onDraw(HDC hdc)
    {
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    void startGDIPlus()
    {
        // Initialize GDI+.
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    }

    void stopGDIPlus()
    {
        GdiplusShutdown(gdiplusToken);
    }

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    UThemoMeter *m_pMeter;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    //UDXWinapp app;
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

