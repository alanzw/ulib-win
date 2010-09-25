#ifndef U_Books_H
#define U_Books_H

#include "ustatic.h"

class UBooks : public UStatic
{
    enum {
        ID_TIMER_UPDATE = 1111
    };
public:
    UBooks(HWND hParent, UINT nID, HINSTANCE hInst);
    UBooks();
    ~UBooks();
    UBooks(UBaseWindow *pWndParent, UINT nID = -1);
    
    //
    BOOL create();

    //
    BOOL onPaint();
    
    void Start();

	//
    void Pause();
    
private:


};

#endif // U_Books_H
