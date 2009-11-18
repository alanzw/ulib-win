//#define UNICODE
#define  _WIN32_WINNT 0x0400

#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#ifdef __GNUC__
#include <gdiplus/gdiPlus.h>
#else
#include <gdiPlus.h>
#endif

#include "ubasewindow.h"
#include "uwinapp.h"

using namespace Gdiplus;

class UGDIPlusButton : public UBaseWindow
{
public:
    UGDIPlusButton(UBaseWindow *pWndParent, UINT nID)
        : UBaseWindow(*pWndParent, NULL, _T("UGDIPLUSBUTTON"), nID),
        m_hRgn(0), m_pCachedBitmap(0),
        m_bMouseOver(FALSE),
        m_bTracking(FALSE),
        m_bMouseClicked(FALSE),
		m_bDirtyBuffer(TRUE)
    {
        addStyles(WS_CHILD);
    }

    ~UGDIPlusButton()
    {

    }

	BOOL onDestroy()
	{
		if (m_pCachedBitmap)
		{
			delete m_pCachedBitmap;
			m_pCachedBitmap = NULL;
		}

		if (m_hRgn)
		{
			DeleteObject(m_hRgn);
			m_hRgn = NULL;
		}
		return FALSE;
	}

    void onDraw(HDC hdc)
    {
        Graphics graphics(hdc);

        // Only redraw the control if the buffer is dirty
        if (m_bDirtyBuffer)
        {
            DrawButton(graphics);
            m_bDirtyBuffer = FALSE;
        }

        graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
    }

	virtual BOOL onSize(WPARAM wParam, LPARAM lParam)
	{
		m_bDirtyBuffer = TRUE;
		return FALSE;
	}

    BOOL onLButtonDown(WPARAM wParam, LPARAM lParam)
    {
        m_bMouseClicked = !m_bMouseClicked;
		m_bDirtyBuffer = TRUE;
        this->invalidate(TRUE);
		
        return FALSE;
    }

	BOOL onLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		// Make the button's BN_CLICKED notify work
		::SendMessage(getParent(), WM_COMMAND, MAKEWPARAM(getID(), BN_CLICKED), 0);
		return FALSE;
	}

    BOOL filterMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_ERASEBKGND == uMessage)
        {
            return onEraseBkGnd((HDC)wParam);
        }

		if (WM_MOUSEMOVE == uMessage)
		{
			return onMouseMove(wParam, lParam);
		}
		

        if (WM_MOUSEHOVER == uMessage)
        {
            return onMouseHover();
        }

        if (WM_MOUSELEAVE == uMessage)
        {
            return onMouseLeave();
        }

		if (WM_LBUTTONUP == uMessage)
		{
			return onLButtonUp(wParam, lParam);
		}
		

        return FALSE;
    }

    BOOL onEraseBkGnd(HDC hdc)
    {
        return TRUE;
    }

    BOOL onMouseMove(WPARAM wParam, LPARAM lParam)
    {
        if (!m_bTracking)
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = *this;
            tme.dwFlags = TME_LEAVE|TME_HOVER;
            tme.dwHoverTime = 1;
            m_bTracking = TrackMouseEvent(&tme);
			m_bTracking = TRUE;
        }
        if(m_bMouseOver == FALSE)
        {
            m_bMouseOver = TRUE;
        }
		return FALSE;
    }

    BOOL onMouseHover()
    {
		m_bDirtyBuffer = TRUE;
		m_bMouseOver = TRUE;
		m_bTracking = TRUE;
        this->invalidate();
        return TRUE;
    }

    BOOL onMouseLeave()
    {
		m_bDirtyBuffer = TRUE;
        m_bMouseOver = FALSE;
        m_bTracking = FALSE;
        this->invalidate();
        return TRUE;
    }
private:
    HRGN m_hRgn;
    RECT m_rcClient;
    CachedBitmap *m_pCachedBitmap;
    /** m_bMouseOver: set to true then the mouse coursor move over the control */
    BOOL m_bMouseOver;
    /** m_bTracking: helps in establishing mouse-over-control event*/
    BOOL m_bTracking;
    /** m_bMouseClicked: this variable toggles whenever user left clicks the control */
    BOOL m_bMouseClicked;
	BOOL m_bDirtyBuffer;
