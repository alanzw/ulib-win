// C source code to a simple COM object, compiled into an ordinary
// dynamic link library (DLL).
#define   INITGUID
#include <windows.h>
#include <objbase.h>
#include "hello.h"

// A count of how many objects our DLL has created (by some
// app calling our IClassFactory object's CreateInstance())
// which have not yet been Release()'d by the app
static DWORD        OutstandingObjects;

// A count of how many apps have locked our DLL via calling our
// IClassFactory object's LockServer()
static DWORD        LockCount;

// The IExample object ////////////////////////////////////////////////////////////

// In our .H file, we use a macro which defines our IExample struct
// as so:
//
// typedef struct {
//    IExampleVtbl  *lpVtbl;
// } IExample;
//
// In other words, the .H file defines our IExample to have nothing
// but a pointer to its VTable. And of course, every COM object must
// start with a pointer to its VTable.
//
// But we actually want to add some more members to our IExample.
// We just don't want any app to be able to know about, and directly
// access, those members. So here we'll define a MyRealIExample that
// contains those extra members. The app doesn't know that we're
// really allocating and giving it a MyRealIExample object. We'll
// lie and tell it we're giving a plain old IExample. That's ok
// because a MyRealIExample starts with the same VTable pointer.
//
// We add a DWORD reference count so that this IExample
// can be allocated (which we do in our IClassFactory object's
// CreateInstance()) and later freed. And, we have an extra
// 80 char buffer, which is used by some of the functions we'll
// add to IExample
typedef struct {
    IExampleVtbl    *lpVtbl;
    DWORD            count;
    char            buffer[80];
} MyRealIExample;

// Here are IExample's functions.
//
// Every COM object's interface must have the 3 functions QueryInterface(),
// AddRef(), and Release().
//
// I also chose to add 2, extra functions to IExample, which a program
// will call with the names GetString and SetString.

// IExample's QueryInterface()
static HRESULT STDMETHODCALLTYPE QueryInterface(IExample *this, REFIID vTableGuid, void **ppv)
{
    // Check if the GUID matches IExample VTable's GUID. We gave the C variable name
    // IID_IExample to our VTable GUID. We can use an OLE function called
    // IsEqualIID to do the comparison for us. Also, if the caller passed a
    // IUnknown GUID, then we'll likewise return the IExample, since it can
    // masquerade as an IUnknown object too
    if (!IsEqualIID(vTableGuid, &IID_IUnknown) && !IsEqualIID(vTableGuid, &IID_IExample))
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
static ULONG STDMETHODCALLTYPE AddRef(IExample *this)
{
    // Increment IExample's reference count, and return the updated value.
    // NOTE: We have to typecast to gain access to any data members. These
    // members are not defined in our .H file (so that an app can't directly
    // access them). Rather they are defined only above in our MyRealIExample
    // struct. So typecast to that in order to access those data members
    return(++((MyRealIExample *)this)->count);
}

// IExample's Release()
static ULONG STDMETHODCALLTYPE Release(IExample *this)
{
    // Decrement IExample's reference count. If 0, then we can safely free
    // this IExample now
    if (--((MyRealIExample *)this)->count == 0)
    {
        GlobalFree(this);
        InterlockedDecrement(&OutstandingObjects);
        return(0);
    }
    return(((MyRealIExample *)this)->count);
}

// ================== The following are my own extra functions added to IExample

// IExample's SetString(). This copies the passed string to IExample's
// buffer[]
static HRESULT STDMETHODCALLTYPE SetString(IExample *this, char *str)
{
    DWORD  i;

    // Make sure that caller passed a buffer
    if (!str) return(E_POINTER);

    // Copy the passed str to IExample's buffer
    i = lstrlen(str);
    if (i > 79) i = 79;
    CopyMemory(((MyRealIExample *)this)->buffer, str, i);
    ((MyRealIExample *)this)->buffer[i] = 0;

    return(NOERROR);
}

// IExample's GetString(). This retrieves IExample's buffer[],
// and stores its contents in a buffer passed by the caller
static HRESULT STDMETHODCALLTYPE GetString(IExample *this, char *buffer, DWORD length)
{
    DWORD  i;

    // Make sure that caller passed a buffer
    if (!buffer) return(E_POINTER);

    if (length)
    {
        // Let's copy IExample's buffer to the passed buffer
        i = lstrlen(((MyRealIExample *)this)->buffer);
        --length;
        if (i > length) i = length;
        CopyMemory(buffer, ((MyRealIExample *)this)->buffer, i);
        buffer[i] = 0;
    }
    return(NOERROR);
}

// Here's IExample's VTable. It never changes so we can declare it
// static
static const IExampleVtbl IExample_Vtbl = {
    QueryInterface,
    AddRef,
    Release,
    SetString,
    GetString
};


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
// wants to create and retrieve a pointer to our IExample
static HRESULT STDMETHODCALLTYPE classCreateInstance(IClassFactory *this, IUnknown *punkOuter, REFIID vTableGuid, void **objHandle)
{
    HRESULT hr;
    register IExample *thisobj;

    // Assume an error by clearing caller's handle
    *objHandle = 0;

    // We don't support aggregation in this example
    if (punkOuter)
        hr = CLASS_E_NOAGGREGATION;
    else
    {
        // Allocate our IExample object (actually a MyRealIExample)
        if (!(thisobj = (IExample *)GlobalAlloc(GMEM_FIXED, sizeof(MyRealIExample))))
            hr = E_OUTOFMEMORY;
        else
        {
            // Store IExample's VTable in the object
            thisobj->lpVtbl = (IExampleVtbl *)&IExample_Vtbl;

            // Increment the reference count so we can call Release() below and
            // it will deallocate only if there is an error with QueryInterface()
            ((MyRealIExample *)thisobj)->count = 1;

            // Initialize any other members we added to the IExample. We added
            // a buffer member
            ((MyRealIExample *)thisobj)->buffer[0] = 0;

            // Fill in the caller's handle with a pointer to the IExample we just
            // allocated above. We'll let IExample's QueryInterface do that, because
            // it also checks the GUID the caller passed, and also increments the
            // reference count (to 2) if all goes well
            hr = IExample_Vtbl.QueryInterface(thisobj, vTableGuid, objHandle);

            // Decrement reference count. NOTE: If there was an error in QueryInterface()
            // then Release() will be decrementing the count back to 0 and will free the
            // IExample for us. One error that may occur is that the caller is asking for
            // some sort of object that we don't support (ie, it's a GUID we don't recognize)
            IExample_Vtbl.Release(thisobj);

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
static const IClassFactoryVtbl IClassFactory_Vtbl = {
    classQueryInterface,
    classAddRef,
    classRelease,
    classCreateInstance,
    classLockServer
};


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
    if (IsEqualCLSID(objGuid, &CLSID_IExample))
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
 * Called by the Windows OS when this DLL is loaded or unloaded.
 */

BOOL WINAPI DllMain(HINSTANCE instance, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // Clear static counts
            OutstandingObjects = LockCount = 0;

            // Initialize my IClassFactory with the pointer to its VTable
            MyIClassFactoryObj.lpVtbl = (IClassFactoryVtbl *)&IClassFactory_Vtbl;

            // We don't need to do any thread initialization
            DisableThreadLibraryCalls(instance);
        }
    }

    return(1);
}

