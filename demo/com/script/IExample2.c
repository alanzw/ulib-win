// C source code to a simple COM object, compiled into an ordinary
// dynamic link library (DLL). This demonstrates adding IDispatch
// functions to our object (so that it can be used by scripting
// languages like VBscript and JScript).

#include <windows.h>
#include <objbase.h>
#include "IExample2.h"





// A count of how many objects our DLL has created (by some
// app calling our IClassFactory object's CreateInstance())
// which have not yet been Release()'d by the app
static DWORD        OutstandingObjects;

// A count of how many apps have locked our DLL via calling our
// IClassFactory object's LockServer()
static DWORD        LockCount;

// Where I store a pointer to my type library's TYPEINFO
static ITypeInfo    *MyTypeInfo;











// The IExample2 object ////////////////////////////////////////////////////////////

// In our .H file, we use a macro which defines our IExample2 struct
// as so:
//
// typedef struct {
//    IExample2Vtbl  *lpVtbl;
// } IExample2;
//
// In other words, the .H file defines our IExample2 to have nothing
// but a pointer to its VTable. And of course, every COM object must
// start with a pointer to its VTable.
//
// But we actually want to add some more members to our IExample2.
// We just don't want any app to be able to know about, and directly
// access, those members. So here we'll define a MyRealIExample2 that
// contains those extra members. The app doesn't know that we're
// really allocating and giving it a MyRealIExample2 object. We'll
// lie and tell it we're giving a plain old IExample2. That's ok
// because a MyRealIExample2 starts with the same VTable pointer.
//
// We add a DWORD reference count so that this IExample2
// can be allocated (which we do in our IClassFactory object's
// CreateInstance()) and later freed. And, we have an extra
// BSTR (pointer) string, which is used by some of the functions we'll
// add to IExample2
typedef struct {
    IExample2Vtbl    *lpVtbl;
    DWORD            count;
    BSTR            string;
} MyRealIExample2;

// Here are IExample2's functions.
//
// Every COM object's interface must have the 3 functions QueryInterface(),
// AddRef(), and Release().
//
// I also chose to add 2, extra functions to IExample2, which a program
// will call with the names GetString and SetString.

// IExample2's QueryInterface()
static HRESULT STDMETHODCALLTYPE QueryInterface(IExample2 *this, REFIID vTableGuid, void **ppv)
{
    // Check if the GUID matches IExample2 VTable's GUID. We gave the C variable name
    // IID_IExample2 to our VTable GUID. We can use an OLE function called
    // IsEqualIID to do the comparison for us. Also, if the caller passed a
    // IUnknown GUID, then we'll likewise return the IExample2, since it can
    // masquerade as an IUnknown object too. Finally, if the called passed a
    // IDispatch GUID, then we'll return the IExample2, since it can masquerade
    // as an IDispatch too
    if (!IsEqualIID(vTableGuid, &IID_IUnknown) && !IsEqualIID(vTableGuid, &IID_IExample2) && !IsEqualIID(vTableGuid, &IID_IDispatch))
    {
      // We don't recognize the GUID passed to us. Let the caller know this,
      // by clearing his handle, and returning E_NOINTERFACE.
      *ppv = 0;
      return(E_NOINTERFACE);
    }

    // Fill in the caller's handle
    *ppv = this;

    // Increment the count of callers who have an outstanding pointer to this object
    this->lpVtbl->AddRef(this);

    return(NOERROR);
}

// IExample's AddRef()
static ULONG STDMETHODCALLTYPE AddRef(IExample2 *this)
{
    // Increment IExample2's reference count, and return the updated value.
    // NOTE: We have to typecast to gain access to any data members. These
    // members are not defined in our .H file (so that an app can't directly
    // access them). Rather they are defined only above in our MyRealIExample2
    // struct. So typecast to that in order to access those data members
    return(++((MyRealIExample2 *)this)->count);
}

// IExample's Release()
static ULONG STDMETHODCALLTYPE Release(IExample2 *this)
{
    // Decrement IExample2's reference count. If 0, then we can safely free
    // this IExample2 now
    if (--((MyRealIExample2 *)this)->count == 0)
    {
        if (((MyRealIExample2 *)this)->string) SysFreeString(((MyRealIExample2 *)this)->string);
        GlobalFree(this);
        InterlockedDecrement(&OutstandingObjects);
        return(0);
    }
    return(((MyRealIExample2 *)this)->count);
}

