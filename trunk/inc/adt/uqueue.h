/*
 * =====================================================================================
 *
 *       Filename:  uqueue.h
 *
 *    Description:  queue
 *
 *        Version:  1.0
 *        Created:  2009/7/2 20:53:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef HU_QUEUE_H
#define HU_QUEUE_H

namespace huys
{

namespace ADT
{

/*
 * Circular implementation of Queue
 */

template <class T, size_t maxqueue = 10>
class UQueue
{
public:
    typedef size_t size_type;
    typedef T entry_type;
    typedef T& reference;
    typedef const T& const_reference;

    enum Error_code {
        success = 0,
        fail,
        range_error,
        underflow,
        overflow,
        fatal,
        not_present,
        duplicate_error,
        entry_insert,
        entry_found,
        internal_error
    };
public:
    UQueue()
    : _count(0), _front(0), _rear(maxqueue-1)
    {}

    bool empty() const
    {
        return 0==_count;
    }

    bool full() const
    {
        return maxqueue == _count;
    }

    void clear()
    {
        while (!empty())
        {
            serve();
        }
    }

    Error_code append(const entry_type &item)
    {
        if (_count >= maxqueue)
        {
            return overflow;
        }
        ++_count;
        _rear = (maxqueue == (_rear+1)) ? 0 : (_rear+1);
        entry[_rear] = item;
        return success;
    }

    Error_code serve()
    {
        if (0 >= _count)
        {
            return underflow;
        }
        --_count;
        _front = (maxqueue == (_front+1)) ? 0 : (_front+1);
        return success;
    }

    Error_code retrieve(entry_type &item) const
    {
        if (0 >= _count)
        {
            return underflow;
        }
        item = entry[_front];
        return success;
    }

    size_type size() const
    {
        return _count;
    }

    entry_type &front()
    {
        return entry[_front];
    }

    const entry_type & front() const
    {
        return entry[_front];
    }

    entry_type &back()
    {
        return entry[_rear];
    }

    const entry_type & back() const
    {
        return entry[_rear];
    }
private:
    size_type _count;
    size_type _front;
    size_type _rear;
    entry_type entry[maxqueue];
};

}; // namespace ADT

}; // namespace huys

#endif // HU_QUEUE_H

