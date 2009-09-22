#ifndef U_STATIC_H
#define U_STATIC_H

#include "ucontrol.h"

class ULIB_API UStatic : public UControl
{
public:
    UStatic(HWND hParent, UINT nResource, HINSTANCE hInst);
    UStatic() {}
    virtual ~UStatic();
    virtual BOOL create();
    BOOL setText(const TCHAR *pText);
    BOOL setIcon(UINT nResID);
    HICON getIcon();
    BOOL setBitmap(HANDLE hImage);
    HANDLE getBitmap();
protected:
    const TCHAR *m_pText;
private:
};

#endif // U_STATIC_H

/*===================================================
 *   Static Control Styles
 *===========================
 *   SS_BITMAP
 *   SS_BLACKFRAME
 *   SS_BLACKRECT
 *   SS_CENTER
 *   SS_CENTERIMAGE
 *   SS_EDITCONTROL
 *   SS_ENDELLIPSIS
 *   SS_ENHMETAFILE
 *   SS_ETCHEDFRAME
 *   SS_ETCHEDHORZ
 *   SS_ETCHEDVERT
 *   SS_GRAYFRAME
 *   SS_GRAYRECT
 *   SS_ICON
 *   SS_LEFT
 *   SS_LEFTNOWORDWRAP
 *   SS_NOPREFIX
 *   SS_NOTIFY
 *   SS_OWNERDRAW
 *   SS_PATHELLIPSIS
 *   SS_REALSIZECONTROL
 *   SS_REALSIZEIMAGE
 *   SS_RIGHT
 *   SS_RIGHTJUST
 *   SS_SIMPLE
 *   SS_SUNKEN
 *   SS_TYPEMASK
 *   SS_WHITEFRAME
 *   SS_WHITERECT
 *   SS_WORDELLIPSIS
 *====================================================*/
