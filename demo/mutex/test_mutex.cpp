#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "umutex.h"
#include "udlgapp.h"
#include "umsg.h"

BEGIN_DLGAPP
    //
    showMsg(_T("hello!"));

    //
    showMsgFormat(_T("formated message"), _T("%d -- %d"), 22, 100);

    //
    showError(_T("Error:"));

    //
    showErrorByNum(22);

    //
    showCustomMsgBox(_T("Customized Message"));

END_DLGAPP