// ================== The standard IDispatch functions

// This is just a helper function for the IDispatch functions below
static HRESULT loadMyTypeInfo(void)
{
    register HRESULT    hr;
    LPTYPELIB            pTypeLib;

    // Load our type library and get a ptr to its TYPELIB. Note: This does an
    // implicit pTypeLib->lpVtbl->AddRef(pTypeLib)
    if (!(hr = LoadRegTypeLib(&CLSID_TypeLib, 1, 0, 0, &pTypeLib)))
    {
        // Get Microsoft's generic ITypeInfo, giving it our loaded type library. We only
        // need one of these, and we'll store it in a global Tell Microsoft this is for
        // our IExample2's VTable, by passing that VTable's GUID
        if (!(hr = pTypeLib->lpVtbl->GetTypeInfoOfGuid(pTypeLib, &IID_IExample2, &MyTypeInfo)))
        {
            // We no longer need the ptr to the TYPELIB now that we've given it
            // to Microsoft's generic ITypeInfo. Note: The generic ITypeInfo has done
            // a pTypeLib->lpVtbl->AddRef(pTypeLib), so this TYPELIB ain't going away
            // until the generic ITypeInfo does a pTypeLib->lpVtbl->Release too
            pTypeLib->lpVtbl->Release(pTypeLib);

            // Since caller wants us to return our ITypeInfo pointer,
            // we need to increment its reference count. Caller is
            // expected to Release() it when done
            MyTypeInfo->lpVtbl->AddRef(MyTypeInfo);
        }
    }

    return(hr);
}

// IExample2's GetTypeInfoCount()
static ULONG STDMETHODCALLTYPE GetTypeInfoCount(IExample2 *this, UINT *pCount)
{
    *pCount = 1;
    return(S_OK);
}

// IExample2's GetTypeInfo()
static ULONG STDMETHODCALLTYPE GetTypeInfo(IExample2 *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
    register HRESULT    hr;

    // Assume an error
    *pTypeInfo = 0;

    if (itinfo)
        hr = ResultFromScode(DISP_E_BADINDEX);

    // If our ITypeInfo is already created, just increment its ref count. NOTE: We really should
    // store the LCID of the currently created TYPEINFO and compare it to what the caller wants.
    // If no match, unloaded the currently created TYPEINFO, and create the correct one. But since
    // we support only one language in our IDL file anyway, we'll ignore this
    else if (MyTypeInfo)
    {
        MyTypeInfo->lpVtbl->AddRef(MyTypeInfo);
        hr = 0;
    }
    else
    {
        // Load our type library and get Microsoft's generic ITypeInfo object. NOTE: We really
        // should pass the LCID to match, but since we support only one language in our IDL
        // file anyway, we'll ignore this
        hr = loadMyTypeInfo();
    }

    if (!hr) *pTypeInfo = MyTypeInfo;

    return(hr);
}

// IExample2's GetIDsOfNames()
static ULONG STDMETHODCALLTYPE GetIDsOfNames(IExample2 *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
    if (!MyTypeInfo)
    {
        register HRESULT    hr;

        if ((hr = loadMyTypeInfo())) return(hr);
    }

    // Let OLE32.DLL's DispGetIDsOfNames() do all the real work of using our type
    // library to look up the DISPID of the requested function in our object
    return(DispGetIDsOfNames(MyTypeInfo, rgszNames, cNames, rgdispid));
}

// IExample2's Invoke()
static ULONG STDMETHODCALLTYPE Invoke(IExample2 *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
   // We implement only a "default" interface
   if (!IsEqualIID(riid, &IID_NULL))
      return(DISP_E_UNKNOWNINTERFACE);

    // We need our type lib's TYPEINFO (to pass to DispInvoke)
    if (!MyTypeInfo)
    {
        register HRESULT    hr;

        if ((hr = loadMyTypeInfo())) return(hr);
    }

    // Let OLE32.DLL's DispInvoke() do all the real work of calling the appropriate
    // function in our object, and massaging the passed args into the correct format
    return(DispInvoke(this, MyTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr));
}

// ================== The following are my own extra functions added to IExample

