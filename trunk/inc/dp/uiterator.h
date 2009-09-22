#ifndef U_ITERATOR_H
#define U_ITERATOR_H

template<class T,class U>
class Iterator
{
public:
    typedef std::vector<T>::iterator iter_type;

    Iterator(U*  pData)
    : m_pData(pData)
    {
        m_it = m_pData->m_data.begin();
    }

    void first()
    {
        m_it = m_pData->m_data.begin();
    }

    void next()
    {
        m_it++;
    }

    bool isDone()
    {
        return (m_it == m_pData->m_data.end());
    }

    iter_type current()
    {
        return m_it;
    }
private:
    U* m_pData;
    iter_type m_it;
};

template<class T,class U,class A>
class setIterator
{
public:
    typedef std::set<T,U>::iterator iter_type;

    setIterator(A* pData):m_pData(pData)
    {
        m_it = m_pData->m_data.begin();
    }

    void first()
    {
        m_it = m_pData->m_data.begin();
    }

    void next()
    {
        m_it++;
    }

    bool isDone()
    {
        return (m_it == m_pData->m_data.end());
    }

    iter_type current()
    {
        return m_it;
    }

private:
    A*                m_pData;
    iter_type        m_it;
};

template <class T>
class aggregate
{
    friend Iterator<T,aggregate>;
public:
    void add(T a)
    {
        m_data.push_back(a);
    }

    Iterator<T,aggregate>* create_iterator()
    {
        return new Iterator<T,aggregate>(this);
    }


private:
    std::vector<T> m_data;
};

template <class T,class U>
class aggregateSet
{
    friend setIterator<T,U,aggregateSet>;
public:
    void add(T a)
    {
        m_data.insert(a);
    }

    setIterator<T,U,aggregateSet>* create_iterator()
    {
        return new setIterator<T,U,aggregateSet>(this);
    }

    void Print()
    {
        copy(m_data.begin(),m_data.end(),ostream_iterator<T>(cout,"\r\n"));
    }

protected:
private:
    std::set<T,U> m_data;
};

#endif // U_ITERATOR_H

