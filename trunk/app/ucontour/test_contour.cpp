/*
 * =====================================================================================
 *
 *       Filename:  test_contour.cpp
 *
 *    Description:  Test for creating a contour
 *
 *        Version:  1.0
 *        Created:  08/21/2009 10:25:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udlgapp.h"
#include "udialogx.h"
#include "ugdi.h"
#include "colors.h"

using huys::UDialogBox;

class UDialogContour : public UDialogBox
{
public:
    UDialogContour(HINSTANCE hInst, UINT nID)
    : UDialogBox(hInst, nID)
    {
    }

    BOOL onInit()
    {
        return TRUE;
    }

    void onDraw(HDC hdc)
    {
        RECT rc;
        ::GetClientRect(m_hDlg, &rc);
        drawGrid(hdc, rc);
    }
private:
    void drawGrid(HDC &hdc, RECT &rc)
    {
        huys::URectangle urc(20, 20, 260, 260);
        urc.setLineColor(huys::red);
        urc.Draw(hdc);

        huys::ULine ul(20, 80, 260, 80);
        ul.setLineColor(huys::red);
        ul.Draw(hdc);

        ul.offsetY(60);
        ul.Draw(hdc);

        ul.offsetY(60);
        ul.Draw(hdc);

        ul.SetStartPnt(80, 20);
        ul.SetEndPnt(80, 260);
        ul.Draw(hdc);

        ul.offsetX(60);
        ul.Draw(hdc);

        ul.offsetX(60);
        ul.Draw(hdc);
    }
};

UDLGAPP_T(UDialogContour, IDD_CONTOUR);

