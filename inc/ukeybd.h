#ifndef U_KEYBOARD_H
#define U_KEYBOARD_H

#include "ulib.h"

class ULIB_API UKeyboard
{
public:
    static void setNumLock(BOOL bState = TRUE);

    enum KeyBoradType {
        IBM_PC_XT_83_KEY        = 1,
        OLIVETTI_ICO_102_KEY    = 2,
        IBM_PC_AT_84_KEY        = 3,
        IBM_ENHANCED_101_2_KEY  = 4,
        NOKIA_1050              = 5,
        NOKIA_9104              = 6,
        JAPANESE                = 7
    };

    static int getType();
protected:
private:
};

class ULIB_API USendKey
{
public:
    enum {

    };
public:
    USendKey();
    ~USendKey();
private:
    bool activeTargetWnd(HWND hWnd);
};

#endif // U_KEYBOARD_H

