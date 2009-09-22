/*
 * =====================================================================================
 *
 *       Filename:  ukav_tipbar.h
 *
 *    Description:  UKav TipBar
 *
 *        Version:  1.0
 *        Created:  08/15/2009 06:50:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_KAV_TIPBAR_H
#define U_KAV_TIPBAR_H

#include "ustatic.h"
#include "colors.h"

class UTipBar : public UStatic
{
    enum {
        MAX_TIP_LENGTH = 512
    };
public:
    UTipBar(HWND hParent, UINT nID)
        : UStatic(hParent, nID, ::GetModuleHandle(NULL))
    {
        //m_dwStyles &= ~SS_SIMPLE;
        m_dwStyles |= SS_BLACKRECT;
    }

    ~UTipBar()
    {}

    virtual BOOL create()
    {
        BOOL bRet = UStatic::create();
        this->subclassProc();
        return  bRet;
    }

    virtual BOOL onPaint()
    {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rc;

        this->getClientRect(&rc);

        hdc = BeginPaint(m_hSelf, &ps);

        draw(hdc, rc);

        EndPaint(m_hSelf, &ps);

        return FALSE;
    }

    void setInfo(const TCHAR *sInfo)
    {
        lstrcpy(m_sInfo, sInfo);
    }

    void setBkBmp(UBitmap *ubmp)
    {
        m_ubmp = ubmp;
    }

//    virtual BOOL onCtrlColor(WPARAM wParam, LPARAM lParam)
//    {
//        HDC hdc = (HDC)wParam;
//        ::SetBkMode(hdc, TRANSPARENT);
//        HBRUSH hBrush = ::CreateSolidBrush(huys::darkvoilet);
//        return (BOOL)hBrush;
//    }

private:
    TCHAR m_sInfo[MAX_TIP_LENGTH];
    UBitmap *m_ubmp;

    void draw(HDC &hdc, RECT &rc)
    {

        ::SetBkMode(hdc, TRANSPARENT);
        ::SetBkColor(hdc, huys::deeppink);
        ::SetTextColor(hdc, huys::blueviolet);

        if (NULL != m_ubmp)
        {
            m_ubmp->showStretch(hdc, rc);
        }

        //Rectangle(hdc, rc.left+1, rc.top+1, rc.right-1, rc.bottom-1);
        huys::URectangle urc(rc.left+1, rc.top+1, rc.right-1, rc.bottom-1);
        urc.setFilledColor(huys::deeppink);
        urc.setFilledStyle(BS_SOLID);
        urc.Draw(hdc);


        int nHeight = rc.bottom - rc.top;
        int nWidth = rc.right - rc.left;
        RECT rect = {
            rc.left + nWidth / 2 - 40,
            rc.top + nHeight/2 - 10,
            rc.right - 10,
            rc.bottom - 10
        };

        //::ExtTextOut(hdc, rect.left, rect.top, ETO_OPAQUE, &rect, m_sInfo, lstrlen(m_sInfo), NULL);
        ::TextOut(hdc, rect.left, rect.top, m_sInfo, lstrlen(m_sInfo));
    }
};



#endif // U_KAV_TIPBAR_H

