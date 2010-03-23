/********************************************************************
 created:   2006/10/18
 filename:  colors.h
 author:    hu
 purpose:   Define the RGB Macro with colors' name.
            Simplify the use of COLORREF
 modified:  2008/09/30
*********************************************************************/

#ifndef COLORS_RGB_H
#define COLORS_RGB_H

namespace huys
{

typedef COLORREF Color;

const COLORREF red        = RGB(0xFF, 0x00, 0x00);
const COLORREF green      = RGB(0x00, 0xFF, 0x00);
const COLORREF blue       = RGB(0x00, 0x00, 0xFF);
const COLORREF white      = RGB(0xFF, 0xFF, 0xFF);
const COLORREF black      = RGB(0x00, 0x00, 0x00);
const COLORREF yellow     = RGB(0xFF, 0xFF, 0x00);
const COLORREF cyan       = RGB(0x00, 0xFF, 0xFF); // ¿×È¸À¶
const COLORREF magenta    = RGB(0xFF, 0x00, 0xFF); // Æ·ºì
const COLORREF brown      = RGB(0xA5, 0x2A, 0x2A);
const COLORREF gray       = RGB(0x80, 0x80, 0x80);
const COLORREF orange     = RGB(0xFF, 0xA5, 0x00);
const COLORREF tomato     = RGB(0xFF, 0x63, 0x47);
const COLORREF silver     = RGB(0xC0, 0xC0, 0xC0);
const COLORREF pink       = RGB(0xFF, 0xC0, 0xCB);
const COLORREF gold       = RGB(0xFF, 0xD7, 0x00);
const COLORREF skyblue    = RGB(0x87, 0xCE, 0xEB);

//
const COLORREF aliceblue      = RGB(0xF0, 0xF8, 0xFF);
const COLORREF antiquewhite   = RGB(0xFA, 0xEB, 0xD7);
const COLORREF aqua           = RGB(0x00, 0xFF, 0xFF);
const COLORREF aquamarine     = RGB(0x7F, 0xFF, 0xD4);
const COLORREF azure          = RGB(0xF0, 0xFF, 0xFF);
const COLORREF beige          = RGB(0xF5, 0xF5, 0xDC);
const COLORREF bisque         = RGB(0xFF, 0xE4, 0xC4);
const COLORREF blanchedalmond = RGB(0xFF, 0xEB, 0xCD);
const COLORREF blueviolet     = RGB(0x8A, 0x2B, 0xE2);
const COLORREF burlywood      = RGB(0xDE, 0xB8, 0x87);
const COLORREF cadetblue      = RGB(0x5F, 0x9E, 0xA0);
const COLORREF chartreuse     = RGB(0x7F, 0xFF, 0x00);
const COLORREF chocalate      = RGB(0xD2, 0x69, 0x1E); 
const COLORREF snow           = RGB(0xFF, 0xFA, 0xFA);

};

#endif //COLORS_H
