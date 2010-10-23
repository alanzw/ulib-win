#ifndef U_STRING_H
#define U_STRING_H

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <cstdio>

namespace huys
{

namespace ADT
{

template <typename T, size_t default_bufsize=10>
class UString
{
public:
    typedef size_t size_type;
public:
    UString()
    : m_pBuf(0), m_nBufSize(default_bufsize), m_nStrLength(0)
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
    : m_pBuf(0), m_nBufSize(default_bufsize), m_nStrLength(0)
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
    :m_pBuf(0), m_nBufSize(s.m_nBufSize), m_nStrLength(s.m_nStrLength)
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
            memcpy((void *)m_pBuf, sText, sizeof(T)*len);
            m_nStrLength = len;
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

        *this = s.c_str();

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

    void setLength(size_type size)
    {
        m_nStrLength = size;
        m_pBuf[m_nStrLength] = '\0';
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
    size_type find(const T s, size_type start = 0, size_type end = -1)
    {
        for (size_type pos = start; pos < m_nStrLength && pos < end; ++pos)
        {
            if (s == m_pBuf[pos])
                return pos;
        }
        return -1;
    }

    //
    size_type rfind(const T s)
    {
        for (size_type pos = m_nBufSize - 1 ; pos > 0; --pos)
        {
            if (s == m_pBuf[pos])
                return pos;
        }
        return -1;
    }

    size_type findCharSet(const T * chars, size_type n, size_type start = 0)
    {
        size_type pos = -1;
        size_type posNew = -1;
        for (size_type i=0; i<n; ++i)
        {
            if ((posNew = find(chars[i], start)) != -1)
            {
                if (pos == -1 || posNew < pos)
                {
                    pos = posNew;
                }
            }
        }

        return pos;
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

        UString s(n-m+1);
        s.m_nStrLength = n-m+1;

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

    UString & format(const T * fmt, ...)
    {
        T buf[4096];
        T * p = buf;
        size_type n;

        va_list args;
        va_start(args, fmt);
        n = _vsnprintf(buf, sizeof(buf) - 1, fmt, args);
        va_end(args);

        p += (n < 0) ? sizeof(buf) - 1 : n;

        //while ( p > buf  &&  isspace(p[-1]) )
        while ( p > buf && ' ' ==p[-1] )
        {
            *--p = '\0';
        }
        *this = buf;

        return *this;
    }

    bool compare(const T * str)
    {
        return ( this->length() == (unsigned int)lstrlen(str) )       // optimization on some platforms
            && ( lstrcmp(this->c_str(), str) == 0 );    // actual compare
    }

	void update()
	{
		m_nStrLength = lstrlen(m_pBuf);
	}
private:
    T *m_pBuf;
    size_type m_nBufSize;
    size_type m_nStrLength;
};

template  <typename T, size_t default_bufsize>
inline bool operator == (const UString<T, default_bufsize> & a, const UString<T, default_bufsize> & b)
{
    return    ( a.length() == b.length() )            // optimization on some platforms
        && ( lstrcmp(a.c_str(), b.c_str()) == 0 );    // actual compare
}

template  <typename T, size_t default_bufsize>
inline bool operator < (const UString<T, default_bufsize>  & a, const UString<T, default_bufsize>  & b)
{
    return lstrcmp(a.c_str(), b.c_str()) < 0;
}

template  <typename T, size_t default_bufsize>
inline bool operator != (const UString<T, default_bufsize> & a, const UString<T, default_bufsize>& b) { return !(a == b); }

template  <typename T, size_t default_bufsize>
inline bool operator >  (const UString<T, default_bufsize> & a, const UString<T, default_bufsize> & b) { return b < a; }

template  <typename T, size_t default_bufsize>
inline bool operator <= (const UString<T, default_bufsize> & a, const UString<T, default_bufsize> & b) { return !(b < a); }

template  <typename T, size_t default_bufsize>
inline bool operator >= (const UString<T, default_bufsize> & a, const UString<T, default_bufsize> & b) { return !(a < b); }

template  <typename T, size_t default_bufsize>
inline bool operator == (const UString<T, default_bufsize> & a, const char* b) { return strcmp(a.c_str(), b) == 0; }

template  <typename T, size_t default_bufsize>
inline bool operator == (const char* a, const UString<T, default_bufsize> & b) { return b == a; }

template  <typename T, size_t default_bufsize>
inline bool operator != (const UString<T, default_bufsize> & a, const char* b) { return !(a == b); }

template  <typename T, size_t default_bufsize>
inline bool operator != (const char* a, const UString<T, default_bufsize> & b) { return !(b == a); }

template  <typename T, size_t default_bufsize>
inline UString<T, default_bufsize> operator + (const UString<T, default_bufsize> & a, const UString<T, default_bufsize> & b)
{
    UString<T, default_bufsize> tmp = a;
    tmp += b;
    return tmp;
}

template  <typename T, size_t default_bufsize>
inline UString<T, default_bufsize> operator + (const UString<T, default_bufsize> & a, const char* b)
{
    UString<T, default_bufsize> tmp = a;
    tmp += b;
    return tmp;
}

template  <typename T, size_t default_bufsize>
inline UString<T, default_bufsize> operator + (const char* a, const UString<T, default_bufsize> & b)
{
    UString<T, default_bufsize> tmp = a;
    tmp += b;
    return tmp;
}


typedef UString<char> UStringAnsi;
typedef UString<wchar_t> UStringUnicode;

}; // namespace ADT


}; // namespace huys

#endif // U_STRING_H

