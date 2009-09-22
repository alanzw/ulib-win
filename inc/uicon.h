#ifndef U_ICON_H
#define U_ICON_H

#include "uobject.h"

class ULIB_API UIcon : public UUserObject
{
public:
    UIcon();
    ~UIcon();
    BOOL loadStandard(LPCSTR lpIconName);
    BOOL loadIconEx(HINSTANCE hInstance, LPCSTR lpIconName);
    BOOL loadIconEx(HINSTANCE hInstance, UINT nResId);

    bool destroy();

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
