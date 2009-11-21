#ifndef U_STRING_H
#define U_STRING_H

#include <windows.h>
#include <tchar.h>
#include <cassert>

namespace huys
{

namespace ADT
{

template <typename T, size_t DEFAULT_BUFSIZE=10>
class UString
{
public:
    typedef size_t size_type;

public:
    UString()
    : m_pBuf(0), m_nBufSize(DEFAULT_BUFSIZE), m_nStrLength(0)
    {
        m_pBuf = new T[m_nBufSize+1];
        clear();
    }

    explicit UString(size_type nCap)
    : m_pBuf(0), m_nBufSize(nCap), m_nStrLength(0)
    {
        m_pBuf = new T[m_nBufSize+1];
        clear();
    }

    UString(const T *sText)
    : m_pBuf(0), m_nBufSize(DEFAULT_BUFSIZE), m_nStrLength(0)
    {
        if (0 == sText)
        {
            m_pBuf = new T[m_nBufSize+1];
            clear();
        }
        else
        {
            size_type len = lstrlen(sText);
            m_nBufSize = (len > m_nBufSize ? len : m_nBufSize);
            m_nStrLength = len;
            m_pBuf = new T[m_nBufSize+1];
            memcpy((void *)m_pBuf, sText, sizeof(T)*(m_nStrLength));
            m_pBuf[m_nStrLength] = 0;
        }
    }

    UString(const UString &s)
    : m_pBuf(0), m_nBufSize(s.m_nBufSize), m_nStrLength(s.m_nStrLength)
    {
        m_pBuf = new T[m_nBufSize+1];
        memset((void *)m_pBuf, 0, sizeof(T)*(m_nBufSize+1));
        memcpy((void *)m_pBuf, (const void *)s.m_pBuf, sizeof(T)*(m_nStrLength));
        m_pBuf[m_nStrLength] = 0;
    }

    ~UString()
    {
        if (m_pBuf)
        {
            delete [] m_pBuf;
        }
    }

    UString & operator = (const T * sText)
    {
        size_type len = lstrlen(sText);

        if (0 == len)
        {
            m_nStrLength = len;
            memset((void *)m_pBuf, 0, sizeof(T)*(m_nBufSize+1));
        }
        else if (len < m_nBufSize)
        {
            m_nStrLength = len;
            memcpy((void *)m_pBuf, sText, sizeof(T)*(m_nStrLength));
        }
        else
        {
            delete [] m_pBuf;
            m_nBufSize = len;
            m_nStrLength = len;
            m_pBuf = new T[m_nBufSize+1];
            memcpy((void *)m_pBuf, sText, sizeof(T)*(m_nStrLength));
        }

        m_pBuf[m_nStrLength] = 0;
        return *this;
    }

    UString & operator=(const UString &s)
    {
        if (this == &s)
        {
            return *this;
        }

        m_nStrLength = s.m_nStrLength;
        m_nBufSize = s.m_nBufSize;
        m_pBuf = new T[m_nBufSize+1];
        memcpy((void *)m_pBuf, (const void *)s.m_pBuf, sizeof(T)*(m_nStrLength));
        m_pBuf[m_nStrLength] = 0;
        return *this;
    }

    UString & operator += (const T * sText)
    {
        size_type len = lstrlen(sText);

        if (m_nStrLength + len <= m_nBufSize)
        {
            memcpy((void *)(m_pBuf+m_nStrLength), sText, len);
            m_nStrLength = m_nStrLength + len;
        }
        else
        {
            m_nBufSize = m_nStrLength + len;
            T * p = new T[m_nBufSize+1];
            memcpy((void *)p, (const void *)m_pBuf, sizeof(T)*m_nStrLength);
            memcpy((void *)(p+m_nStrLength), (const void *)sText, sizeof(T)*len);
            m_nStrLength += len;
            delete m_pBuf;
            m_pBuf = p;
        }
        m_pBuf[m_nStrLength] = 0;
        return *this;
    }

