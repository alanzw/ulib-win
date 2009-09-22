// Registers our IExample2.DLL as a COM component.

#include <windows.h>
#include <tchar.h>
#include "IExample2.h"



static const TCHAR    OurDllName[] = _T("IExample2.dll");
static const TCHAR    OurProgID[] = _T("IExample2.object");
static const TCHAR    TypeLibName[] = _T("IExample2.tlb");
static const TCHAR    ObjectDescription[] = _T("IExample2 COM component");
static const TCHAR    FileDlgTitle[] = _T("Locate IExample2.dll to register it");
static const TCHAR    FileDlgExt[] = _T("DLL files\000*.dll\000\000");
static const TCHAR    ClassKeyName[] = _T("Software\\Classes");
static const TCHAR    CLSID_Str[] = _T("CLSID");
static const TCHAR    InprocServer32Name[] = _T("InprocServer32");
static const TCHAR    ThreadingModel[] = _T("ThreadingModel");
static const TCHAR    ProgIDName[] = "ProgID";
static const TCHAR    BothStr[] = _T("both");
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

    stringFromCLSID(&buffer[0], (REFCLSID)(&CLSID_IExample2));

    // Open "HKEY_LOCAL_MACHINE\Software\Classes"
    if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, &ClassKeyName[0], 0, KEY_WRITE, &rootKey))
    {
        // Delete ProgID subkey and everything under it
        if (!RegOpenKeyEx(rootKey, &OurProgID[0], 0, KEY_ALL_ACCESS, &hKey))
        {
            RegDeleteKey(hKey, &CLSID_Str[0]);
            RegCloseKey(hKey);
            RegDeleteKey(rootKey, &OurProgID[0]);

            // Delete our CLSID key and everything under it
            if (!RegOpenKeyEx(rootKey, &CLSID_Str[0], 0, KEY_ALL_ACCESS, &hKey))
            {
                if (!RegOpenKeyEx(hKey, &buffer[0], 0, KEY_ALL_ACCESS, &hKey2))
                {
                    RegDeleteKey(hKey2, &InprocServer32Name[0]);

                    RegDeleteKey(hKey2, &ProgIDName[0]);

                    RegCloseKey(hKey2);
                    RegDeleteKey(hKey, &buffer[0]);
                }

                RegCloseKey(hKey);
            }
        }

        RegCloseKey(rootKey);

        // Unregister type library
        UnRegisterTypeLib(&CLSID_TypeLib, 1, 0, LOCALE_NEUTRAL, SYS_WIN32);
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
            // For a script engine to call the OLE function CLSIDFromProgID() (passing
            // our registered ProgID in order to get our IExample2 object's GUID), then 
            // we need to create a subkey named with our IExample2 ProgID string. We've
            // decided to use the ProgID "IExample2.object"
            if (!RegCreateKeyEx(rootKey, &OurProgID[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey, &disposition))
            {
                // Set its default value to some "friendly" string that helps
                // a user identify what this COM DLL is for. Setting this value
                // is optional. You don't need to do it
                RegSetValueEx(hKey, 0, 0, REG_SZ, (const BYTE *)&ObjectDescription[0], sizeof(ObjectDescription));

                // Create a "CLSID" subkey whose default value is our IExample2 object's GUID (in ascii string format)
                if (!(disposition = RegCreateKeyEx(hKey, &CLSID_Str[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey2, &disposition)))
                {
                    stringFromCLSID(&buffer[0], (REFCLSID)(&CLSID_IExample2));
                    disposition = RegSetValueEx(hKey2, 0, 0, REG_SZ, (const BYTE *)&buffer[0], lstrlen(&buffer[0]) + 1);
                    RegCloseKey(hKey2);
                }
                RegCloseKey(hKey);

                if (!disposition)
                {
                    // Open "HKEY_LOCAL_MACHINE\Software\Classes\CLSID"
                    if (!RegOpenKeyEx(rootKey, &CLSID_Str[0], 0, KEY_ALL_ACCESS, &hKey))
                    {
                        // Create a subkey whose name is the ascii string that represents
                        // our IExample2 object's GUID
                        if (!RegCreateKeyEx(hKey, &buffer[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hKey2, &disposition))
                        {
                            // Set its default value to some "friendly" string that helps
                            // a user identify what this COM DLL is for. Setting this value
                            // is optional. You don't need to do it
                            RegSetValueEx(hKey2, 0, 0, REG_SZ, (const BYTE *)&ObjectDescription[0], sizeof(ObjectDescription));

                            // Create an "InprocServer32" key whose default value is the path of this DLL
                            if (!RegCreateKeyEx(hKey2, &InprocServer32Name[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkExtra, &disposition))
                            {
                                disposition = 1;
                                if (!RegSetValueEx(hkExtra, 0, 0, REG_SZ, (const BYTE *)&filename[0], lstrlen(&filename[0]) + 1))
                                {
                                    // Create a "ThreadingModel" value set to the string "both" (ie, we don't need to restrict an
                                    // application to calling this DLL's functions only from a single thread. We don't use global
                                    // data in our IExample2 functions, so we're thread-safe)
                                    disposition = RegSetValueEx(hkExtra, &ThreadingModel[0], 0, REG_SZ, (const BYTE *)&BothStr[0], sizeof(BothStr));
                                }

                                RegCloseKey(hkExtra);

                                // Create a "ProgID" subkey whose default value is our ProgID. This allows the app to call ProgIDFromCLSID()
                                if (!disposition && !(disposition = RegCreateKeyEx(hKey2, &ProgIDName[0], 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkExtra, &disposition)))
                                {
                                    disposition = RegSetValueEx(hkExtra, 0, 0, REG_SZ, (const BYTE *)&OurProgID[0], sizeof(OurProgID));
                                    RegCloseKey(hkExtra);
                                    if (!disposition) result = 0;
                                }
                            }

                            RegCloseKey(hKey2);
                        }

                        RegCloseKey(hKey);
                    }

                    // Register the type lib (which is assumed to be a .TLB file in the same dir as this DLL)
                    if (!result)
                    {
                        ITypeLib    *pTypeLib;
                        LPTSTR        str;

                        str = &filename[0] + lstrlen(&filename[0]);
                        while (str > &filename[0] && *(str - 1) != '\\') --str;
                        lstrcpy(str, &TypeLibName[0]);

                    #ifdef UNICODE
                        if (!(result = LoadTypeLib(&filename[0], &pTypeLib)))
                        {
                            result = RegisterTypeLib(pTypeLib, &filename[0], 0);
                            pTypeLib->lpVtbl->Release(pTypeLib);
                        }
                    #else
                        {
                        wchar_t        wbuffer[MAX_PATH];

                        MultiByteToWideChar(CP_ACP, 0, &filename[0], -1, &wbuffer[0], MAX_PATH);
                        if (!(result = LoadTypeLib(&wbuffer[0], &pTypeLib)))
                        {
                            result = RegisterTypeLib(pTypeLib, &wbuffer[0], 0);
                            pTypeLib->lpVtbl->Release(pTypeLib);
                        }
                        }
                    #endif
                    }

                    if (!result)
                        MessageBox(0, "Successfully registered IExample2.DLL as a COM component.", &ObjectDescription[0], MB_OK);
                }
            }

            RegCloseKey(rootKey);
        }

        // If an error, make sure we clean everything up
        if (result)
        {
            cleanup();
            MessageBox(0, "Failed to register IExample2.DLL as a COM component.", &ObjectDescription[0], MB_OK|MB_ICONEXCLAMATION);
        }
    }

    return(0);
}
