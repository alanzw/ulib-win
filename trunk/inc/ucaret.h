#ifndef U_CARET_H
#define U_CARET_H

#include "uobject.h"

class ULIB_API UCaret : public UUserObject
{
public:
    UCaret();
    ~UCaret();
    
    bool create(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight);
    
    bool destroy();
    
    void setPos(int x, int y);
    
    void show(HWND hWnd);
    
    void hide(HWND hWnd);
};

#endif // U_CARET_H
