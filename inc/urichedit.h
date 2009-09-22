#ifndef U_RICHEDIT_H
#define U_RICHEDIT_H

#include "ucontrol.h"

class ULIB_API URichEdit : public UControl
{
public:
    URichEdit(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~URichEdit();
    //
    virtual BOOL create();
protected:
private:
};

#endif // U_RICHEDIT_H

