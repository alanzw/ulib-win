#ifndef U_GDIPLUS_H
#define U_GDIPLUS_H

#include "ulib.h"

class ULIB_API UGDIPlusHelper
{
public:
    UGDIPlusHelper();
    ~UGDIPlusHelper();
private:
    class UGDIPlus_Stuff * ps;
};

class ULIB_API UGDIPlus_ImageUtil
{

};

#endif // U_GDIPLUS_H
