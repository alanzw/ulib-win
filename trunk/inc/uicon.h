#ifndef U_ICON_H
#define U_ICON_H

#include "uobject.h"

class ULIB_API UIcon : public UUserObject
{
public:
    UIcon();
    ~UIcon();
    BOOL loadStandard(LPCTSTR lpIconName);
    BOOL loadIconEx(HINSTANCE hInstance, LPCTSTR lpIconName);
    BOOL loadIconEx(HINSTANCE hInstance, UINT nResId);
    
    BOOL loadImage(HINSTANCE hInstance, UINT nID);

    virtual bool destroy();

    operator HICON ()
    {
        return (HICON)m_hObj;
    }

    BOOL draw(HWND hwnd, const int x, const int y);
    BOOL draw(HWND hwnd, const POINT *pt);
    HICON getHICON();
protected:

private:
    ICONINFO m_info;
    //HICON m_hObj;
};

#endif // U_ICON_H
