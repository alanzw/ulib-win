// This is a C example that tests the IExample COM component (in IExample.dll).

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
    if (S_OK == CoInitialize(NULL))
    {
        // Get IExample.DLL's IClassFactory
        if ((hr = CoGetClassObject( CLSID_IExample,
                                    CLSCTX_INPROC_SERVER,
                                    NULL,
                                    IID_IClassFactory,
                                    (LPVOID *)&classFactory )) != S_OK)
        {
            MessageBox( NULL,
                        _T("Can't get IClassFactory"),
                        _T("CoGetClassObject error"),
                        MB_OK | MB_ICONEXCLAMATION );
        }
        else
        {
            // Create an IExample object
            if ((hr = classFactory->CreateInstance(
                    NULL,
                    IID_IExample,
                    (LPVOID *)&exampleObj)) != S_OK )
            {
                classFactory->Release();
                MessageBox( NULL,
                            _T("Can't create IExample object"),
                            _T("CreateInstance error"),
                            MB_OK|MB_ICONEXCLAMATION );
            }
            else
            {
                char buffer[80];

                // Release the IClassFactory. We don't need it now that we have the one
                // IExample we want
                classFactory->Release();

                // Call SetString to set the text "Hello world!"
                exampleObj->SetString("Hello world!");

                // Retrieve the string to make sure we get "Hello world!"
                exampleObj->GetString(buffer, sizeof(buffer));

                printf("string = %s (should be Hello world!)\n", buffer);

                // Release the IExample now that we're done with it
                exampleObj->Release();
                system("pause");
            }
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
