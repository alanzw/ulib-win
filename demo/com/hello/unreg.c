// Unregisters the IExample.DLL COM component.
#define   INITGUID
#include <windows.h>
#include <tchar.h>
#include <initguid.h>
#include "hello.h"



static const TCHAR    OurDllName[] = _T("hello.dll");
static const TCHAR    ObjectDescription[] = _T("hello COM component");
static const TCHAR    FileDlgTitle[] = _T("Locate hello.dll to de-register it");
static const TCHAR    FileDlgExt[] = _T("DLL files\000*.dll\000\000");
static const TCHAR    ClassKeyName[] = _T("Software\\Classes");
static const TCHAR    CLSID_Str[] = _T("CLSID");
static const TCHAR    InprocServer32Name[] = _T("InprocServer32");
static const TCHAR    GUID_Format[] = _T("{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}");





/************************ stringFromCLSID() ***********************
 * Converts an object's GUID (array) to an ascii string (in a special
 * format where there are groups of ascii digits separated by a dash).
 * NOTE: Using wsprintf() avoids needing to load ole32.dll just to
 * call StringFromCLSID(). We're just doing the same thing it would do.
 */

static void stringFromCLSID(LPTSTR buffer, REFCLSID ri)
{
    wsprintf(buffer, &GUID_Format[0],
        ((REFCLSID)ri)->Data1, ((REFCLSID)ri)->Data2, ((REFCLSID)ri)->Data3, ((REFCLSID)ri)->Data4[0],
        ((REFCLSID)ri)->Data4[1], ((REFCLSID)ri)->Data4[2], ((REFCLSID)ri)->Data4[3],
        ((REFCLSID)ri)->Data4[4], ((REFCLSID)ri)->Data4[5], ((REFCLSID)ri)->Data4[6],
        ((REFCLSID)ri)->Data4[7]);
}

/************************** WinMain() ************************
 * Program Entry point
 */

int WINAPI WinMain(HINSTANCE hinstExe, HINSTANCE hinstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
    int                result;
    TCHAR            filename[MAX_PATH];

    {
    OPENFILENAME    ofn;

    // Pick out where our DLL is located. We need to know its location in
    // order to register it as a COM component
    lstrcpy(&filename[0], &OurDllName[0]);
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = &FileDlgExt[0];
    ofn.lpstrFile = &filename[0];
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = &FileDlgTitle[0];
    ofn.Flags = OFN_FILEMUSTEXIST|OFN_EXPLORER|OFN_PATHMUSTEXIST;
    result = GetOpenFileName(&ofn);
    }

    if (result > 0)
    {
        HKEY        rootKey;
        HKEY        hKey;
        HKEY        hKey2;
        TCHAR        buffer[39];

        stringFromCLSID(&buffer[0], (REFCLSID)(&CLSID_IExample));

        // Open "HKEY_LOCAL_MACHINE\Software\Classes"
        if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, &ClassKeyName[0], 0, KEY_WRITE, &rootKey))
        {
            // Delete our CLSID key and everything under it
            if (!RegOpenKeyEx(rootKey, &CLSID_Str[0], 0, KEY_ALL_ACCESS, &hKey))
            {
                if (!RegOpenKeyEx(hKey, &buffer[0], 0, KEY_ALL_ACCESS, &hKey2))
                {
                    RegDeleteKey(hKey2, &InprocServer32Name[0]);

                    RegCloseKey(hKey2);
                    RegDeleteKey(hKey, &buffer[0]);
                }

                RegCloseKey(hKey);
            }

            RegCloseKey(rootKey);
        }

        MessageBox(0, "De-registered hello.DLL as a COM component.", &ObjectDescription[0], MB_OK|MB_ICONEXCLAMATION);
    }

    return(0);
}
