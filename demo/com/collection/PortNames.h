#ifndef _PORTNAMES_H_
#define _PORTNAMES_H_

// Our ICollection VTable's GUID
// {F69902B1-20A0-4e99-97ED-CD671AA87B5C}
DEFINE_GUID(IID_ICollection, 0xf69902b1, 0x20a0, 0x4e99, 0x97, 0xed, 0xcd, 0x67, 0x1a, 0xa8, 0x7b, 0x5c);

// Our ICollection's VTable
#undef  INTERFACE
#define INTERFACE ICollection
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
    // IUnknown functions
    STDMETHOD  (QueryInterface)        (THIS_ REFIID, void **) PURE;
    STDMETHOD_ (ULONG, AddRef)        (THIS) PURE;
    STDMETHOD_ (ULONG, Release)        (THIS) PURE;
    // IDispatch functions
    STDMETHOD_ (ULONG, GetTypeInfoCount)(THIS_ UINT *) PURE;
    STDMETHOD_ (ULONG, GetTypeInfo)        (THIS_ UINT, LCID, ITypeInfo **) PURE;
    STDMETHOD_ (ULONG, GetIDsOfNames)    (THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
    STDMETHOD_ (ULONG, Invoke)            (THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;
    // Extra functions
    STDMETHOD (Count)                    (THIS_ long *);
    STDMETHOD (Item)                    (THIS_ long, VARIANT *);
    STDMETHOD (_NewEnum)                (THIS_ IUnknown **);  
};

// My IENUMITEM struct. This stores a value for one item
// in our linked list of items
typedef struct _IENUMITEM {
    struct _IENUMITEM    *next;        // A linked list of IENUMITEMs.
    VARIANT                value;        // This item's value
} IENUMITEM;

// Above, we use a macro which defines our ICollection struct
// as so:
//
// typedef struct {
//    ICollectionVtbl  *lpVtbl;
// } ICollection;
//
// In other words, it defines our ICollection to have nothing
// but a pointer to its VTable. And of course, every COM object must
// start with a pointer to its VTable.
//
// But we actually want to add some more members to our ICollection.
// So here we'll define a MyRealICollection that contains those extra
// members.
//
// We add a DWORD reference count so that this ICollection can be
// allocated (for example, we allocate an ICollection when our
// IExample3 object's GetPorts() calls allocPortsCollection) and
// later freed.
typedef struct {
    ICollectionVtbl        *lpVtbl;
    DWORD                count;
} MyRealICollection;

void            initCollectionTypeInfo(void);
void            freeCollectionTypeInfo(void);
IDispatch *        allocPortsCollection(void);
void            freePortsCollection(void);
HRESULT            initPortsCollection(void);

#endif // _PORTNAMES_H_
