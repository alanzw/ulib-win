#ifndef U_STRING_REP_H
#define U_STRING_REP_H

class StringRep
{
public:
    StringRep(const char *s);

    ~StringRep();

    friend class String;
private:
    int _count;
    char *_rep;
};

#endif // U_STRING_REP_H
