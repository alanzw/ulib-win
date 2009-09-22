// This is a C example that tests the IExample3 COM component (in IExample3.dll).

#include <windows.h>
#include <objbase.h>
#include <stdio.h>

#define INITGUID

#include "IExample3.h"

int main(int argc, char **argv)
{
    IExample3        *exampleObj;
    IClassFactory    *classFactory;
    HRESULT            hr;

    // We must initialize OLE before we do anything with COM objects. NOTE:
    // some COM components, such as the IE browser engine, require that you
    // use OleInitialize() instead. But our COM component doesn't require this
    if (!CoInitialize(0))
    {
        // Get IExample3.DLL's IClassFactory
        if ((hr = CoGetClassObject(&CLSID_IExample3, CLSCTX_INPROC_SERVER, 0, &IID_IClassFactory, &classFactory)))
            MessageBox(0, "Can't get IClassFactory", "CoGetClassObject error", MB_OK|MB_ICONEXCLAMATION);
        else
        {
            // Create an IExample object
            if ((hr = classFactory->lpVtbl->CreateInstance(classFactory, 0, &IID_IExample3, &exampleObj)))
            {
                classFactory->lpVtbl->Release(classFactory);
                MessageBox(0, "Can't create IExample3 object", "CreateInstance error", MB_OK|MB_ICONEXCLAMATION);
            }
            else
            {

                // Release the IClassFactory. We don't need it now that we have the one
                // IExample3 we want
                classFactory->lpVtbl->Release(classFactory);






                //==========================================================================
                // STUDY THIS
                //==========================================================================
                {
                // Get the IDispatch object for the Ports collection, and stuff it into our variable "portsObj".
                // NOTE: What our DLL is really returning is a MyRealICollection object. But this app doesn't
                // know that. All we know here is that this object is an IDispatch. So its VTable has the 3
                // IUnknown functions (QueryInterface, AddRef, and Release) and the 4 IDispatch functions
                // (GetTypeInfoCount, GetTypeInfo, GetIDsOfNames, and Invoke). And that's all as far as we're
                // concerned. If the object has any other functions, we can't call them directly by referencing
                // lpVtbl. We have to call those extra functions indirectly via the Invoke function, as we'll
                // see below
                IDispatch            *portsObj;

                if ((hr = exampleObj->lpVtbl->GetPorts(exampleObj, &portsObj)))
                    MessageBox(0, "Can't get the Ports collection (IDispatch) object", "GetPorts error", MB_OK|MB_ICONEXCLAMATION);
                else
                {
                    VARIANT            ret;
                    ULONG            count, i;
                    DISPID            dispid;
                    OLECHAR            *funcName;

                    funcName = (OLECHAR *)L"Count";

                    // In order to call our collection's Count function, we have to do it indirectly by calling our
                    // collection's Invoke function. This is a pain in the ass. Thank you, Microsoft's Visual Basic
                    // team. First, we need to discover the DISPID (number) associated with the Count function. We
                    // do this by calling the collection's GetIDsOfNames function. We pass it a handle to a
                    // unicode string of the function name we want (ie, L"Count"). We also pass it a pointer to some
                    // variable where it returns the DISPID
                    if ((hr = portsObj->lpVtbl->GetIDsOfNames(portsObj, &IID_NULL, &funcName, 1, LOCALE_USER_DEFAULT, &dispid)))
                        MessageBox(0, "Can't get Count()'s DISPID", "GetIDsOfNames error", MB_OK|MB_ICONEXCLAMATION);
                    else
                    {
                        // Ok, now we have the DISPID for the Count function. It's stored in our variable "dispid".
                        //
                        // We can now call Invoke to indirectly call Count. This is going to be more of a pain in
                        // the ass because we have to initialize and pass a Visual Basic DISPPARAMS struct. And
                        // if the Count function is passed any args, we need to set this DISPPARMS rgvarg member to
                        // point to an array of VARIANT structs -- one per each arg we need to pass to Count.
                        // Fortunately, the Count function is passed no args, so we can simply zero out the DISPPARAMS
                        DISPPARAMS    dspp;

                        ZeroMemory(&dspp, sizeof(DISPPARAMS));

                        // Now we can actually call Invoke. We pass the DISPID for the Count function. This
                        // causes Invoke to call Count. We also pass a VARIANT where we want Count to return
                        // whatever our IDL file marks as its [out, retval]. In this case, the VARIANT is
                        // going to store a long value which is a count of how many items are in the collection.
                        // Because the Count function is marked as [propget] in our IDL, we need to indicate
                        // this by specifying the DISPATCH_PROPERTYGET flag. Oh, and we need to initialize the
                        // VARIANT first, by calling VariantInit
                        VariantInit(&ret);
                        if ((hr = portsObj->lpVtbl->Invoke(portsObj, dispid, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD|DISPATCH_PROPERTYGET, &dspp, &ret, 0, 0)))
                            MessageBox(0, "Can't get a count of items", "Invoke/Count error", MB_OK|MB_ICONEXCLAMATION);
                        else
                        {
                            VARIANT        args[1];

                            // Pull the count out of the return VARIANT. To be perfectly safe, we really should
                            // check the VARIANT's vt member to make sure it's VT_I4 (ie, a long was stuffed into
                            // it). After all, theoretically we could have been returned a VT_BSTR of L"100" (if
                            // there were 100 items). Then, we'd have use atoi on the VARIANT's bstrVal member.
                            // And after that, we'd have to SysFreeString our VARIANT's bstrVal. But, we're going
                            // to assume that the Count function didn't do something totally stupid (even though
                            // MS Visual Basic programmers thought this whole thing up, and if one of them had
                            // written our COM DLL... well...)
                            count = ret.lVal;

                            // If the Count function returned a VT_BSTR in our VARIANT, we'd have to SysFreeString
                            // our VARIANT's bstrVal. If the Count function returned a VT_UNKNOWN or VT_DISPATCH,
                            // we'd have to Release() on the VARIANT's punkVal. A call to VariantClear() does this
                            // for us. It also resets the VARIANT's vt member to VT_EMPTY so we can reuse it on
                            // the following call to Invoke (when we indirectly call the Item function)
                            VariantClear(&ret);

                            // Now let's do a loop, calling Item (indirectly via Invoke) to fetch (and display)
                            // each port name. Normally, we'd have to first call GetIDsOfNames again, this time
                            // passing the string L"Item" to get the DISPID for the Item function. But we know
                            // that it must have a DISPID of DISPID_VALUE. So we can simply pass DISPID_VALUE to
                            // Invoke in order to indirectly call Item().
                            //
                            // For passing any args to Item, we need a DISPPARAMS. We'll just reuse the same
                            // DISPPARAMS we used above.
                            //
                            // And we do have to pass 1 arg to Item (a long indicating which item we want
                            // fetched), so we need to fill in the array of 1 VARIANT struct. First we set the
                            // DISPPARMS cArgs member to 1 to indicate we're passing only one arg. Now
                            // we need to set its rgvarg member to point to a VARIANT. And finally, we need
                            // to fill in this VARIANT's lVal with the item number we wish to fetch, and set
                            // the vt member to VT_I4 (because we're stuffing a long value into the VARIANT).
                            VariantInit(&args[0]);
                            ZeroMemory(&dspp, sizeof(DISPPARAMS));
                            dspp.cArgs = 1;
                            dspp.rgvarg = &args[0];
                            args[0].vt = VT_I4;

                            for (i = 0; i < count; i++)
                            {
                                // Indicate (to the Item function) which item's port name we want
                                args[0].lVal = i;

                                // Call Invoke to indirectly call Item. We pass a DISPID of DISPID_VALUE. This
                                // causes Invoke to call Item. We also pass a VARIANT where we want Item to return
                                // the value of the item we requested. In this case, the VARIANT is going to return
                                // a BSTR of that item's port name. Because the Item function is marked as [propget]
                                // in our IDL, we need to indicate this by specifying the DISPATCH_PROPERTYGET flag
                                if ((hr = portsObj->lpVtbl->Invoke(portsObj, DISPID_VALUE, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD|DISPATCH_PROPERTYGET, &dspp, &ret, 0, 0)))
                                {
                                    MessageBox(0, "Can't get next item's port name", "Invoke/Item error", MB_OK|MB_ICONEXCLAMATION);
                                    break;
                                }
                                else
                                {
                                    // Display the port name. NOTE: It's a unicode string
                                    printf("%S\n", ret.bstrVal);

                                    // Call to VariantClear() to SysFreeString that BSTR, and also reset the
                                    // VARIANT's vt member to VT_EMPTY so we can reuse it on the next call
                                    // to Invoke (when we indirectly call the Item function again)
                                    VariantClear(&ret);
                                }
                            }
                        }
                    }

                    // Release the Ports collection IDispatch now that we're done with it
                    portsObj->lpVtbl->Release(portsObj);
                }
                }
                //==========================================================================



                // Release the IExample3 now that we're done with it
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
