#include <windows.h>
#include <tchar.h>
#include <exception>
#include <new>

#include "uthunk32.h"

//
#ifndef HEAP_CREATE_ENABLE_EXECUTE
#define HEAP_CREATE_ENABLE_EXECUTE 0x00040000
#endif

//
void* volatile UThunk32Base::bytecodeHeap = NULL;
UMutex UThunk32Base::heapMutex;

UThunk32Base::UThunk32Base()
{
    // Double checked locking, guaranteed by Acquire/Release-semantics in Microsoft's
    // volatile-implementation.
    if( NULL == bytecodeHeap )
    {
        // boost::mutex::scoped_lock lock(heapMutex);
        UMutex::scoped_lock lock(heapMutex);
        if (bytecodeHeap == NULL)
        {
            bytecodeHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
            if(bytecodeHeap == NULL)
            {
                //throw std::exception("Heap creation failed!");
                throw 2222;
            }

            // Schedule the heap to be destroyed when the application terminates.
            // Until then, it will manage its own size.
            atexit(cleanupHeap);
        }
    }

    bytecode = reinterpret_cast<Bytecode*>(HeapAlloc(bytecodeHeap, 0, sizeof(Bytecode)));
    if (bytecode == NULL) 
    { 
        //throw std::exception("Bytecode allocation failed!");
        throw 3333;
    }
    new (bytecode) Bytecode;
}

UThunk32Base::~UThunk32Base()
{
    if (bytecode)
    {
        bytecode->~Bytecode();
        HeapFree(bytecodeHeap, 0, bytecode);
    }
}

void UThunk32Base::cleanupHeap()
{
    HeapDestroy(UThunk32Base::bytecodeHeap);
}

