/*
 * =====================================================================================
 *
 *       Filename:  ex.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009-6-22 0:38:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include "ex.h"

class Control
{
public:
    template <typename T>
    void No(Type) {}

    const char *_GetClassName() {return "";}
    PropertyClass<Control, const char *, &Control::No, &Control::_GetClassName> mClassName;
};

