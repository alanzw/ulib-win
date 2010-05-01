#ifndef U_EDIT_H
#define U_EDIT_H

#include "ucontrol.h"

const int EDIT_MAX_LENGTH = 256;

class ULIB_API UEdit : public UControl
{
public:
    UEdit(HWND hParent, UINT nResource, HINSTANCE hInst);
    UEdit() {}
    UEdit(UBaseWindow *pWndParent, UINT nID);
    
    virtual ~UEdit();
    //
    virtual BOOL create();

    //
    BOOL setText(const TCHAR *lpText);
    int  getText(TCHAR *lpText);
    BOOL clear();

    //
    int getLineCount();
    int getLineLength(int n = -1);
    
    //
    int getLine(int nIndex, LPTSTR lpBuffer, WORD wSize);

    //
    LONG scrollLineDown();
    LONG scrollLineUp();
    LONG scrollPageDown();
    LONG scrollPageUp();
    //
    BOOL setLimitText(int n);
    BOOL limitText(int n);

    //
    BOOL addLine(LPCTSTR sLine);
    
    //
    BOOL showBalloonTip(LPCWSTR pszTitle, LPCWSTR pszText, INT nIcon);
protected:
private:
};

#endif // U_EDIT_H
