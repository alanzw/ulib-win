// This is a C example that tests the IExample2 COM component (in IExample2.dll).

#include <windows.h>
#include <objbase.h>
#include <stdio.h>
#include "IExample2.h"

int main(int argc, char **argv)
{
    IExample2        *exampleObj;
    IClassFactory    *classFactory;
    HRESULT            hr;

    // We must initialize OLE before we do anything with COM objects. NOTE:
    // some COM components, such as the IE browser engine, require that you
    // use OleInitialize() instead. But our COM component doesn't require this
    if (!CoInitialize(0))
    {
        // Get IExample.DLL's IClassFactory
        if ((hr = CoGetClassObject(&CLSID_IExample2, CLSCTX_INPROC_SERVER, 0, &IID_IClassFactory, &classFactory)))
            MessageBox(0, "Can't get IClassFactory", "CoGetClassObject error", MB_OK|MB_ICONEXCLAMATION);
        else
        {
            // Create an IExample object
            if ((hr = classFactory->lpVtbl->CreateInstance(classFactory, 0, &IID_IExample2, &exampleObj)))
            {
                classFactory->lpVtbl->Release(classFactory);
                MessageBox(0, "Can't create IExample object", "CreateInstance error", MB_OK|MB_ICONEXCLAMATION);
            }
            else
            {
                BSTR    str;

                // Release the IClassFactory. We don't need it now that we have the one
                // IExample we want
                classFactory->lpVtbl->Release(classFactory);

                // Alloc a BSTR and call SetString to set the text "Hello world!"
                if (!(str = SysAllocString(L"Hello world!")))
                    MessageBox(0, "Can't create BSTR", "SysAllocString error",  MB_OK|MB_ICONEXCLAMATION);

                else
                {
                    exampleObj->lpVtbl->SetString(exampleObj, str);

                    // Free the BSTR above
                    SysFreeString(str);

                    // Retrieve the string to make sure we get "Hello world!"
                    if (exampleObj->lpVtbl->GetString(exampleObj, &str))
                        MessageBox(0, "GetString failed", "GetString error",  MB_OK|MB_ICONEXCLAMATION);
                    else
                    {
                        MessageBoxW(0, str, L"GetString return", MB_OK);

                        // Free the BSTR that GetString allocated for us
                        SysFreeString(str);
                    }
                }

                // Release the IExample now that we're done with it
                exampleObj->lpVtbl->Release(exampleObj);
            }
        }

        // When finally done with OLE, free it
        CoUninitialize();
    }
    else
        MessageBox(0, "Can't initialize COM", "CoInitialize error", MB_OK|MB_ICONEXCLAMATION);

    return(0);
}
