// This file contains functions for managing a list of "Port names".
#undef INITGUID

#include <windows.h>
#include <objbase.h>

#include "IExample3.h"
#include "PortNames.h"

//====================================================================
//======================= Global variables ===========================
//====================================================================

// Import this variable from IExample3.c
extern DWORD OutstandingObjects;

// A linked list of IENUMITEMs for our "Port names"
static IENUMITEM    *PortsList;

// Our ICollection's ITypeInfo. We need only one of these so
// we can make it global
static ITypeInfo    *CollectionTypeInfo;










//====================================================================
//======================= Helper functions ===========================
//====================================================================

// This is just a helper function to free up our PortsList. Called when our DLL unloads.
void freePortsCollection(void)
{
    IENUMITEM *item;

    item = PortsList;

    // Is there another item in the list?
    while ((item = PortsList))
    {
        // Get the next item *before* we delete this one
        PortsList = item->next;

        // If the item's value is an object, we need to Release()
        // it. If it's a BSTR, we need to SysFreeString() it.
        // VariantClear does this for us.
        VariantClear(&item->value);

        // Free the IENUMITEM.
        GlobalFree(item);
    }
}

// This is just a helper function to initialize our Ports list.
// Called when our DLL first loads.
HRESULT initPortsCollection(void)
{
    IENUMITEM *item;

    // Add a "Port 1" IENUMITEM to our list.
    if ((PortsList = item = (IENUMITEM *)GlobalAlloc(GMEM_FIXED, sizeof(IENUMITEM))))
    {
        item->next = 0;
        item->value.vt = VT_BSTR;
        if ((item->value.bstrVal = SysAllocString(L"Port 1")))
        {
            // Add a "Port 2" IENUMITEM to our list.
            if ((item->next = (IENUMITEM *)GlobalAlloc(GMEM_FIXED, sizeof(IENUMITEM))))
            {
                item = item->next;
                item->value.vt = VT_BSTR;
                if ((item->value.bstrVal = SysAllocString(L"Port 2")))
                {
                    // Add a "Port 3" IENUMITEM to our list.
                    if ((item->next = (IENUMITEM *)GlobalAlloc(GMEM_FIXED, sizeof(IENUMITEM))))
                    {
                        item = item->next;
                        item->next = 0;
                        item->value.vt = VT_BSTR;
                        if ((item->value.bstrVal = SysAllocString(L"Port 3")))
                            return(S_OK);
                    }
                }
            }
        }
    }

    // Error
    freePortsCollection();
    return(E_FAIL);
}

// This helper function initializes our ICollection TypeInfo.
// It's called when our DLL is loading.
void initCollectionTypeInfo(void)
{
    // We haven't yet created the ITypeInfo for our ICollection
    CollectionTypeInfo = 0;
}

// This helper function just Release()'s our ICollection TypeInfo.
// It's called when our DLL is unloading.
void freeCollectionTypeInfo(void)
{
    if (CollectionTypeInfo) CollectionTypeInfo->lpVtbl->AddRef(CollectionTypeInfo);
}








//====================================================================
//===================== ICollection functions ========================
//====================================================================

// ICollection's AddRef()
static STDMETHODIMP_(ULONG) Collection_AddRef(ICollection *this)
{
    return(++((MyRealICollection *)this)->count);
}

// ICollection's QueryInterface()
static STDMETHODIMP Collection_QueryInterface(ICollection *this, REFIID riid, void **ppvObj)
{
    if (!ppvObj) return(E_POINTER);

    // It can masquerade as an IUnknown or an IDispatch. Of course, it really has an
    // ICollection VTable, so if anyone passed the GUID we associated with that,
    // then we confirm it. But since no other entity beside our DLL should know
    // anything about an ICollection (ie, we tell the outside world only that it's
    // an IDispatch), we shouldn't need to check for this GUID
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IDispatch) /* || IsEqualIID(riid, &IID_ICollection) */)
    {
        *ppvObj = this;
        Collection_AddRef(this);
        return(NOERROR);
    }

    *ppvObj = 0;
    return(E_NOINTERFACE);
}

