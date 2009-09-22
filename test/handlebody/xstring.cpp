#include <cstring>

#include "xstring.h"
#include "stringrep.h"

String::String()
: m_rep(new StringRep(""))
{
}

String::~String()
{
    if (--m_rep->_count <= 0)
    {
        delete m_rep;
    }
}

String::String(const String &s)
: m_rep(s.m_rep)
{
    m_rep->_count++;
}

String::String(const char *s)
: m_rep(new StringRep(s))
{
    m_rep->_count++;
}

String & String::operator = (const String &s)
{
    s.m_rep->_count++;
    if (--m_rep->_count <= 0)
    {
        delete m_rep;
    }
    m_rep = s.m_rep;

    return *this;
}

String & String::operator = (const char *s)
{
    if (--m_rep->_count <= 0)
    {
        delete m_rep;
    }
    m_rep = new StringRep(s);
    m_rep->_count++;

    return *this;
}

void String::purChar(char c)
{
    //
    int len = strlen(m_rep->_rep);
    char *newrep = new char[len+2];
    strcpy(newrep, m_rep->_rep);
    newrep[len] = c;
    newrep[len+1] = '\0';
    if (--m_rep->_count <= 0)
    {
        delete m_rep;
    }
    m_rep = new StringRep(newrep);
}

String::operator char *()
{
    return m_rep->_rep;
}

const char *String::toString() const
{
    return m_rep->_rep;
}

istream& operator>>(istream& is, String& s)
{
    is >> (char *)s;
    return is;
}

ostream& operator<<(ostream& os, const String& s)
{
    os <<  s.toString();
    return os;
}
