#include <windows.h>
#include <tchar.h>

#include <objbase.h>
#include <unknwn.h>

#include "com/ucom.h"

namespace huys
{
namespace UCOM
{

USmartObject::USmartObject (CLSID const & classId, bool running)
:_iUnk (0)
{
    HRESULT hr = S_OK;
    if (running)
    {
        ::GetActiveObject (classId, 0, & _iUnk);
    }
    if (_iUnk == 0)
    {
        hr = ::CoCreateInstance (
            classId,
            0,
            CLSCTX_SERVER,
            IID_IUnknown,
            (void**)& _iUnk);
    }
    if (FAILED (hr))
        throw HEx (hr, "Couldn't create instance");

}


void * USmartObject::acquireInterface (IID const & iid)
{
    void * p = 0;
    HRESULT hr = _iUnk->QueryInterface (iid, & p);
    if (FAILED (hr))
    {
        if (hr == E_NOINTERFACE)
            throw "No such interface";
        else
            throw HEx (hr, "Couldn't acquire interface");
    }
    return p;
}

// Can use name if found on the PATH
TypeLibrary::TypeLibrary (WCHAR * path)
{
    HRESULT hr = ::LoadTypeLib (path, &_iLib);
    if (FAILED (hr))
    {
        if (hr == TYPE_E_CANTLOADLIBRARY)
            _iLib = 0;
        else
            throw HEx (hr, "Couldn't load type library");
    }
    if (_iLib != 0)
    {
        hr = RegisterTypeLib (_iLib, path, 0);
        if (FAILED (hr))
            throw HEx (hr, "Couldn't register type library");
    }
}

void TypeLibrary::GetDocumentation (int idx, BString & name, BString & doc)
{
    _iLib->GetDocumentation (idx, name.GetPointer (), doc.GetPointer (), 0, 0);
}

ITypeInfo * TypeLibrary::GetTypeInfo (int idx)
{
    ITypeInfo * info;
    HRESULT hr = _iLib->GetTypeInfo (idx, &info);
    if (FAILED (hr))
        throw HEx (hr, "Couldn't get type info");
    return info;
}

ITypeInfo * TypeLibrary::GetTypeInfo (WCHAR * name)
{
    ITypeInfo * info = 0;
    MEMBERID id = 0;
    unsigned short cFound = 1; // look for first match only
    HRESULT hr = _iLib->FindName (name, 0, &info, &id, &cFound);
    if (hr == TYPE_E_ELEMENTNOTFOUND)
        throw "Name not found in library";
    if (FAILED (hr) || info == 0)
        throw HEx (hr, "Couldn't find type info by name");
    return info;
}

//
// Type Info
//

TypeInfo::TypeInfo (TypeLibrary & lib, int idx)
    : USmartFace<ITypeInfo> (lib.GetTypeInfo (idx))
{
    HRESULT hr = _i->GetTypeAttr (&_attr);
    if (FAILED (hr))
        throw HEx (hr, "Couldn't get type attributes");
}

TypeInfo::TypeInfo (TypeLibrary & lib, WCHAR * name)
    : USmartFace<ITypeInfo> (lib.GetTypeInfo (name))
{
    HRESULT hr = _i->GetTypeAttr (&_attr);
    if (FAILED (hr))
        throw HEx (hr, "Couldn't get type attributes");
}

void * TypeInfo::CreateInstance (IID const & iid)
{
    void * inst = 0;
    HRESULT hr = _i->CreateInstance (0, iid, &inst);
    if (FAILED (hr))
        throw HEx (hr, "Couldn't create instance from type info");
    return inst;
}

void TypeInfo::GetDocumentation (BString & name, BString & doc)
{
    _i->GetDocumentation (MEMBERID_NIL, name.GetPointer (), doc.GetPointer (), 0, 0);
}

}; // namespace UOle
}; // namespace huys

