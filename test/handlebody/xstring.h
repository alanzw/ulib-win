#ifndef U_XSTRING_H
#define U_XSTRING_H

#include <iostream>
using std::istream;
using std::ostream;

class StringRep;

class String
{
public:
    String();
    String(const String &s);
    ~String();
    String(const char *s);
    String & operator = (const String &s);
    String & operator = (const char *s);

    void purChar(char c);

    friend istream& operator>>(istream& is, String& s);
    friend ostream& operator<<(ostream& os, const String& s);

private:
    StringRep *m_rep;

    operator char *();

    const char *toString() const;
};

#endif // U_XSTRING_H
