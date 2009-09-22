#ifndef  U_IMAGEBUTTON_H_INC
#define  U_IMAGEBUTTON_H_INC

#include "ubutton.h"
#include "colors.h"
#include "uimagelist.h"
#include "ubitmap.h"
#include "ufont.h"

class UImageButton : public UOwnerDrawnButton
{
public:
    enum {
        TEXT_INCLUDE   = 0x0001,
        IMAGE_RIGHT    = 0x0002,
        IMAGE_VCENTER  = 0x0004,
        IMAGE_BOTTOM   = 0x0008,
        IMAGE_HCENTER  = 0x0010,
        FOCUS_TEXTONLY = 0x0020
    };
private:
    DWORD m_dwAlignMode;
    RECT m_rcImage;
    RECT m_rcText;
    RECT m_rcTextFocus;
    huys::Color m_clrText;
    huys::Color m_clrMask;
    BOOL m_bLoaded;
    UBitmap m_ubmImage;
    UBitmap m_ubmDisabled;
    UImageList m_uiml;
public:
    UImageButton(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UOwnerDrawnButton(hParent, nResource, hInst),
      m_dwAlignMode(TEXT_INCLUDE | IMAGE_VCENTER | FOCUS_TEXTONLY),
      m_clrText(::GetSysColor(COLOR_WINDOWTEXT)),
      m_bLoaded(FALSE)
    {

    }

    virtual ~UImageButton()
    {

    }

    bool setButtomImage(UINT uiImageID, COLORREF clrMask)
    {
        if (m_bLoaded)
        {
            m_bLoaded = FALSE;
            m_ubmImage.destroyBitmap();
            m_ubmDisabled.destroyBitmap();
            m_uiml.destroy();
        }

        if (!m_ubmImage.loadFromResource(uiImageID, m_hInstance))
        {
            return false;
        }

        if (!m_uiml.create(m_ubmImage.getWidth(), m_ubmImage.getHeight(), ILC_COLORDDB|ILC_MASK,  1, 1))
        {
            return false;
        }



        m_clrMask = clrMask;

        m_uiml.addMasked(m_ubmImage, clrMask);

        m_ubmDisabled.createMappedBitmap(m_hInstance, uiImageID, clrMask, huys::white);

        m_bLoaded = TRUE;

        this->reCalcLayout();
        this->invalidate();
        return true;
    }
    
    void reset()
    {
        this->reCalcLayout();
        this->invalidate();
    }

    void setAlignMode(DWORD dwAlignMode)
    {
        m_dwAlignMode = dwAlignMode;
    }

    void setTextColor(huys::Color clrText)
    {
        m_clrText = clrText;
    }

    virtual BOOL onDrawItem(WPARAM wParam, LPARAM lParam)
    {
        LPDRAWITEMSTRUCT lpDrawItem;
        HDC hdc;

        lpDrawItem = (LPDRAWITEMSTRUCT) lParam;
        hdc = lpDrawItem->hDC;

        //
        if (lpDrawItem->CtlType != ODT_BUTTON)
            return FALSE;

        HFONT hGuiFont = NULL;
        hGuiFont = (HFONT)::SendMessage(m_hParent, WM_GETFONT, 0, 0);

        if (NULL == hGuiFont)
        {
            hGuiFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
        }

        RECT rcButton = {0};
        rcButton = lpDrawItem->rcItem;
        RECT rcItem = {0};
        rcItem = rcButton;

        BOOL   bFocus      = lpDrawItem->itemState & ODS_FOCUS;
        BOOL   bDisabled   = ODS_DISABLED & lpDrawItem->itemState;

        ::FillRect(hdc, &rcButton, ::GetSysColorBrush(COLOR_BTNFACE));

        if (bFocus)
        {
            rcItem.left += 1;
            rcItem.right -= 1;
            rcItem.top += 1;
            rcItem.bottom -= 1;
        }

        RECT rcFocus = {
            rcItem.left += 3,
            rcItem.top += 3,
            rcItem.right -= 3,
            rcItem.bottom -= 3
        };

        BOOL bPushed = FALSE;
        if ( lpDrawItem->itemState & ODS_SELECTED )
        {
            bPushed = TRUE;
        }
        //
        if (!bPushed)
        {
            ::DrawFrameControl(hdc, &rcItem, DFC_BUTTON , DFCS_BUTTONPUSH);
        }

        if (!bDisabled)
        {
            //Focused
            if( bFocus )
            {
                ::FrameRect(hdc, &rcButton, (HBRUSH)::GetStockObject(BLACK_BRUSH));
            }
            //Pushed
            if( bPushed )
            {
                ::FrameRect(hdc, &rcItem, (HBRUSH)::GetStockObject(BLACK_BRUSH));
            }
        }

        if( ( lpDrawItem->itemState & ODS_FOCUS ) )
        {
            if( m_dwAlignMode & FOCUS_TEXTONLY )
            {
                ::DrawFocusRect(hdc, &m_rcTextFocus);
            }
            else
            {
                ::DrawFocusRect(hdc, &rcFocus);
            }
        }



        if(lpDrawItem->itemAction & ODA_FOCUS)
        {
            if(m_dwAlignMode & FOCUS_TEXTONLY)
            {
                ::DrawFocusRect(hdc, &m_rcTextFocus);
            }
            else
            {
                ::DrawFocusRect(hdc, &rcFocus);
            }
        }

        if (m_bLoaded)
        {
            if (!bDisabled)
            {
                m_uiml.drawIndirect(hdc, 0, m_rcImage.left, m_rcImage.top, m_ubmImage.getWidth(),
                    m_ubmImage.getHeight());
            }
            else
            {
                ::DrawState(hdc, NULL, NULL, (LPARAM)(HBITMAP)m_ubmDisabled, 0,
                    m_rcImage.left, m_rcImage.top, m_ubmDisabled.getWidth(),
                    m_ubmDisabled.getHeight(), DSS_DISABLED |DST_BITMAP);
            }
        }

        // Drawing Text
        HFONT hOldFont = (HFONT)::SelectObject(hdc, hGuiFont);

        TCHAR sCaption[512];

        this->getWindowText(sCaption, 512);

        ::SetBkMode(hdc, TRANSPARENT);

        RECT  rcTemp = {0};
        rcTemp = m_rcText;

        if( bPushed )
        {
            rcTemp.left += 1;
            rcTemp.right += 1;
            rcTemp.top += 1;
            rcTemp.bottom += 1;
        }

        ::SetBkColor(hdc, GetSysColor( COLOR_BTNFACE ));

        if ( ODS_DISABLED & lpDrawItem->itemState )
        {
            // offset 1, 1
            rcTemp.left += 1;
            rcTemp.right += 1;
            rcTemp.top += 1;
            rcTemp.bottom += 1;

            ::SetTextColor(hdc, GetSysColor( COLOR_WINDOW ) );
            ::DrawText(hdc, sCaption, lstrlen(sCaption), &rcTemp, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

            // offset -1, -1
            rcTemp.left -= 1;
            rcTemp.right -= 1;
            rcTemp.top -= 1;
            rcTemp.bottom -= 1;
            ::SetTextColor(hdc, GetSysColor( COLOR_GRAYTEXT));
            ::DrawText(hdc, sCaption, lstrlen(sCaption), &rcTemp, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        else
        {
            ::SetTextColor(hdc, m_clrText);
            ::DrawText(hdc, sCaption, lstrlen(sCaption), &rcTemp , DT_SINGLELINE | DT_CENTER | DT_VCENTER );
        }

        return TRUE;
    }

    virtual BOOL onMessage(UINT nMessage, WPARAM wParam, LPARAM lParam)
    {
        switch (nMessage)
        {
        case WM_SYSCOLORCHANGE:
            return this->onSysColorChange(wParam, lParam);
        case WM_SIZE:
            return this->onSize(wParam, lParam);
        default:
            return UControl::onMessage(nMessage, wParam, lParam);
        }
    }
private:
    BOOL onSysColorChange(WPARAM wParam, LPARAM lParam)
    {
        this->invalidate();
        return FALSE;
    }

    BOOL onSize(WPARAM wParam, LPARAM lParam)
    {
        reCalcLayout();
        this->invalidate();
        return FALSE;
    }

    void reCalcLayout()
    {
        RECT rcClient = {0};
        this->getClientRect(&rcClient);

        ::ZeroMemory(&m_rcImage, sizeof(RECT));
        ::ZeroMemory(&m_rcText, sizeof(RECT));
        ::ZeroMemory(&m_rcTextFocus, sizeof(RECT));

        //Default Left Top
        m_rcImage.top = 6;
        m_rcImage.bottom = m_rcImage.top + m_ubmImage.getHeight();

        m_rcImage.left  = 6;
        m_rcImage.right = m_ubmImage.getWidth() + 6;

        //Image Right
        if( m_dwAlignMode & IMAGE_RIGHT )
        {
            m_rcImage.left  = ( rcClient.right - m_ubmImage.getWidth() ) - 6;
            m_rcImage.right = m_rcImage.left + m_ubmImage.getWidth();
        }

        //Image Center
        if( m_dwAlignMode & IMAGE_VCENTER )
        {
            int iTopIndex = ( rcClient.bottom - rcClient.top) - ( m_rcImage.bottom - m_rcImage.top);
            if( iTopIndex > 0 )
            {
                iTopIndex = iTopIndex / 2;
                if( iTopIndex < 4 )
                {
                    iTopIndex = 4;
                }
                m_rcImage.top = iTopIndex;
                m_rcImage.bottom = m_rcImage.top + m_ubmImage.getHeight();
            }

        }
        else if( m_dwAlignMode & IMAGE_BOTTOM )
        {

            m_rcImage.top = rcClient.bottom - rcClient.top - m_ubmImage.getHeight() - 6;
            if( m_rcImage.top < 6 )
                m_rcImage.top = 6;

            m_rcImage.bottom = m_rcImage.top + m_ubmImage.getHeight();

        }


        if( ( m_dwAlignMode & IMAGE_HCENTER ) )
        {
            int iTopIndex = rcClient.right - rcClient.left - m_ubmImage.getWidth();
            if( iTopIndex > 0 ) {

                iTopIndex = iTopIndex / 2;

                if( iTopIndex < 4 )
                    iTopIndex = 4;

                m_rcImage.left   = iTopIndex;
                m_rcImage.right  = m_rcImage.left + m_ubmImage.getWidth();

            }

        }

        if( m_dwAlignMode & TEXT_INCLUDE ) {  //Including Text

            if( m_dwAlignMode & IMAGE_VCENTER ) {

                m_rcText.top    = 6;
                m_rcText.bottom = rcClient.bottom - 6;

                //Calculate Text Focus Rect
                m_rcTextFocus.top = 4;
                m_rcTextFocus.bottom = rcClient.bottom - 4;

            }
            else {

                if( m_dwAlignMode & IMAGE_BOTTOM ) {

                    m_rcText.top    = 6;
                    m_rcText.bottom = m_rcImage.top - 6;

                    //Calculate Text Focus Rect
                    m_rcTextFocus.top = 4;
                    m_rcTextFocus.bottom = m_rcImage.top - 4;


                }
                else {

                    m_rcText.top    = m_rcImage.bottom + 6;
                    m_rcText.bottom = rcClient.bottom - 6;

                    //Calculate Text Focus Rect
                    m_rcTextFocus.top = m_rcImage.bottom + 4;
                    m_rcTextFocus.bottom = rcClient.bottom - 4;


                }

            }

            //Image Center
            if( m_dwAlignMode & IMAGE_VCENTER ) {

                if( m_dwAlignMode & IMAGE_RIGHT ) {
                    m_rcText.left   = 6;
                    m_rcText.right  = m_rcImage.left - 6;

                    //Calculate Text Focus Rect
                    m_rcTextFocus.left  = 4;
                    m_rcTextFocus.right = m_rcImage.left - 4;

                }
                else {

                    m_rcText.left   = m_rcImage.right + 6;
                    m_rcText.right  = rcClient.right  - 6;

                    //Calculate Text Focus Rect
                    m_rcTextFocus.left  = m_rcImage.right + 4;
                    m_rcTextFocus.right = rcClient.right  - 4;

                }

            }
            else {

                m_rcText.left   = 6;
                m_rcText.right  = rcClient.right  - 6;

                //Calculate Text Focus Rect
                m_rcTextFocus.left  = 4;
                m_rcTextFocus.right = rcClient.right  - 4;
            }

        }
    }
};

#endif   /* ----- #ifndef U_IMAGEBUTTON_H_INC  ----- */
