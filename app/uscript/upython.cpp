#include "upython.h"

namespace huys
{

UPython::UPython()
{
    this->initialize();
}

UPython::~UPython()
{
    this->finalize();
}

bool UPython::initialize()
{
    Py_Initialize();
    return Py_IsInitialized();
}

bool UPython::finalize()
{
    Py_Finalize();
    return !Py_IsInitialized();
}

bool UPython::isReady()
{
    return Py_IsInitialized();
}

PyObject* UPython::parse( const char* lpszText )
{
    return PyString_FromString(lpszText);
}

int UPython::doString(const char* sText)
{
    return PyRun_SimpleString(sText);
}

}; // namespace huys
