#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "umutex.h"
#include "udlgapp.h"
#include "umsg.h"

HANDLE createSingleInstance(LPCTSTR sName)
{
    HANDLE hMutex = NULL;
    hMutex = ::CreateMutex(NULL, TRUE, sName);
    switch (::GetLastError())
    {
    case ERROR_SUCCESS:
        showMsg(_T("OK!"));
        break;
    case ERROR_ALREADY_EXISTS:
        showError(_T("Already running!"));
        break;
    default:
        break;
    }
    return hMutex;
}

BEGIN_DLGAPP

    createSingleInstance(_T("xxxxxxxxxxx_mymutex_333331118"));

END_DLGAPP

