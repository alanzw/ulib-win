/*
 * =====================================================================================
 *
 *       Filename:  uxpm.h
 *
 *    Description:  Class UXpm
 *
 *        Version:  1.0
 *        Created:  2009-8-10 17:32:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

/*
 * XPM (X PixMap) is a format for storing/retrieving X pixmaps to/from files.
 *
 */

#ifndef U_XPM_H
#define U_XPM_H

typedef unsigned long Pixel;    /* Index into colormap */

struct UXpmAttribute
{

};

struct UXpmColorSymbol
{
    char *name;
    char *value;
    Pixel pixel;
};

struct UXpmColor
{
    char *string;
    char *symbols;
    char *m_color;
    char *g4_color;
    char *g_color;
    char *c_color;
};

struct UXpmImage
{
    unsigned int width;
    unsigned int height;
    unsigned int cpp;
    unsigned int ncolors;
    UXpmColor *colorTable;
    unsigned int *data;
};

class UXpm
{
public:
    UXpm(const char *sFilename);
    bool parse();
private:
    UXpmImage _image;
};

#endif // U_XPM_H

