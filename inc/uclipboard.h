#ifndef U_CLIPBOARD_H
#define U_CLIPBOARD_H

#include "ulib.h"

class ULIB_API UClipBoard
{
public:
    UClipBoard();
    ~UClipBoard();

    //
    static bool copyToClipborad(const TCHAR *text);

    //
    static bool getFromClipboard(TCHAR *buf);
protected:
private:
};

#endif // U_CLIPBOARD_H
