#ifndef U_STRING_H
#define U_STRING_H

#include <string.h>
#include <windows.h>
#include <tchar.h>

namespace huys
{

namespace ADT
{

template <typename T>
class UString
{
public:
    UString()
    {
        m_nBufSize = 0;
        m_pBuf = 0;
        m_nStrLength = 0;
    }

    UString(const T *sText)
    {
        if (0 == sText)
        {
            m_nBufSize = 0;
            m_pBuf = 0;
        }
        else
        {
            m_nBufSize = lstrlen(sText);
            m_pBuf = new T[m_nBufSize+1];
            memcpy((void *)m_pBuf, sText, sizeof(T)*(m_nBufSize+1));
        }
    }

    UString(const UString &s)
    {
        if (0 == s.m_nBufSize)
        {
            m_nBufSize = 0;
            m_pBuf = 0;
        }
        else
        {
            m_nBufSize = s.m_nBufSize;
            m_pBuf = new T[m_nBufSize+1];
            memcpy((void *)m_pBuf, (const void *)s.m_pBuf, sizeof(T)*(m_nBufSize+1));
        }
    }

    ~UString()
    {
        if (m_pBuf)
        {
            delete m_pBuf;
        }
    }

    UString & operator=(const UString &s)
    {
        if (this == &s)
        {
            return *this;
        }

        if (0 == s.m_nBufSize)
        {
            m_nBufSize = 0;
            m_pBuf = 0;
        }
        else
        {
            m_nBufSize = s.m_nBufSize;
            m_pBuf = new T[m_nBufSize+1];
            memcpy((void *)m_pBuf, (const void *)s.m_pBuf, sizeof(T)*(m_nBufSize+1));
        }

        return *this;
    }

    operator T *()
    {
        return m_pBuf;
    }

    operator const T *()
    {
        return m_pBuf;
    }

    T * getBuffer()
    {
        return m_pBuf;
    }

    const T *getBuffer() const
    {
        return m_pBuf;
    }

    const T * c_str() const
    {
        return m_pBuf;
    }

    //
    int size() const
    {
        return m_nBufSize;
    }

    //
    int find(const T s)
    {
        for (int pos = 0; pos < m_nBufSize; ++pos)
        {
            if (s == m_pBuf[pos])
                return pos;
        }
        return -1;
    }

    UString substr(int start, int end)
    {
        if (start > end)
        {
            int temp = start;
            start = end;
            end = temp;
        }

        T t = m_pBuf[end];
        T *p = m_pBuf + start;
        m_pBuf[end] = '\0';
        UString s = p;
        m_pBuf[end] = t;
        return s;
    }
    //friend istream & operator>>( istream &is, UString<T> &Value );
    //friend ostream & operator<<( ostream &os, const UString<T> &Value );
private:
    T *m_pBuf;
    int m_nBufSize;
    int m_nStrLength;
};

typedef UString<char> UStringAnsi;
typedef UString<wchar_t> UStringUnicode;

}; // namespace ADT


}; // namespace huys

#endif // U_STRING_H