// ICollection's Release()
static STDMETHODIMP_(ULONG) Collection_Release(ICollection *this)
{
    if (--((MyRealICollection *)this)->count) return(((MyRealICollection *)this)->count);

    // NOTE: We don't free our PortsList. That list is freed by freePortsCollection()
    // when we're done with that list (ie, when our DLL terminates).

    // Free the MyRealICollection
    GlobalFree(this);

    // One less outstanding object
    InterlockedDecrement(&OutstandingObjects);
    
    return(0);
}

// This is just a helper function for the IDispatch functions below
static HRESULT loadCollectionTypeInfo(void)
{
    register HRESULT    hr;
    LPTYPELIB            pTypeLib;

    // Load our type library and get a ptr to its TYPELIB. Note: This does an
    // implicit pTypeLib->lpVtbl->AddRef(pTypeLib)
    if (!(hr = LoadRegTypeLib(&CLSID_TypeLib, 1, 0, 0, &pTypeLib)))
    {
        // Get Microsoft's generic ITypeInfo, giving it our loaded type library. We only
        // need one of these, and we'll store it in a global. Tell Microsoft this is for
        // our ICollection, by passing that GUID
        if (!(hr = pTypeLib->lpVtbl->GetTypeInfoOfGuid(pTypeLib, &IID_ICollection, &CollectionTypeInfo)))
        {
            // We no longer need the ptr to the TYPELIB now that we've given it
            // to Microsoft's generic ITypeInfo. Note: The generic ITypeInfo has done
            // a pTypeLib->lpVtbl->AddRef(pTypeLib), so this TYPELIB ain't going away
            // until the generic ITypeInfo does a pTypeLib->lpVtbl->Release too
            pTypeLib->lpVtbl->Release(pTypeLib);

            // Since caller wants us to return our ITypeInfo pointer,
            // we need to increment its reference count. Caller is
            // expected to Release() it when done
            CollectionTypeInfo->lpVtbl->AddRef(CollectionTypeInfo);
        }
    }

    return(hr);
}

// ICollection's GetTypeInfoCount()
static ULONG STDMETHODCALLTYPE GetTypeInfoCount(ICollection *this, UINT *pCount)
{
    // We do have type library information for our ICollection object, so return 1
    *pCount = 1;
    return(S_OK);
}

// ICollection's GetTypeInfo(). The caller uses this to get ahold of an ITypeInfo
// object that contains information about the extra functions in our ICollection's
// VTable (ie, Count, Item, and _NewEnum).
static ULONG STDMETHODCALLTYPE GetTypeInfo(ICollection *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
    register HRESULT    hr;

    // Assume an error
    *pTypeInfo = 0;

    if (itinfo)
        hr = ResultFromScode(DISP_E_BADINDEX);

    // If our ITypeInfo is already created, just increment its ref count. NOTE: We really should
    // store the LCID of the currently created ITypeInfo and compare it to what the caller wants.
    // If no match, unloaded the currently created ITypeInfo, and create the correct one. But since
    // we support only one language in our IDL file anyway, we'll ignore this
    else if (CollectionTypeInfo)
    {
        CollectionTypeInfo->lpVtbl->AddRef(CollectionTypeInfo);
        hr = 0;
    }
    else
    {
        // Load our type library and get Microsoft's generic ITypeInfo object. NOTE: We really
        // should pass the LCID to match, but since we support only one language in our IDL
        // file anyway, we'll ignore this
        hr = loadCollectionTypeInfo();
    }

    if (!hr) *pTypeInfo = CollectionTypeInfo;

    return(hr);
}

// ICollection's GetIDsOfNames(). This is used to get the DISPID for any
// one of the extra functions in our ICollection's VTable (ie, Count,
// Item, and _NewEnum).
// NOTE: Since we MUST give the DISPID of DISPID_VALUE to the Item()
// function, and we MUST give the DISPID of DISPID_NEWENUM to the
// _NewEnum() function, the app should already know these two DISPIDs.
// Therefore, there's no reason for him to call GetIDsOfNames to retrieve
// the DISPID of "Item". But if he did pass rgszNames = "Item", we're
// return  DISPID_VALUE. But if the host wants to call the Count()
// function, he'll definitely need GetIDsOfNames to retrieve Count()'s
// DISPID. That DISPID could be any positive number.
static ULONG STDMETHODCALLTYPE GetIDsOfNames(ICollection *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
    if (!CollectionTypeInfo)
    {
        register HRESULT    hr;

        if ((hr = loadCollectionTypeInfo())) return(hr);
    }

    // Let OLE32.DLL's DispGetIDsOfNames() do all the real work of using our type
    // library to look up the DISPID of the requested function in our ICollection
    return(DispGetIDsOfNames(CollectionTypeInfo, rgszNames, cNames, rgdispid));
}

