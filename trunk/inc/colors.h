#ifndef COLORS_RGB_H
#define COLORS_RGB_H

/********************************************************************
 created:   2006/10/18
 filename:  colors.h
 author:    hu
 purpose:   Define the RGB Macro with colors' name.
            Simplify the use of COLORREF
 modified:  2008/09/30
*********************************************************************/

namespace huys
{

typedef COLORREF Color;

inline BYTE RValue(Color clr)
{
    return GetRValue(clr);
}

inline BYTE GValue(Color clr)
{
    return GetGValue(clr);
}

inline BYTE BValue(Color clr)
{
    return GetBValue(clr);
}



const COLORREF red        = RGB(0xFF, 0x00, 0x00);
const COLORREF green      = RGB(0x00, 0xFF, 0x00);
const COLORREF blue       = RGB(0x00, 0x00, 0xFF);
const COLORREF white      = RGB(0xFF, 0xFF, 0xFF);
const COLORREF black      = RGB(0x00, 0x00, 0x00);
const COLORREF yellow     = RGB(0xFF, 0xFF, 0x00);
const COLORREF cyan       = RGB(0x00, 0xFF, 0xFF); //
const COLORREF magenta    = RGB(0xFF, 0x00, 0xFF); //
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

const COLORREF babyblue       = RGB(0xE0, 0xFF, 0xFF);
const COLORREF beige          = RGB(0xF5, 0xF5, 0xDC);
const COLORREF bisque         = RGB(0xFF, 0xE4, 0xC4);
const COLORREF bistre         = RGB(0x3D, 0x2B, 0x1F);
const COLORREF blanchedalmond = RGB(0xFF, 0xEB, 0xCD);
const COLORREF blueviolet     = RGB(0x8A, 0x2B, 0xE2);
const COLORREF bole           = RGB(0x79, 0x44, 0x3B);
const COLORREF burlywood      = RGB(0xDE, 0xB8, 0x87);

const COLORREF cadetblue      = RGB(0x5F, 0x9E, 0xA0);
const COLORREF chartreuse     = RGB(0x7F, 0xFF, 0x00);
const COLORREF chocalate      = RGB(0xD2, 0x69, 0x1E);
const COLORREF snow           = RGB(0xFF, 0xFA, 0xFA);

const COLORREF lightpink      = RGB(0xFF, 0xB6, 0xC1);
const COLORREF crimson        = RGB(0xDC, 0x14, 0x3C);
const COLORREF lavenderblush  = RGB(0xFF, 0xF0, 0xF5);
const COLORREF palevioletred  = RGB(0xDB, 0x70, 0x93);
const COLORREF hotpink        = RGB(0xFF, 0x69, 0xB4);
const COLORREF deeppink       = RGB(0xFF, 0x14, 0x93);
const COLORREF mediumvioletred= RGB(0xC7, 0x15, 0x85);
const COLORREF orchid         = RGB(0xDA, 0x70, 0xD6);
const COLORREF thistle        = RGB(0xD8, 0xBF, 0xD8);
const COLORREF plum           = RGB(0xDD, 0xA0, 0xDD);
const COLORREF violet         = RGB(0xEE, 0x82, 0xEE);
const COLORREF fuchsia        = RGB(0xFF, 0x00, 0xFF);
const COLORREF darkmagenta    = RGB(0x8B, 0x00, 0x8B);
const COLORREF purple         = RGB(0x80, 0x00, 0x80);
const COLORREF mediumorchid   = RGB(0xBA, 0x55, 0xD3);
const COLORREF darkvoilet     = RGB(0x94, 0x00, 0xD3);
const COLORREF darkorchid     = RGB(0x99, 0x32, 0xCC);
const COLORREF indigo         = RGB(0x4B, 0x00, 0x82);

const COLORREF xpblue         = RGB(0x64, 0x95, 0xED);

const COLORREF darkgreen      = RGB(0x00, 0x64, 0x00);
const COLORREF seagreen       = RGB(0x2E, 0x8B, 0x57);

const COLORREF royalblue      = RGB(0x41, 0x69, 0xE1);
const COLORREF navy           = RGB(0x00, 0x00, 0x80);

};

#endif //COLORS_H
