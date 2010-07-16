#ifndef U_AUI_ENTRY_H
#define U_AUI_ENTRY_H

#include "uedit.h"
#include "udc.h"
#include "ubrush.h"
#include "adt/uautoptr.h"
#include "colors.h"

namespace AUI
{

typedef UEdit UEntry;

typedef huys::ADT::UAutoPtr<UEntry> UEntryP;

class UIntEntry : public UEntry
{
public:
    UIntEntry(HWND hParent, UINT nResource, HINSTANCE hInst)
    : UEdit(hParent, nResource, hInst)
    {
        setDefaultColors();
    }

    UIntEntry(UBaseWindow *pWndParent, UINT nResource)
    : UEdit(pWndParent, nResource)
    {
        setDefaultColors();
    }

    // Redirect the Control Procedure
    BOOL create()
    {
        return UEdit::create() && this->subclassProc();
    }

    virtual BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        if (wParam >= '0' && wParam <= '9') return FALSE;
        return TRUE;
    }

    virtual BOOL onPaint()
    {
        UPaintDC dc(m_hSelf);

        char str[256];
        this->getText(str);

        if (m_brush.isNull())
        {
            m_brush.createSolidBrush(m_clrBK);
        }
        else
        {
            m_brush.destroy();
            m_brush.createSolidBrush(m_clrBK);
        }

        dc.setBKMode(OPAQUE);
        dc.setBKColor(m_clrBK);

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
    { m_clrBKDef = clr; }

    void setBKColorSubstitute(huys::Color clr)
    { m_clrBKSub = clr; }

    void setTextColor(huys::Color clr)
    { m_clrText = clr; }

private:

    void setDefaultColors()
    {
        m_clrBKDef = huys::tomato;
        m_clrBKSub = huys::green;

        m_clrBK = m_clrBKDef;

        m_clrText = huys::black;
    }

    BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
    {
        HDC hdcChild = (HDC)wParam;
        SetTextColor(hdcChild, m_clrText);
        SetBkColor(hdcChild, m_clrBK);
        return (BOOL)(HBRUSH)m_brush;
    }
private:
    UBrush m_brush;

    huys::Color m_clrBK;
    huys::Color m_clrText;

    huys::Color m_clrBKDef;
    huys::Color m_clrBKSub;

};

typedef huys::ADT::UAutoPtr<UIntEntry> UIntEntryP;

}; // namespace AUI

#endif // U_AUI_ENTRY_H