// ICollection's Invoke(). This is used to indirectly call the extra
// functions in our ICollection's VTable (ie, Count, Item, and _NewEnum)
static ULONG STDMETHODCALLTYPE Invoke(ICollection *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
   // We implement only a "default" interface
   if (!IsEqualIID(riid, &IID_NULL))
      return(DISP_E_UNKNOWNINTERFACE);

    if (!CollectionTypeInfo)
    {
        register HRESULT    hr;

        if ((hr = loadCollectionTypeInfo())) return(hr);
    }

    // Let OLE32.DLL's DispInvoke() do all the real work of calling the appropriate
    // function in our object, and massaging the passed args into the correct format
    return(DispInvoke(this, CollectionTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr));
}

// ICollection's Count(). Gets a count of items in the collection
static STDMETHODIMP Count(ICollection *this, long *total)
{
    register DWORD        count;
    register IENUMITEM    *item;

    // Count how many items in the list by just walking it all
    // the way to the last IENUMITEM, incrementing a count for
    // each item
    count = 0;
    item = (IENUMITEM *)&PortsList;
    while ((item = item->next)) ++count;

    // Return the total
    *total = count;

    return(S_OK);
}

// ICollection's Item(). Retrieves the (value of the) item at the
// specified (0-based) index within the collection.
// NOTE: Our IDL file must assign this a DISPID of DISPID_VALUE
static STDMETHODIMP Item(ICollection *this, long index, VARIANT *ret)
{
    register IENUMITEM    *item;

    ret->vt = VT_EMPTY;

    // Locate to the item that the caller wants
    item = PortsList;
    while (item && index--) item = item->next;

    // Any more items left?
    if (item)
    {
        // Copy the item's value to the VARIANT that the caller supplied.
        // If what we're returning to the caller is an object, we must AddRef()
        // it on the caller's behalf. The caller is expected to Release() it
        // when done with it. If what we're returning is a BSTR, then we must
        // SysAllocString a copy of it. Caller is expected to SysFreeString it.
        // Other datatypes are simply copied to the caller's VARIANT as is.
        // VariantCopy() does all this for us. It also returns S_OK if all
        // went well
        return(VariantCopy(ret, &item->value));
    }

    // If no more items, return S_FALSE
    return(S_FALSE);
}

// ICollection's _NewEnum(). For now, we don't implement this.
static STDMETHODIMP _NewEnum(ICollection *this, IUnknown **enumObj)
{
    return(E_NOTIMPL);
}

// Our ICollection object's VTable. Note that although the Count, Item,
// and _NewEnum functions are in the VTable, we're not going to let
// any app know that. The app knows about only the first 7 (IUnknown and
// IDispatch) functions. To call Count, Item, or _NewEnum, the app will
// have to indirectly call those functions through the Invoke function,
// passing the correct DISPID
static const ICollectionVtbl ICollectionVTable = {Collection_QueryInterface,
Collection_AddRef,
Collection_Release,
GetTypeInfoCount,
GetTypeInfo,
GetIDsOfNames,
Invoke,
Count,
Item,
_NewEnum};









// This is just a helper function to allocate/initialize an IDispatch
// (really, a MyRealICollection) object for our PortsList.
IDispatch * allocPortsCollection(void)
{
    register MyRealICollection    *collection;

    // Allocate an ICollection object (actually, a MyRealICollection)
    if ((collection = (MyRealICollection *)GlobalAlloc(GMEM_FIXED, sizeof(MyRealICollection))))
    {
        // Store its VTable
        collection->lpVtbl = (ICollectionVtbl *)&ICollectionVTable;

        // AddRef it
        collection->count = 1;

        // Indicate another outstanding object since we'll be returning it
        // to an application which is expected to Release() it
        InterlockedIncrement(&OutstandingObjects);
    }

    // Return it as if it were an IDispatch (which it can be used as)
    return((IDispatch *)collection);
}
