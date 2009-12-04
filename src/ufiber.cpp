#include <windows.h>
#include <tchar.h>
#include <cassert>
#include "ufiber.h"

UFiber::UFiber(LPFIBER_START_ROUTINE lpRoutine, SIZE_T dwStackSize /*= 0*/)
{
    _fiber = ::CreateFiber(dwStackSize, lpRoutine, (LPVOID)this);
}

UFiber::UFiber()
: _fiber(0)
{}

UFiber::~UFiber()
{
    if(0 != _fiber)
    {
        ::DeleteFiber(_fiber);
    }
}

bool UFiber::create(LPFIBER_START_ROUTINE lpRoutine, SIZE_T dwStackSize /*= 0*/)
{
    assert(_fiber == NULL);
    _fiber = ::CreateFiber(dwStackSize, lpRoutine, (LPVOID)this);
    return _fiber != NULL;
}

bool UFiber::createEx(LPFIBER_START_ROUTINE lpRoutine, SIZE_T dwStackCommitSize /*= 0*/, SIZE_T dwStackReserveSize /*= 0*/)
{
    assert(_fiber == NULL);
    _fiber = ::CreateFiberEx(dwStackCommitSize, dwStackReserveSize, 0, lpRoutine, (LPVOID)this);
    return _fiber != NULL;
}

bool UFiber::convertThreadToFiber()
{
    assert(_fiber == NULL);
    _fiber = ::ConvertThreadToFiber(this);
    return _fiber != NULL;
}

void UFiber::attach(LPVOID p)
{
    assert(_fiber == NULL);
    _fiber = p;
}

LPVOID UFiber::detach()
{
    LPVOID result = _fiber; 
    _fiber = NULL;
    return result;
}

LPVOID UFiber::getFiber()
{
    return _fiber;
}

void UFiber::run()
{
    assert(_fiber != NULL);
    ::SwitchToFiber(_fiber);
}

