#ifndef U_PYTHON_H
#define U_PYTHON_H

#include <Python.h>

namespace huys
{

class UPython
{
public:
    UPython();
    ~UPython();

    bool initialize();

    bool finalize();

    bool isReady();

    PyObject *parse(const char* lpszText);
};

}; // namespace huys

#endif // U_PYTHON_H
