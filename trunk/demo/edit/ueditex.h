
/*
 * =====================================================================================
 *
 *       Filename:  ueditex.h
 *
 *    Description:  Class UEditEx
 *
 *        Version:  1.0
 *        Created:  2009/9/26 12:20:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_EDIT_EX_H
#define U_EDIT_EX_H

class UEditEx : public UEdit
{
public:
    UEditEx(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UEdit(hParent, nResource, hInst)
    {
        m_clrBKDef = huys::tomato;
        m_clrBKSub = huys::green;

        m_clrBK = m_clrBKDef;

        m_clrText = huys::black;
    }

    // Redirect the Control Procedure
    BOOL create()
    {
        return UEdit::create() && this->subclassProc();
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        //if (wParam <'0' || wParam >'9')
        //   return 0;
        //char code[10];
        //wsprintf(code, "0x%x -- %c", wParam, wParam);
        if (wParam >= '0' && wParam <= '9' || wParam == VK_RETURN ) return 0;
         //   ::MessageBox(m_hSelf, "xx", code, MB_OK);
        return TRUE;
    }

    virtual BOOL onPaint()
    {
        UPaintDC dc(m_hSelf);

        char str[256];
        this->getText(str);

        drawit(str, dc);

        return FALSE;
    }

    virtual BOOL onSetFocus()
    {
        m_clrBK = m_clrBKSub;
        this->invalidate();
        return FALSE;
    }

    virtual BOOL onKillFocus()
    {
        m_clrBK = m_clrBKDef;
        this->invalidate();
        return FALSE;
    }

public:
    void setBKColorDefault(huys::Color clr)
    { m_clrBK = m_clrBKDef = clr; }

    void setBKColorSubstitute(huys::Color clr)
    { m_clrBKSub = clr; }

    void setTextColor(huys::Color clr)
    { m_clrText = clr; }

private:

    BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdcChild = (HDC)wParam;
        SetTextColor(hdcChild, m_clrText);
        SetBkColor(hdcChild, m_clrBK);
        return (BOOL)(HBRUSH)m_brush;
    }

    void drawit(char *str, HDC hdc)
    {
        if (m_brush.isNull())
        {
            m_brush.createSolidBrush(m_clrBK);
        }
        else
        {
            m_brush.destroy();
            m_brush.createSolidBrush(m_clrBK);
        }

        USmartDC dc(hdc);

        dc.setBKMode(OPAQUE);
        dc.setBKColor(m_clrBK);
 /*
        HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, m_brush);
        HPEN hOldPen = (HPEN)::SelectObject(hdc, ::GetStockObject(NULL_PEN));
        RECT rc;
        ::GetClientRect(m_hSelf, &rc);
        //::ScreenToClient(m_hSelf, (LPPOINT)&rc);
        //rc.left += 5;
        //rc.top += 5;
        ::Rectangle(hdc, 0, 0, rc.right - rc.left + 1, rc.bottom - rc.top + 1);
        ::SetTextColor(hdc, huys::black);
        //::TextOut(hdc, 2, 2, str, strlen(str));
        ::DrawText(hdc, str, strlen(str), &rc, DT_EDITCONTROL);
        ::SelectObject(hdc, hOldBrush);
        //::SelectObject(hdc, hOldPen);
*/
        HBRUSH hOldBrush = (HBRUSH)dc.selectObj(m_brush);
        HPEN hOldPen = (HPEN)dc.selectObj(::GetStockObject(NULL_PEN));
        dc.selectObj(this->getFont());

        huys::URectL rect;
        this->getClientRect(rect);

        dc.rectangle(0, 0, rect.width() + 1, rect.height() + 1);

        dc.setTextColor(m_clrText);
        dc.drawText(str, strlen(str), rect, DT_EDITCONTROL);
        dc.selectObj(hOldBrush);
        dc.selectObj(hOldPen);
    }
protected:
    UBrush m_brush;

    huys::Color m_clrBK;
    huys::Color m_clrText;

    huys::Color m_clrBKDef;
    huys::Color m_clrBKSub;
};

class UEditFillet : public UEditEx
{
    //鼠标滑过时的效果
    enum
    {
        FOCUS_DEFAULT,			//默认效果：只有边框颜色变化
        FOCUS_ENLARGE,			//文本框变大		
        FOCUS_BKCOLOR,			//背景色变化
        FOCUS_BORDERTHICK = 4	//边框厚度变化
    };
public:
    UEditFillet(HWND hParent, UINT nResource, HINSTANCE hInst)
        : UEditEx(hParent, nResource, hInst)
    {
        m_clrOutterShadow = RGB(230, 230, 230);
	    m_clrInnerShadow = RGB(244, 244, 244);

        m_clrBorderHover = RGB(150, 150, 150);
	    m_clrBorderNormal = RGB(192, 192, 192);

        m_nHoverStyle = 0;
        m_bHover = FALSE;
    }

    /* virtual */ BOOL onEraseBkgnd(HDC hdc)
    {
        USmartDC dc(hdc);

        UBrush brhInnerShadow, brhOutterShadow;
        brhInnerShadow.createSolidBrush(m_clrInnerShadow);
        brhOutterShadow.createSolidBrush(m_clrOutterShadow);
        
        HBRUSH hOldBrush = (HBRUSH)dc.selectObj(brhOutterShadow);
        HPEN hOldPen = (HPEN)dc.selectObj(::GetStockObject(NULL_PEN));
        
        huys::URectL rcClient;
        
        this->getClientRect(rcClient);

        huys::URectL rcShadow = rcClient;
        rcShadow.inflate(3, 3);
        
        //外阴影
        rcShadow.offset(3, 3);
        dc.roundRect(rcShadow.left(), rcShadow.top(),
            rcShadow.right(), rcShadow.bottom(), 5, 5);
        
        //内阴影
        dc.selectObj(brhInnerShadow);
        rcShadow.offset(-3, -3);
        dc.roundRect(rcShadow.left(), rcShadow.top(),
            rcShadow.right(), rcShadow.bottom(), 5, 5);
        
        //可编辑区域
        UBrush brhEdit;
        brhEdit.createSolidBrush(m_clrBK);
        dc.selectObj(brhEdit);		
        dc.rectangle(rcClient);
        
        
		dc.selectObj(hOldBrush);

        //边缘圆角框
        
        UPen pen;
        if(m_bHover)
        {
            pen.createPen(PS_SOLID, (m_nHoverStyle & FOCUS_BORDERTHICK) ? m_nHoverBorderThick : 1, m_clrBorderHover);
        }
        else
            pen.createPen(PS_SOLID, 1, m_clrBorderNormal);
        
        dc.selectObj(pen);
        dc.selectObj(::GetStockObject(NULL_BRUSH));
        
        huys::URectL rcBorder = rcClient;
        rcBorder.inflate(3, 3);
        
        dc.roundRect(rcBorder.left(), rcBorder.top(),
            rcBorder.right(), rcBorder.bottom(), 5,5);

        return UEditEx::onEraseBkgnd(hdc);
    }
private:
    huys::Color m_clrInnerShadow;
    huys::Color m_clrOutterShadow;

    huys::Color m_clrBorderHover;
    huys::Color m_clrBorderNormal;

    BOOL m_bHover;
    int m_nHoverStyle;
    int m_nHoverBorderThick;
};

#endif // U_EDIT_EX_H