// IExample2's SetString(). This copies the passed string to IExample2's
// string[]
static HRESULT STDMETHODCALLTYPE SetString(IExample2 *this, BSTR str)
{
   // Make sure that caller passed a buffer
   if (!str) return(E_POINTER);

   // First, free any old BSTR we allocated
   if (((MyRealIExample2 *)this)->string) SysFreeString(((MyRealIExample2 *)this)->string);

   // Make a copy of the caller's string and save this BSTR
   if (!(((MyRealIExample2 *)this)->string = SysAllocStringLen(str, SysStringLen(str))))
        return(E_OUTOFMEMORY);

    return(NOERROR);
}

// IExample2's GetString(). This retrieves IExample2's string[],
// and stores its contents in a buffer passed by the caller
static HRESULT STDMETHODCALLTYPE GetString(IExample2 *this, BSTR *buffer)
{
    // Make sure that caller passed a handle
    if (!buffer) return(E_POINTER);

    // Create a copy of our string. The caller is responsible for freeing it
    if (!(*buffer = SysAllocString(((MyRealIExample2 *)this)->string)))
        return(E_OUTOFMEMORY);

    return(NOERROR);
}

// Here's IExample2's VTable. It never changes so we can declare it
// static
static const IExample2Vtbl IExample2_Vtbl = {QueryInterface,
AddRef,
Release,
GetTypeInfoCount,
GetTypeInfo,
GetIDsOfNames,
Invoke,
SetString,
GetString};











// The IClassFactory object ///////////////////////////////////////////////////////

// Since we only ever need one IClassFactory object, we declare
// it static. The only requirement is that we ensure any
// access to its members is thread-safe
static IClassFactory    MyIClassFactoryObj;

// IClassFactory's AddRef()
static ULONG STDMETHODCALLTYPE classAddRef(IClassFactory *this)
{
    // Someone is obtaining my IClassFactory, so inc the count of
    // pointers that I've returned which some app needs to Release()
    InterlockedIncrement(&OutstandingObjects);

    // Since we never actually allocate/free an IClassFactory (ie, we
    // use just 1 static one), we don't need to maintain a separate
    // reference count for our IClassFactory. We'll just tell the caller
    // that there's at least one of our IClassFactory objects in existance
    return(1);
}

// IClassFactory's QueryInterface()
static HRESULT STDMETHODCALLTYPE classQueryInterface(IClassFactory *this, REFIID factoryGuid, void **ppv)
{
    // Make sure the caller wants either an IUnknown or an IClassFactory.
    // In either case, we return the same IClassFactory pointer passed to
    // us since it can also masquerade as an IUnknown
    if (IsEqualIID(factoryGuid, &IID_IUnknown) || IsEqualIID(factoryGuid, &IID_IClassFactory))
    {
        // Call my IClassFactory's AddRef
        this->lpVtbl->AddRef(this);

        // Return (to the caller) a ptr to my IClassFactory
        *ppv = this;

        return(NOERROR);
    }

    // We don't know about any other GUIDs
    *ppv = 0;
    return(E_NOINTERFACE);
}

// IClassFactory's Release()
static ULONG STDMETHODCALLTYPE classRelease(IClassFactory *this)
{
    // One less object that an app has not yet Release()'ed
    return(InterlockedDecrement(&OutstandingObjects));
}

// IClassFactory's CreateInstance() function. It is called by
// someone who has a pointer to our IClassFactory object and now
// wants to create and retrieve a pointer to our IExample2
static HRESULT STDMETHODCALLTYPE classCreateInstance(IClassFactory *this, IUnknown *punkOuter, REFIID vTableGuid, void **objHandle)
{
    HRESULT                hr;
    register IExample2    *thisobj;

    // Assume an error by clearing caller's handle
    *objHandle = 0;

    // We don't support aggregation in this example
    if (punkOuter)
        hr = CLASS_E_NOAGGREGATION;
    else
    {
        // Allocate our IExample2 object (actually a MyRealIExample2)
        if (!(thisobj = (IExample2 *)GlobalAlloc(GMEM_FIXED, sizeof(MyRealIExample2))))
            hr = E_OUTOFMEMORY;
        else
        {
            // Store IExample2's VTable in the object
            thisobj->lpVtbl = (IExample2Vtbl *)&IExample2_Vtbl;

            // Increment the reference count so we can call Release() below and
            // it will deallocate only if there is an error with QueryInterface()
            ((MyRealIExample2 *)thisobj)->count = 1;

            // Initialize any other members we added to the IExample2. We added
            // a string member
            ((MyRealIExample2 *)thisobj)->string = 0;

            // Fill in the caller's handle with a pointer to the IExample we just
            // allocated above. We'll let IExample2's QueryInterface do that, because
            // it also checks the GUID the caller passed, and also increments the
            // reference count (to 2) if all goes well
            hr = IExample2_Vtbl.QueryInterface(thisobj, vTableGuid, objHandle);

            // Decrement reference count. NOTE: If there was an error in QueryInterface()
            // then Release() will be decrementing the count back to 0 and will free the
            // IExample for us. One error that may occur is that the caller is asking for
            // some sort of object that we don't support (ie, it's a GUID we don't recognize)
            IExample2_Vtbl.Release(thisobj);

            // If success, inc static object count to keep this DLL loaded
            if (!hr) InterlockedIncrement(&OutstandingObjects);
        }
    }

    return(hr);
}