    UString & operator += (const UString & s)
    {
        if (m_nStrLength + s.m_nStrLength <= m_nBufSize)
        {
            memcpy((void *)(m_pBuf+m_nStrLength), (const void *)s.m_pBuf, sizeof(T)*s.m_nStrLength);
            m_nStrLength += s.m_nStrLength;
        }
        else
        {
            m_nBufSize = m_nStrLength + s.m_nStrLength;
            T * p = new T[m_nBufSize+1];
            memcpy((void *)p, (const void *)m_pBuf, sizeof(T)*m_nStrLength);
            memcpy((void *)(p+m_nStrLength), (const void *)s.m_pBuf, sizeof(T)*s.m_nStrLength);
            m_nStrLength += s.m_nStrLength;
            delete m_pBuf;
            m_pBuf = p;
        }
        m_pBuf[m_nStrLength] = 0;
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

    void reserve(size_type bufsize)
    {
        if (bufsize <= m_nBufSize) return;

        T * p = new T[bufsize];
        memcpy((void *)p, (const void *)m_pBuf, m_nStrLength);
        delete m_pBuf;
        m_pBuf = p;
        m_nBufSize = bufsize;
        m_pBuf[m_nStrLength] = 0;
    }

    const T * c_str() const
    {
        return m_pBuf;
    }

    size_type length() const
    {
        return m_nStrLength;
    }

    //
    size_type size() const
    {
        return m_nStrLength;
    }

    size_type buffer_size() const
    {
        return m_nBufSize;
    }

    bool empty() const
    {
        return 0 == m_nStrLength;
    }

    T & at(size_type index)
    {
        assert( index < length() );
        return m_pBuf[index];
    }

    const T & at(size_type index) const
    {
        assert( index < length() );
        return m_pBuf[index];
    }

    T & operator[](size_type index)
    {
        assert( index < length() );
        return m_pBuf[index];
    }

    const T & operator[](size_type index) const
    {
        assert( index < length() );
        return m_pBuf[index];
    }


    void clear()
    {
        m_nStrLength = 0;
        memset((void *)m_pBuf, 0, sizeof(T)*(m_nBufSize+1));
    }

    //
    size_type find(const T s)
    {
        for (size_type pos = 0; pos < m_nBufSize; ++pos)
        {
            if (s == m_pBuf[pos])
                return pos;
        }
        return -1;
    }

    UString substr(size_type start, size_type end)
    {
        if (start > end)
        {
            size_type temp = start;
            start = end;
            end = temp;
        }

        size_type m = (start < 0 ? 0 : start);
        size_type n = (end > m_nStrLength ? m_nStrLength : end);

        UString s(n-m);
        s.m_nStrLength = n-m;

        memcpy((void *)s.m_pBuf, (const void *)(m_pBuf+m), sizeof(T)*s.m_nStrLength);
        s.m_pBuf[s.m_nStrLength] = 0;
        return s;
    }
    
    UString & swap(size_type m, size_type n)
    {
        assert( m >=0 && m < length() && n >=0 && n < length() );
        T temp = m_pBuf[m];
        m_pBuf[m] = m_pBuf[n];
        m_pBuf[n] = temp;
        return *this;
    }
    
    UString & reverse()
    {
        size_type half_len = m_nStrLength / 2;
        
        for (int i = 0; i<=half_len; ++i)
        {
            this->swap(i, m_nStrLength-1-i);
        }
        return *this;
    }

    UString & operator << (const UString & in)
    {
        *this += in;
        return *this;
    }

    UString & operator << (const T * in)
    {
        *this += in;
        return *this;
    }

private:
    T *m_pBuf;
    size_type m_nBufSize;
    size_type m_nStrLength;
};

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator == (const UString<T, DEFAULT_BUFSIZE> & a, const UString<T, DEFAULT_BUFSIZE> & b)
{
    return    ( a.length() == b.length() )            // optimization on some platforms
        && ( lstrcmp(a.c_str(), b.c_str()) == 0 );    // actual compare
}

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator < (const UString<T, DEFAULT_BUFSIZE>  & a, const UString<T, DEFAULT_BUFSIZE>  & b)
{
    return lstrcmp(a.c_str(), b.c_str()) < 0;
}

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator != (const UString<T, DEFAULT_BUFSIZE> & a, const UString<T, DEFAULT_BUFSIZE>& b) { return !(a == b); }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator >  (const UString<T, DEFAULT_BUFSIZE> & a, const UString<T, DEFAULT_BUFSIZE> & b) { return b < a; }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator <= (const UString<T, DEFAULT_BUFSIZE> & a, const UString<T, DEFAULT_BUFSIZE> & b) { return !(b < a); }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator >= (const UString<T, DEFAULT_BUFSIZE> & a, const UString<T, DEFAULT_BUFSIZE> & b) { return !(a < b); }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator == (const UString<T, DEFAULT_BUFSIZE> & a, const char* b) { return strcmp(a.c_str(), b) == 0; }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator == (const char* a, const UString<T, DEFAULT_BUFSIZE> & b) { return b == a; }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator != (const UString<T, DEFAULT_BUFSIZE> & a, const char* b) { return !(a == b); }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline bool operator != (const char* a, const UString<T, DEFAULT_BUFSIZE> & b) { return !(b == a); }

template  <typename T, size_t DEFAULT_BUFSIZE>
inline UString<T, DEFAULT_BUFSIZE> operator + (const UString<T, DEFAULT_BUFSIZE> & a, const UString<T, DEFAULT_BUFSIZE> & b)
{
    UString<T, DEFAULT_BUFSIZE> tmp = a;
    tmp += b;
    return tmp;
}

template  <typename T, size_t DEFAULT_BUFSIZE>
inline UString<T, DEFAULT_BUFSIZE> operator + (const UString<T, DEFAULT_BUFSIZE> & a, const char* b)
{
    UString<T, DEFAULT_BUFSIZE> tmp = a;
    tmp += b;
    return tmp;
}

template  <typename T, size_t DEFAULT_BUFSIZE>
inline UString<T, DEFAULT_BUFSIZE> operator + (const char* a, const UString<T, DEFAULT_BUFSIZE> & b)
{
    UString<T, DEFAULT_BUFSIZE> tmp = a;
    tmp += b;
    return tmp;
}


typedef UString<char> UStringAnsi;
typedef UString<wchar_t> UStringUnicode;


}; // namespace ADT


}; // namespace huys

#endif // U_STRING_H

