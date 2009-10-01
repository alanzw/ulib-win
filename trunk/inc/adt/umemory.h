/*
 * =====================================================================================
 *
 *       Filename:  umemory.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  09/30/2009 07:59:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_MEMORY_H
#define U_MEMORY_H

namespace huys
{

namespace ADT
{

//
template <typename T>
class UMemoryBlockReference;

//
template <typename T>
class UMemoryBlock
{
    friend class UMemoryBlockReference<T>;
public:
    typedef T* TPtr;
protected:
    UMemoryBlock(size_t length, T * data)
    {
        _length = length;
        _data = data;
        _dataBlockAddress = data;
        _reference = 0;
    }

    UMemoryBlock()
    : UMemoryBlock(0,0)
    {}

    explicit UMemoryBlock(size_t items)
    {
        _length = items;
        allocate(_length);
        _reference = 0;
    }

    virtual ~UMemoryBlock()
    {
        if (_dataBlockAddress)
        {
            deallocate();
        }
    }

    T * data()
    {
        return _data;
    }

    const T * data()
    {
        return _data;
    }

    T * & dataBlockAddress()
    {
        return _dataBlockAddress;
    }

    size_t length() const
    {
        return _length;
    }

    void addReference()
    {
        ++_reference;
    }

    int removeReference()
    {
        int refcount = --_reference;
        return refcount;
    }

    int references() const
    {
        int refcount = _reference;
        return refcount;
    }

protected:
    void allocate(size_t length);
    void deallocate();

private:
    UMemoryBlock(const UMemoryBlock<T>&);
    void operator=(const UMemoryBlock<T>&);

private:
    T * _data;
    T * _dataBlockAddress;

    int _reference;
    size_t _length;
};

template <typename T>
class UMemoryBlockReference
{
protected:
    T * _data;
private:
    UMemoryBlock<T> *_block;
public:
    UMemoryBlockReference()
    {
        _block = 0;
        addRef();
        _data = 0;
    }

    UMemoryBlockReference(UMemoryBlockReference<T>& ref, size_t offset = 0)
    {
        _block = ref._block;
        addRef();
        _data = ref._data + offset;
    }

    explicit UMemoryBlockReference(size_t items)
    {
        _block = new UMemoryBlock<T>(items);
        addRef();
        _data = _block->_data;
    }

    ~UMemoryBlockReference()
    {
        blockRemoveRef();
    }

protected:
    int numRef()
    {
        if (_block)
        {
            return _block->references();
        }
        return -1;
    }

    void changeToNullBlock()
    {
        blockRemoveRef();
        _block = 0;
        addRef();
        _data = 0;
    }

    void changeBlock(UMemoryBlock<T> & ref, size_t offset = 0)
    {
        blockRemoveRef();
        _block = ref._block;
        addRef();
        _data = ref._data + offset;
    }

    void newBlock(size_t items)
    {
        blockRemoveRef();
        _block = new UMemoryBlock<T>(items);
        addRef();
        _data = _block->data();
    }

    void blockRemoveRef()
    {
        int refcount = removeReference();

        if (0 == refcount)
        {
            delete _block;
        }
    }

    void addRef() const
    {
        if (_block)
        {
            _block->addRef();
        }
    }

    int removeReference()
    {
        if (_block)
        {
            return _block->removeReference();
        }
        else
        {
            return -1;
        }
    }
};

}; // namespace ADT

}; // namespace huys

#endif // U_MEMORY_H

