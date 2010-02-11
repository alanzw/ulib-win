#ifndef U_INFOBAR_H
#define U_INFOBAR_H

#include <ocidl.h>
#include <olectl.h>
#include <shlwapi.h>

#include "adt/ustring.h"

#include "colors.h"

#include "uicon.h"

class UInfoBar : public UStatic
{
public:
    UInfoBar(HWND hParent)
        : UStatic(hParent, IDC_STATIC, ::GetModuleHandle(NULL))
    {
        m_dwStyles &= ~SS_SIMPLE;
    }

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();

        _icon.loadImage(m_hInstance, IDI_CHAT, 64, 64);
        return  bRet;
    }

    virtual BOOL onPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(m_hSelf, &ps);

        RECT rc;

        this->getClientRect(&rc);

        onDraw(hdc, &rc);

        EndPaint(m_hSelf, &ps);

        return FALSE;

    }

private:
    huys::Color _clrTextTitle;
    huys::Color _clrTextDetail;

    huys::Color _clrBackground;

    huys::ADT::UStringAnsi _strTitle;
    huys::ADT::UStringAnsi _strDetail;

    UIcon _icon;
private:
    enum BackFillMode {
        HVistaGradient,
        HGradient,
        VGradient
    };

    void onDraw(HDC hdc, LPRECT lpRect)
    {
        // background
        DrawGradientFill(hdc, lpRect, HGradient);

        //
        //RECT rc = {5, 5, 5+128, 5+128};
        //pic.show(hdc, &rc);
        _icon.drawEx(hdc, 5, 5, 64, 64);
    }

    //
    // Draw the Inside Gradient.
    //
    void DrawGradientFill(HDC hDCMem, LPRECT lpRect, BackFillMode FillMode)
    {
        huys::Color m_crA = RGB(9, 74, 116);
        huys::Color m_crB = RGB(32, 121, 140);
        huys::Color m_crC = RGB(5, 111, 90);

        TRIVERTEX rcVertex[4];
        rcVertex[0].x=lpRect->left;
        rcVertex[0].y=lpRect->top;
        rcVertex[0].Red=GetRValue(m_crA)<<8;
        rcVertex[0].Green=GetGValue(m_crA)<<8;
        rcVertex[0].Blue=GetBValue(m_crA)<<8;
        rcVertex[0].Alpha=0x0000;

        rcVertex[1].x=lpRect->right /2;
        rcVertex[1].y=lpRect->bottom;
        rcVertex[1].Red=GetRValue(m_crB)<<8;
        rcVertex[1].Green=GetGValue(m_crB)<<8;
        rcVertex[1].Blue=GetBValue(m_crB)<<8;
        rcVertex[1].Alpha=0;

        rcVertex[2].x=lpRect->right /2;
        rcVertex[2].y=lpRect->top;
        rcVertex[2].Red=GetRValue(m_crB)<<8;
        rcVertex[2].Green=GetGValue(m_crB)<<8;
        rcVertex[2].Blue=GetBValue(m_crB)<<8;
        rcVertex[2].Alpha=0;

        rcVertex[3].x=lpRect->right;
        rcVertex[3].y=lpRect->bottom;
        rcVertex[3].Red=GetRValue(m_crC)<<8;
        rcVertex[3].Green=GetGValue(m_crC)<<8;
        rcVertex[3].Blue=GetBValue(m_crC)<<8;
        rcVertex[3].Alpha=0;

        GRADIENT_RECT grect;
        grect.UpperLeft=0;
        grect.LowerRight=1;

        ::GradientFill( hDCMem ,rcVertex,2,&grect,1,
        (FillMode == HGradient) ? GRADIENT_FILL_RECT_H :  GRADIENT_FILL_RECT_V);

        ::GradientFill( hDCMem ,&rcVertex[2],2,&grect,1,
            (FillMode == HGradient) ? GRADIENT_FILL_RECT_H :  GRADIENT_FILL_RECT_V);
    }
};

#endif // U_INFOBAR_H
