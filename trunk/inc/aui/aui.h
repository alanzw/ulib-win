#ifndef U_AUI_H
#define U_AUI_H

#include "ustatic.h"
#include "uedit.h"
#include "ubutton.h"

#include "adt/uautoptr.h"

namespace AUI
{

typedef huys::ADT::UAutoPtr<UStatic> UStaticP;
typedef huys::ADT::UAutoPtr<UEdit> UEditP;
typedef huys::ADT::UAutoPtr<UButton> UButtonP;

}; // namespace AUI

#endif // U_AUI_H
