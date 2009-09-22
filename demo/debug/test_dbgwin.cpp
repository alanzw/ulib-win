#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "udialogx.h"
#include "udlgapp.h"

using huys::UDialogBox;

class UDialogExt : public UDialogBox
{
public:
    UDialogExt(HINSTANCE hInst, UINT nID)
        : UDialogBox(hInst, nID)
    {}

    virtual BOOL onInit()
    {
        return TRUE;
    }
protected:
private:
};

UDLGAPP_T(UDialogExt, IDD_TEST);

