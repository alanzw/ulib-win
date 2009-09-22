// This is a C example that tests the IExample COM component (in IExample.dll).
#define   INITGUID
#include <windows.h>
#include <objbase.h>
#include <stdio.h>
#include <tchar.h>

#include "hello.h"

int main(int argc, char **argv)
{
    IExample *exampleObj;
    IClassFactory *classFactory;
    HRESULT hr;

    // We must initialize OLE before we do anything with COM objects. NOTE:
    // some COM components, such as the IE browser engine, require that you
    // use OleInitialize() instead. But our COM component doesn't require this
    if (S_OK == ::CoInitialize(NULL))
    {
        // Get IExample.DLL's IClassFactory
        if ((hr = ::CoGetClassObject(
                        &CLSID_IExample,
                        CLSCTX_INPROC_SERVER,
                        NULL,
                        &IID_IClassFactory,
                        (LPVOID *)&classFactory)) != S_OK )
        {
            MessageBox( NULL,
                        _T("Can't get IClassFactory"),
                        _T("CoGetClassObject error"),
                        MB_OK | MB_ICONEXCLAMATION);
        }
        else
        {
            // Create an IExample object
            if ((hr = classFactory->lpVtbl->CreateInstance(
                            classFactory,
                            NULL,
                            &IID_IExample,
                            (void **)&exampleObj)) != S_OK )
            {
                classFactory->lpVtbl->Release(classFactory);
                MessageBox( NULL,
                            _T("Can't create IExample object"),
                            _T("CreateInstance error"),
                            MB_OK | MB_ICONEXCLAMATION );
            }
            else
            {
                char buffer[80];

                // Release the IClassFactory. We don't need it now that we have the one
                // IExample we want
                classFactory->lpVtbl->Release(classFactory);

                // Call SetString to set the text "Hello world!"
                exampleObj->lpVtbl->SetString(exampleObj, "Hello world!");

                // Retrieve the string to make sure we get "Hello world!"
                exampleObj->lpVtbl->GetString(exampleObj, buffer, sizeof(buffer));

                printf("string = %s (should be Hello world!)\n", buffer);

                // Release the IExample now that we're done with it
                exampleObj->lpVtbl->Release(exampleObj);

                system("pause");
            }
        }

        // When finally done with OLE, free it
        ::CoUninitialize();
    }
    else
        MessageBox(0, "Can't initialize COM", "CoInitialize error", MB_OK|MB_ICONEXCLAMATION);

    return(0);
}

