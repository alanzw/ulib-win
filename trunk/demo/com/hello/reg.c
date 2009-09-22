// Registers our IExample.DLL as a COM component.
#define   INITGUID
#include <windows.h>
#include <tchar.h>
#include <initguid.h>
#include "hello.h"

// These first 3 strings will change if you create your own object
static const TCHAR    OurDllName[] = _T("hello.dll");
static const TCHAR    ObjectDescription[] = _T("hello COM component");
static const TCHAR    FileDlgTitle[] = _T("Locate hello.dll to register it");

static const TCHAR    FileDlgExt[] = _T("DLL files\000*.dll\000\000");
static const TCHAR    ClassKeyName[] = _T("Software\\Classes");
static const TCHAR    CLSID_Str[] = _T("CLSID");
static const TCHAR    InprocServer32Name[] = _T("InprocServer32");
static const TCHAR    ThreadingModel[] = _T("ThreadingModel");
static const TCHAR    BothStr[] = _T("both");
static const TCHAR    GUID_Format[] =
    _T("{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}");

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

/************************ cleanup() ***********************
 * Removes all of the registry entries we create in WinMain.
 * This is called to do cleanup if there's an error
 * installing our DLL as a COM component.
 */

static void cleanup(void)
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
        HKEY        hkExtra;
        TCHAR        buffer[39];
        DWORD        disposition;

        // Assume an error
        result = 1;

        // Open "HKEY_LOCAL_MACHINE\Software\Classes"
        if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, &ClassKeyName[0], 0, KEY_WRITE, &rootKey))
        {
            // Open "HKEY_LOCAL_MACHINE\Software\Classes\CLSID"
            if (!RegOpenKeyEx(rootKey, &CLSID_Str[0], 0, KEY_ALL_ACCESS, &hKey))
            {
                // Create a subkey whose name is the ascii string that represents
                // our IExample2 object's GUID
                stringFromCLSID(&buffer[0], (REFCLSID)(&CLSID_IExample));
                if (!RegCreateKeyEx(hKey, &buffer[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey2, &disposition))
                {
                    // Set its default value to some "friendly" string that helps
                    // a user identify what this COM DLL is for. Setting this value
                    // is optional. You don't need to do it
                    RegSetValueEx(hKey2, 0, 0, REG_SZ, (const BYTE *)&ObjectDescription[0], sizeof(ObjectDescription));

                    // Create an "InprocServer32" key whose default value is the path of this DLL
                    if (!RegCreateKeyEx(hKey2, &InprocServer32Name[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkExtra, &disposition))
                    {
                        if (!RegSetValueEx(hkExtra, 0, 0, REG_SZ, (const BYTE *)&filename[0], lstrlen(&filename[0]) + 1))
                        {
                            // Create a "ThreadingModel" value set to the string "both" (ie, we don't need to restrict an
                            // application to calling this DLL's functions only from a single thread. We don't use global
                            // data in our IExample functions, so we're thread-safe)
                            if (!RegSetValueEx(hkExtra, &ThreadingModel[0], 0, REG_SZ, (const BYTE *)&BothStr[0], sizeof(BothStr)))
                            {
                                result = 0;
                                MessageBox(0, "Successfully registered hello.DLL as a COM component.", &ObjectDescription[0], MB_OK);
                            }
                        }

            // Close all keys we opened/created.

                        RegCloseKey(hkExtra);
                    }

                    RegCloseKey(hKey2);
                }

                RegCloseKey(hKey);
            }

            RegCloseKey(rootKey);
        }

        // If an error, make sure we clean everything up
        if (result)
        {
            cleanup();
            MessageBox(0, "Failed to register hello.DLL as a COM component.", &ObjectDescription[0], MB_OK|MB_ICONEXCLAMATION);
        }
    }

    return(0);
}
