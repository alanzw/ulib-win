#ifndef U_EDIT_H
#define U_EDIT_H

#include "ucontrol.h"

const int EDIT_MAX_LENGTH = 256;

class ULIB_API UEdit : public UControl
{
public:
    UEdit(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UEdit();
    //
    virtual BOOL create();

    BOOL setText(const TCHAR *lpText);
    int  getText(TCHAR *lpText);
    BOOL clear();

    //
    int getLineCount();

    //
    LONG scrollLineDown();

    //
    BOOL setLimitText(int n);
    BOOL limitText(int n);

    //
    BOOL addLine(LPCTSTR sLine);
protected:
private:
};

#endif // U_EDIT_H