public:
    //
    BOOL reCalcSize()
    {
        //this->hide();
        // Calculate CDialog offset
        POINT ptParent = {0, 0};
        ::ClientToScreen(*this, &ptParent);
        ::ScreenToClient(getParent(), &ptParent);

        // Get the device context & create Graphics class
        Graphics graphics(*this);

        // Obtain client rectangle
        RECT rect;
        GetClientRect(*this, &rect);
        m_rcClient = rect;

        // Draw the region
        GraphicsPath path;
        path.AddEllipse((int)m_rcClient.left, (int)m_rcClient.top,
            (int)(m_rcClient.right-m_rcClient.left),
            (int)(m_rcClient.bottom-m_rcClient.top));
        Region rgn(&path);
        SetWindowRgn(*this, rgn.GetHRGN(&graphics), FALSE);

        // Apply CDialog offset
        rgn.Translate((int)ptParent.x, (int)ptParent.y);
        m_hRgn = rgn.GetHRGN(&graphics);
        //this->show();
        //this->update();

        return TRUE;
    }

    void DrawButton(Graphics &graphics)
    {
        Graphics* pMemGraphics;
        Bitmap*    pMemBitmap;

        // create off-screen bitmap
        pMemBitmap = new Bitmap(m_rcClient.right - m_rcClient.left,
            m_rcClient.bottom - m_rcClient.top);

        // create off-screen graphics
        pMemGraphics = Graphics::FromImage(pMemBitmap);

        // Turn anti-aliasing ON
        pMemGraphics->SetSmoothingMode(SmoothingModeAntiAlias);

        /************************************************/
        /*            ACTUAL DRAWING START HERE            */
        /************************************************/

        // Calculate component sizes (based on the client size)
        Rect rcInner(m_rcClient.left, m_rcClient.top, m_rcClient.right-m_rcClient.left, m_rcClient.bottom-m_rcClient.top);
        Rect rcInnerOutline(m_rcClient.left, m_rcClient.top, m_rcClient.right-m_rcClient.left, m_rcClient.bottom-m_rcClient.top);
        Rect rcOutter(m_rcClient.left, m_rcClient.top, m_rcClient.right-m_rcClient.left, m_rcClient.bottom-m_rcClient.top);
        rcInner.Inflate(-15, -15);
        rcInnerOutline.Inflate(-1, -1);

        // Create appropriate colors and brushes (these are client size dependent)
        LinearGradientBrush InnerBrushMouseNoClick( rcInner, Color(255, 0, 0, 255), Color(255, 0, 0, 100), (REAL)0, TRUE);
        LinearGradientBrush InnerBrushMouseClicked( rcInner, Color(255, 100, 149, 255), Color(255, 100, 149, 255), (REAL)0, TRUE);
        LinearGradientBrush OutterBrushMouseOut( rcOutter, Color(255, 100, 100, 100), Color(255, 200, 200, 200), (REAL)0, TRUE);
        LinearGradientBrush OutterBrushMouseIn ( rcOutter, Color(100, 150, 150, 0), Color(200, 255, 255, 0), (REAL)0, TRUE);
        Pen blackPen((ARGB)Color::Black);

        // Draw the background
        if (!m_bMouseOver)
            pMemGraphics->FillEllipse(&OutterBrushMouseOut, rcOutter);
        else
            pMemGraphics->FillEllipse(&OutterBrushMouseIn, rcOutter);

        if (!m_bMouseClicked)
            pMemGraphics->FillEllipse(&InnerBrushMouseNoClick, rcInner);
        else
            pMemGraphics->FillEllipse(&InnerBrushMouseClicked, rcInner);

        pMemGraphics->DrawEllipse(&blackPen, rcInner);
        pMemGraphics->DrawEllipse(&blackPen, rcInnerOutline);

        // Prepare brush and font for text drawing
        SolidBrush  fontBrush(Color(255, 255, 255, 255));
        REAL rFontSize = static_cast<REAL>(min((m_rcClient.bottom- m_rcClient.top)*0.2, (m_rcClient.right-m_rcClient.left)*0.2));
        Font font(L"Times New Roman", rFontSize, FontStyleBold, UnitPixel );

        // Calculate text placement (center)
        RectF textBoundRect;
        PointF ptTextOrigin;
        pMemGraphics->MeasureString(L"Click!", 6, &font, ptTextOrigin, &textBoundRect);
        ptTextOrigin    = PointF ((m_rcClient.right-m_rcClient.left-textBoundRect.Width)/2+1, (m_rcClient.bottom - m_rcClient.top -textBoundRect.Height)/2+1);

        // Finally draw the text
        pMemGraphics->DrawString(L"Click!", -1, &font, ptTextOrigin, &fontBrush);


        /************************************************/
        /*            ACTUAL DRAWING ENDS HERE            */
        /************************************************/

        // Create a cached bitmap for double buffering
        if (m_pCachedBitmap) delete m_pCachedBitmap;
        m_pCachedBitmap = new CachedBitmap(pMemBitmap, &graphics);

        delete pMemBitmap;
        delete pMemGraphics;
    }

};

class GDIPlusWindow : public UBaseWindow
{
public:
    GDIPlusWindow()
        : UBaseWindow(NULL, NULL),
		m_pGDIPlusButton(NULL)
    {
        setTitle(_T("GDIPlus Window"));
        addStyles(WS_CLIPCHILDREN);
    }

    ~GDIPlusWindow()
    {
        CHECK_PTR(m_pGDIPlusButton);
    }

    virtual BOOL onCreate()
    {
        this->setIconBig(IDI_APP);
        startGDIPlus();

        m_pGDIPlusButton = new UGDIPlusButton(this, 11111);
        m_pGDIPlusButton->setPos(200, 200, 200, 200);
        m_pGDIPlusButton->create();
        m_pGDIPlusButton->reCalcSize();
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
    
    virtual BOOL onCommand(WPARAM wParam, LPARAM lParam)
    {
        switch (LOWORD (wParam))
        {
        case 11111:
            {
            showMsg(_T("How are you!"), _T("Hello"));
            return FALSE;
            }
        default:
            return UBaseWindow::onCommand(wParam, lParam);
        }
    }

    //BOOL onEraseBkgnd(HDC hdc)
    //{
    //    m_pGDIPlusButton->reCalcSize();
    //    return TRUE;
    //}
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

    UGDIPlusButton *m_pGDIPlusButton;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new GDIPlusWindow);
    app.init(hInstance);

    return app.run();
}

