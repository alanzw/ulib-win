#ifndef U_PRINTER_H
#define U_PRINTER_H

#include "ulib.h"

class ULIB_API UPrinter
{
public:

    static void listAll();

    static HDC getHDC();
};

#endif // U_PRINTER_H

