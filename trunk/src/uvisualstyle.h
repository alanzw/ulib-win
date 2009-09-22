/*
 * =====================================================================================
 *
 *       Filename:  uvisualstyle.h
 *
 *    Description:  visual styles
 *
 *        Version:  1.0
 *        Created:  2009-7-19 10:39:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_VISUAL_STYLES_H
#define U_VISUAL_STYLES_H

#include "ulib.h"

#include "uxtheme.h"

namespace VisualStyles
{

inline BOOL isThemeActive()
{
    return ::IsThemeActive();
}

}; // namespace VisualStyles

#endif // U_VISUAL_STYLES_H