// IClassFactory's LockServer(). It is called by someone
// who wants to lock this DLL in memory
static HRESULT STDMETHODCALLTYPE classLockServer(IClassFactory *this, BOOL flock)
{
    if (flock) InterlockedIncrement(&LockCount);
    else InterlockedDecrement(&LockCount);

    return(NOERROR);
}

// IClassFactory's VTable
static const IClassFactoryVtbl IClassFactory_Vtbl = {classQueryInterface,
classAddRef,
classRelease,
classCreateInstance,
classLockServer};














// Miscellaneous functions ///////////////////////////////////////////////////////

/************************ DllGetClassObject() ***********************
 * This is called by the OLE functions CoGetClassObject() or
 * CoCreateInstance() in order to get our DLL's IClassFactory object
 * (and return it to someone who wants to use it to get ahold of one
 * of our IExample objects). Our IClassFactory's CreateInstance() can
 * be used to allocate/retrieve our IExample object.
 *
 * NOTE: After we return the pointer to our IClassFactory, the caller
 * will typically call its CreateInstance() function.
 */
__declspec(dllexport)
HRESULT PASCAL DllGetClassObject(REFCLSID objGuid, REFIID factoryGuid, void **factoryHandle)
{
    register HRESULT        hr;

    // Check that the caller is passing our IExample GUID. That's the
    // only object our DLL implements
    if (IsEqualCLSID(objGuid, &CLSID_IExample2))
    {
        // Fill in the caller's handle with a pointer to our IClassFactory object.
        // We'll let our IClassFactory's QueryInterface do that, because it also
        // checks the IClassFactory GUID and does other book-keeping
        hr = classQueryInterface(&MyIClassFactoryObj, factoryGuid, factoryHandle);
    }
    else
    {
        // We don't understand this GUID. It's obviously not for our DLL.
        // Let the caller know this by clearing his handle and returning
        // CLASS_E_CLASSNOTAVAILABLE
        *factoryHandle = 0;
        hr = CLASS_E_CLASSNOTAVAILABLE;
    }

    return(hr);
}





/************************ DllCanUnloadNow() ***********************
 * This is called by some OLE function in order to determine
 * whether it is safe to unload our DLL from memory.
 *
 * RETURNS: S_OK if safe to unload, or S_FALSE if not.
 */
__declspec(dllexport)
HRESULT PASCAL DllCanUnloadNow(void)
{
    // If someone has retrieved pointers to any of our objects, and
    // not yet Release()'ed them, then we return S_FALSE to indicate
    // not to unload this DLL. Also, if someone has us locked, return
    // S_FALSE
    return((OutstandingObjects | LockCount) ? S_FALSE : S_OK);
}





/************************** DllMain() **************************
 * Called by OS when this DLL is loaded or unloaded.
 */

BOOL WINAPI DllMain(HINSTANCE instance, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // No TypeInfo yet loaded
            MyTypeInfo = 0;

            // Clear global counts
            OutstandingObjects = LockCount = 0;

            // Initialize my IClassFactory with the pointer to its vtable
            MyIClassFactoryObj.lpVtbl = (IClassFactoryVtbl *)&IClassFactory_Vtbl;

            // We don't need to do any thread initialization
            DisableThreadLibraryCalls(instance);
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            // Release any TYPEINFO that my IDispatch functions got
            if (MyTypeInfo) MyTypeInfo->lpVtbl->Release(MyTypeInfo);
        }
    }

    return(1);
}
