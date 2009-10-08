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


/*************************************************************

   *                                                            *
   *  DrawBackgroundPattern()                                   *
   *                                                            *
   *  Purpose: This function draws a gradient pattern that      *
   *           transitions between blue and black. This is      *
   *           similar to the background used in Microsoft      *
   *           setup programs.                                  *
   *                                                            *
   *************************************************************/

void DrawBackgroundPattern(HWND hWnd, HDC hdc)
{
   HDC hDC = hdc;  // Get the DC for the window
   RECT rectFill;          // Rectangle for filling band
   RECT rectClient;        // Rectangle for entire client area
   float fStep;            // How large is each band?
   HBRUSH hBrush;

   int iOnBand;  // Loop index

   // How large is the area you need to fill?

   GetClientRect(hWnd, &rectClient);

   // Determine how large each band should be in order to cover the
   // client with 256 bands (one for every color intensity level)

   fStep = (float)rectClient.bottom / 256.0f;

   // Start filling bands

   for (iOnBand = 0; iOnBand < 256; iOnBand++) {

    // Set the location of the current band
    SetRect(&rectFill,
            0,                           // Upper left X
            (int)(iOnBand * fStep),      // Upper left Y
            rectClient.right+1,          // Lower right X
            (int)((iOnBand+1) * fStep)); // Lower right Y

   // Create a brush with the appropriate color for this band
   hBrush = CreateSolidBrush(RGB(0, 0, (255 - iOnBand)));

   // Fill the rectangle
   FillRect(hDC, &rectFill, hBrush);

   // Get rid of the brush you created
   DeleteObject(hBrush);

   };

   // Give back the DC

   //ReleaseDC(hWnd, hDC);
}


// Interpolates intermediate value at N
// X0 corresponds to N = 0 and X1 to N = 255
#define IPOL(X0, X1, N) ((X0) + ((X1) - (X0)) * N / 256)


// Fills the rectangle 'rect' in graded colours
void GradientFill(HDC hdc, const RECT &rect, COLORREF* color_array)
{
  // Calculates size of single colour bands
  int xStep = (rect.right - rect.left) / 256 + 1;
  int yStep = (rect.bottom - rect.top) / 256 + 1;
  // X loop starts
  for (int iX = 0, X = rect.left; iX < 256; iX++)
  {
    // Calculates end colours of the band in Y direction
    int RGBColor[3][2] = {
      {IPOL(GetRValue(color_array[0]), GetRValue(color_array[1]), iX),
        IPOL(GetRValue(color_array[3]), GetRValue(color_array[2]), iX)
      },
      {IPOL(GetGValue(color_array[0]),  GetGValue(color_array[1]), iX),
        IPOL(GetGValue(color_array[3]), GetGValue(color_array[2]), iX)
      },
      {IPOL(GetBValue(color_array[0]), GetBValue(color_array[1]), iX),
        IPOL(GetBValue(color_array[3]), GetBValue(color_array[2]), iX)
      }
    };
    // Y loop starts
    for (int iY = 0, Y = rect.top; iY < 256; iY++)
    {
      // Calculates the colour of the rectangular band
      COLORREF Color = RGB(IPOL(RGBColor[0][0], RGBColor[0][1], iY),
        IPOL(RGBColor[1][0], RGBColor[1][1],  iY),
        IPOL(RGBColor[2][0], RGBColor[2][1], iY));
      // Creates the brush to fill the rectangle
      HBRUSH hBrush = CreateSolidBrush(Color);
      // Paints the rectangular band with the brush
      RECT Rect = {X, Y, X + xStep, Y + yStep};
      FillRect(hdc, &Rect, hBrush);
      // Deletes the brush
      DeleteObject(hBrush);
      // Updates Y value of the rectangle
      Y += yStep;
      if (Y > rect.bottom)
        Y = rect.bottom;
    }
    // Updates X value of the rectangle
    X += xStep;
    if (X > rect.right)
      X = rect.right;
  }
}

