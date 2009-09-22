#include <cstring>

#include "stringrep.h"

StringRep::StringRep(const char *s)
: _count(1)
{
    _rep = new char[strlen(s)+1];
    strcpy(_rep, s);
}

StringRep::~StringRep()
{
    delete [] _rep;
}

