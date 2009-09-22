#ifndef U_STRING_H
#define U_STRING_H

namespace huys
{

namespace ADT
{

template <typename T>
class UString
{
public:
    UString(const T *sText)
    {

    }

    UString(const UString &s)
    {

    }

    ~UString()
    {

    }

    UString & operator=(const UString &s)
    {
        return *this;
    }

private:
    T *m_buf;
};

typedef UString<char> UStringAnsi;
typedef UString<wchar_t> UStringUnicode;

}; // namespace ADT


}; // namespace huys

#endif // U_STRING_H