// Fills the region 'rgn' in graded colours
void GradientFill(HDC hdc, HRGN rgn, COLORREF* color_array)
{
  // Creates memory DC
  HDC hMemDC = CreateCompatibleDC(hdc);
  if (hMemDC)  // Memory DC creation successful
  {
    // Gets bounding rectangle of region
    RECT rectRgn;
    GetRgnBox(rgn, &rectRgn);
    // Left top point of applying mask
    int X = rectRgn.left, Y = rectRgn.top;
    // Size of mask
    int Width = rectRgn.right - X, Height = rectRgn.bottom - Y;
    // Creates bitmap for the mask
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, Width, Height);
    if (hBitmap)  // Bitmap created successfully
    {
      // Selects bitmap in memory DC
      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
      // Prepares gradient filled mask and applies to output DC
      OffsetRect(&rectRgn, -rectRgn.left, -rectRgn.top);
      GradientFill(hMemDC, rectRgn, color_array);
      BitBlt(hdc, X, Y, Width, Height, hMemDC, 0, 0, SRCINVERT);
      // Displays region in black in output DC
      FillRgn(hdc, rgn, (HBRUSH) GetStockObject(BLACK_BRUSH));
      // Applies mask to output DC again
      BitBlt(hdc, X, Y, Width, Height, hMemDC, 0, 0, SRCINVERT);
      // De-selects bitmap from memory DC
      SelectObject(hMemDC, hOldBitmap);
      // Deletes bitmap
      DeleteObject(hBitmap);
    }
    // Deletes memory DC
    DeleteDC(hMemDC);
  }
}

// Fills the text 'text' in graded colours at (x, y)
void GradientFill(HDC hdc, int x, int y, LPSTR text, COLORREF* color_array)
{
  // Creates memory DC
  HDC hMemDC = CreateCompatibleDC(hdc);
  if (hMemDC)          // Memory DC creation successful
  {
    // Gets size of the string
    SIZE Size;
    GetTextExtentPoint32(hdc, text, lstrlen(text), &Size);
    // Creates bounding rectangle of the text
    RECT rectText = {x, y, x + Size.cx, y + Size.cy};
    // Creates bitmap for the mask
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, Size.cx, Size.cy);
    if (hBitmap)      // Bitmap created successfully
    {
      // Selects bitmap in memory DC
      HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
      // Prepares gradient filled mask and applies to output DC
      OffsetRect(&rectText, -rectText.left, -rectText.top);
      GradientFill(hMemDC, rectText, color_array);
      BitBlt(hdc, x, y, Size.cx, Size.cy, hMemDC, 0, 0, SRCINVERT);
      // Displays text in black colour in output DC
      // Background mode is transparent
      COLORREF TextColor = SetTextColor(hdc, RGB(0, 0, 0));
      int BkMode = SetBkMode(hdc, TRANSPARENT);
      TextOut(hdc, x, y, text, lstrlen(text));
      SetBkMode(hdc, BkMode);
      SetTextColor(hdc, TextColor);
      // Applies mask to output DC again
      BitBlt(hdc, x, y, Size.cx, Size.cy, hMemDC, 0, 0, SRCINVERT);
      // De-selects bitmap from memory DC
      SelectObject(hMemDC, hOldBitmap);
      // Deletes bitmap
      DeleteObject(hBitmap);
    }
    // Deletes memory DC
    DeleteDC(hMemDC);
  }
}

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
        DrawBackgroundPattern(m_hDlg, hdc);

        RECT rcGrid = {
            20, 20, 260, 260
        };

        COLORREF clrs[] = {
            huys::red,
            huys::green,
            huys::blue,
            huys::white
        };
        GradientFill(hdc, rcGrid, clrs);

        drawGrid(hdc, rc);
    }

private:
    void drawGrid(HDC &hdc, RECT &rc)
    {
        urc.SetStartPnt(20, 20);
        urc.SetEndPnt(260, 260);
        urc.setLineColor(huys::red);
        urc.Draw(hdc);

        ul.SetStartPnt(20, 80);
        ul.SetEndPnt(260, 80);
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

    huys::URectangle urc;
    huys::ULine ul;
};

UDLGAPP_T(UDialogContour, IDD_CONTOUR);

