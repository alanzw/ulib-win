#ifndef U_SIGSLOT_H
#define U_SIGSLOT_H

#include <set>
#include <list>

#include "ulib.h"

/*
 * Signals and slots is a language construct introduced in Qt,
 * which makes it easy to implement the Observer pattern
 * while avoiding boilerplate code.
 */

namespace USigSlot
{

class has_slots;

class signal_base
{
public:
    virtual void slot_disconnect(has_slots* pslot) = 0;
    virtual void slot_duplicate(const has_slots* pOldSlot, has_slots* pNewSlot) = 0;
};

class connection_base0
{
public:
    virtual has_slots* getDest() const = 0;
    virtual void emit() = 0;
    virtual connection_base0* clone() = 0;
    virtual connection_base0* duplicate(has_slots* pNewDest) = 0;
};

class has_slots
{
    typedef std::set<signal_base *> sender_set;
    typedef sender_set::const_iterator const_iterator;

public:
    has_slots()
    {}

    has_slots(const has_slots &hs)
    {
        const_iterator it = hs.m_senders.begin();
        const_iterator itEnd = hs.m_senders.end();

        while(it != itEnd)
        {
            (*it)->slot_duplicate(&hs, this);
            m_senders.insert(*it);
            ++it;
        }
    }

    void signal_connect(signal_base* sender)
    {
        m_senders.insert(sender);
    }

    void signal_disconnect(signal_base* sender)
    {
        m_senders.erase(sender);
    }

    virtual ~has_slots()
    {
        disconnect_all();
    }

    void disconnect_all()
    {
        const_iterator it = m_senders.begin();
        const_iterator itEnd = m_senders.end();

        while (it != itEnd)
        {
            (*it)->slot_disconnect(this);
            ++it;
        }

        m_senders.erase(m_senders.begin(), m_senders.end());
    }
private:
    sender_set m_senders;
};

class signal_base0 : public signal_base
{
public:
        typedef std::list<connection_base0 *>  connections_list;

        signal_base0()
        {}

        signal_base0(const signal_base0& s)
        : signal_base(s)
        {
            connections_list::const_iterator it = s.m_connected_slots.begin();
            connections_list::const_iterator itEnd = s.m_connected_slots.end();

            while(it != itEnd)
            {
                (*it)->getDest()->signal_connect(this);
                m_connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        ~signal_base0()
        {
            disconnect_all();
        }

        void disconnect_all()
        {
            connections_list::const_iterator it = m_connected_slots.begin();
            connections_list::const_iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                (*it)->getDest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
        }

        void disconnect(has_slots* pclass)
        {
            connections_list::iterator it = m_connected_slots.begin();
            connections_list::iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                if((*it)->getDest() == pclass)
                {
                    delete *it;
                    m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    return;
                }

                ++it;
            }
        }

        void slot_disconnect(has_slots* pslot)
        {
            connections_list::iterator it = m_connected_slots.begin();
            connections_list::iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                connections_list::iterator itNext = it;
                ++itNext;

                if((*it)->getDest() == pslot)
                {
                    m_connected_slots.erase(it);
                    //            delete *it;
                }

                it = itNext;
            }
        }

        void slot_duplicate(const has_slots* oldtarget, has_slots* newtarget)
        {
            connections_list::iterator it = m_connected_slots.begin();
            connections_list::iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                if((*it)->getDest() == oldtarget)
                {
                    m_connected_slots.push_back((*it)->duplicate(newtarget));
                }

                ++it;
            }
        }

    protected:
        connections_list m_connected_slots;
};

template <class T>
class connection0 : public connection_base0
{
public:
    connection0()
        : m_pObject(NULL), m_pMemFunc(NULL)
    {}

    connection0(T* pObject, void (T::*pMemFunc)())
        : m_pObject(pObject), m_pMemFunc(pMemFunc)
    {}

    virtual connection_base0* clone()
    {
        return new connection0<T>(*this);
    }

    virtual connection_base0* duplicate(has_slots *pDest)
    {
        return new connection0<T>((T *)pDest, m_pMemFunc);
    }

    virtual void emit()
    {
        (m_pObject->*m_pMemFunc)();
    }

    virtual has_slots* getDest() const
    {
        return m_pObject;
    }

private:
    T * m_pObject;
    void (T::*m_pMemFunc)();
};

class signal0 : public signal_base0
{
public:
    signal0() {}

    signal0(const signal0 & s)
    : signal_base0(s)
    {}

    template <class TC>
    void connect(TC *pClass, void (TC::*pMenFunc)())
    {
        connection0<TC> * conn = new connection0<TC>(pClass, pMenFunc);
        m_connected_slots.push_back(conn);
        pClass->signal_connect(this);
    }

    void emit()
    {
        connections_list::const_iterator iter = m_connected_slots.begin();
        for(; iter != m_connected_slots.end(); ++iter)
        {
            (*iter)->emit();
        }
    }

    void operator()()
    {
        emit();
    }
};



}; // namespace USigSlot

#endif // U_SIGSLOT_H
