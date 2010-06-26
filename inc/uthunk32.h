/*
 * =====================================================================================
 *
 *       Filename:  uthunk32.h
 *
 *    Description:  UThunk32 Class
 *
 *        Version:  1.0
 *        Created:  2009-10-12 22:18:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_THUNK32_H
#define U_THUNK32_H

#include "ulib.h"
#include "umutex.h"

class ULIB_API UThunk32Base
{
public:
    UThunk32Base();

    ~UThunk32Base();

    struct Bytecode
    {
#if defined(_MSV_VER)
    #pragma pack(push, 1)
        unsigned short stub1;      // lea ecx,
        unsigned long  nThisPtr;   // this
        unsigned char  stub2;      // mov eax,
        unsigned long  nJumpProc;  // pointer to destination function
        unsigned short stub3;      // jmp eax
    #pragma pack(pop)
#else
        unsigned short stub1;      // lea ecx,
        unsigned long  nThisPtr;   // this
        unsigned char  stub2;      // mov eax,
        unsigned long  nJumpProc;  // pointer to destination function
        unsigned short stub3;      // jmp eax
#endif
        Bytecode() :
        stub1      (0x0D8D),
        nThisPtr  (0),
        stub2      (0xB8),
        nJumpProc (0),
        stub3      (0xE0FF)
        {}
    } *bytecode;

    static void* volatile bytecodeHeap;
    //static boost::mutex heapMutex;
    static UMutex heapMutex;
private:
    // Thunks will not be copyable
    UThunk32Base(const UThunk32Base &); // not implemented
    const UThunk32Base& operator=(const UThunk32Base&); // not implemented

    //
    static void __cdecl cleanupHeap();
};

//template <typename R, typename T, typename TARG1, typename TARG2>
//struct remove_class
//{
//    typedef R (*funcType)(TARG1, TARG2);
//};

template <typename R>
struct remove_class
{

};

template <typename R, typename T, typename TARG1, typename TARG2>
struct remove_class<R (T::*)(TARG1, TARG2)>
{
    typedef R (*funcType)(TARG1, TARG2);
};

template <class T, typename memfun_type>
class UThunk32: UThunk32Base
{
public:
    //typedef R (__stdcall *callback_type)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename P));
    //typedef R (__thiscall T::*memfun_type)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename P));
    //typedef remove_class<memfun_type>::funcType callback_type;
    typedef int (__stdcall *callback_type)(int, int);

    // Initializes the thunk with the specified instance pointer
    // and destination member function
    void initializeThunk(T* pInstance, memfun_type memberFunction)
    {
        // This library is meant for 32 bit architecture only
        //BOOST_STATIC_ASSERT(sizeof(pInstance) == sizeof(ULONG));
#if defined(_MSV_VER)
#pragma warning(push)
#pragma warning(suppress: 4311)
        bytecode->nThisPtr = reinterpret_cast<ULONG>(pInstance);
        bytecode->nJumpProc = *reinterpret_cast<ULONG*>(&memberFunction);
#pragma warning(pop)
#endif

        // Flush instruction cache. May be required on some architectures which
        // don't feature strong cache coherency guarantees, though not on neither
        // x86, x64 nor AMD64.
        FlushInstructionCache(GetCurrentProcess(), bytecode, sizeof(Bytecode));
    }

    // Returns a callback which can be used by a method expecting a non-member function
    callback_type getCallback() const
    {
            //_ASSERT(bytecode->nThisPtr != 0);
            //_ASSERT(bytecode->nJumpProc != 0);
            return reinterpret_cast<callback_type>(bytecode);
    }
};


#endif // U_THUNK32_H

