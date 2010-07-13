// This is a C example that tests the IExample COM component (in IExample.dll).

#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <objbase.h>
#include <unknwn.h>
#include <stdio.h>
#include <tchar.h>

#include "hello.h"

#include "com/ucom.h"

int main(int argc, char **argv)
{
    IExample *exampleObj;
    IClassFactory *classFactory;
    HRESULT hr;

    // We must initialize OLE before we do anything with COM objects. NOTE:
    // some COM components, such as the IE browser engine, require that you
    // use OleInitialize() instead. But our COM component doesn't require this
    if (S_OK == CoInitialize(NULL))
    {
        // Create an IExample object
        if ((hr = huys::UCOM::HuCoCreateInstance(
                    "hello.dll",
                    CLSID_IExample,
                    NULL,
                    IID_IExample,
                    (LPVOID *)&exampleObj)) != S_OK )
        {
                //classFactory->Release();
                MessageBox( NULL,
                            _T("Can't create IExample object"),
                            _T("CreateInstance error"),
                            MB_OK|MB_ICONEXCLAMATION );
                return 0;
        }
        else
        {
                char buffer[80];

                // Release the IClassFactory. We don't need it now that we have the one
                // IExample we want
                //classFactory->Release();

                // Call SetString to set the text "Hello world!"
                exampleObj->SetString("Hello world!");

                // Retrieve the string to make sure we get "Hello world!"
                exampleObj->GetString(buffer, sizeof(buffer));

                printf("string = %s (should be Hello world!)\n", buffer);

                // Release the IExample now that we're done with it
                exampleObj->Release();
                system("pause");
        }

        // When finally done with OLE, free it
        CoUninitialize();
    }
    else
    {
        MessageBox( NULL,
                    _T("Can't initialize COM"),
                    _T("CoInitialize error"),
                    MB_OK|MB_ICONEXCLAMATION );
    }
    return 0;
}

