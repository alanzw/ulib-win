/**
 * @filename uarraylist.h
 * @author huys
 * @date 2008.04.04
 * @desciption usage of array list
 */

#ifndef U_ARRAYLIST_H
#define U_ARRAYLIST_H

namespace huys
{

namespace ADT
{

template <class T>
class UArrayList
{
public:
    //
    UArrayList(int size = 10);
    UArrayList(const UArrayList<T> &otherList);
    ~UArrayList();

    const UArrayList<T>& operator=(const UArrayList<T> &otherList);

    bool isEmpty();
    bool isFull();
    int getSize();
    int getMaxSize();
    void print() const;
    bool isItemAtEqual(int location, T item);
    void insertAt(int location, T itemInsert);
    void insertEnd(T itemInsert);
    void removeAt(int location);
    void retrieveAt(int location, T& retItem);
    void replaceAt(int location, const T& itemReplace);
    void clear();
    int search(int item);
    void insert(const T& itemInsert);
    void remove(const T& itemRemove);

    int seqSearch(const T& item);

    //
    typedef void (*callback_routine)(T *item);
    void traverse(callback_routine visit)
    {
        for (int i=0; i<length; i++)
        {
            visit(&(this->list[i]));
        }
    }
protected:

private:
    T *list;
    int length;
    int maxSize;
};

template <class T>
int UArrayList<T>::seqSearch( const T& item )
{
    int index = -1;
    int i;
    for (i=0; i<length; i++)
    {
        if (item == this->list[i])
        {
            index = i;
            break;
        }
    }
    return index;
}

template <class T>
const UArrayList<T>& UArrayList<T>::operator=( const UArrayList<T> &otherList )
{
    this->maxSize = otherList.maxSize;
    this->length = otherList.length;

    if (list)
    {
        delete[] list;
    }

    this->list = new T[maxSize];

    if (length != 0)
    {
        for (int i=0; i<length; i++)
        {
            this->list[i] = otherList.list[i];
        }
    }

    return *this;
}

template <class T>
UArrayList<T>::UArrayList(int size)
{
    if (size<0)
    {
        std::cout << "The Array size must be positive. "
                  << "Creating an array of size 10." << std::endl;
        maxSize = 10;
    } else {
        maxSize = size;
    }
    length = 0;
    list = new T[maxSize];
}


template <class T>
UArrayList<T>::UArrayList(const UArrayList<T> &otherList)
{
    this->maxSize = otherList.maxSize;
    this->length = otherList.length;
    this->list = new T[maxSize];

    if (length != 0)
    {
        for (int i=0; i<length; i++)
        {
            this->list[i] = otherList.list[i];
        }
    }
}

template <class T>
UArrayList<T>::~UArrayList()
{
    if (list)
    {
        delete[] list;
    }
    list = 0;
}

template <class T>
bool UArrayList<T>::isEmpty()
{
    return length == 0;
}

template <class T>
bool UArrayList<T>::isFull()
{
    return length == maxSize;
}

template <class T>
int UArrayList<T>::getSize()
{
    return length;
}

template <class T>
int UArrayList<T>::getMaxSize()
{
    return maxSize;
}

template <class T>
void UArrayList<T>::print() const
{
    for (int i=0; i<length; i++)
    {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;
}

template <class T>
bool UArrayList<T>::isItemAtEqual(int location, T item)
{
    return list[location] == item;
}

template <class T>
void UArrayList<T>::insertAt(int location, T itemInsert)
{
    if (location < 0 || location >= maxSize)
    {
        std::cout << "Overflow!" << std::endl;
    }
    else
    {
        if (length >= maxSize)
        {
            std::cout << "Overflow!" << std::endl;
        }
        else
        {
            for (int i=length; i>location; i--)
            {
                list[i] = list[i-1];
            }

            list[location] = itemInsert;
            length++;
        }
    }
}

template <class T>
void UArrayList<T>::insertEnd(T itemInsert)
{
    if (length >= maxSize)
    {
        std::cout << "Overflow!" << std::endl;
    }
    else
    {
        list[length] = itemInsert;
        length++;
    }
}

template <class T>
void UArrayList<T>::removeAt(int location)
{
    if (location<0 || location>length)
    {
        std::cout << "Error!" << std::endl;
    }
    else
    {
        for (int i=location; i<length-1; i++)
        {
            list[i] = list[i+1];
        }
        length--;
    }
}

template <class T>
void UArrayList<T>::retrieveAt(int location, T& retItem)
{
    if (location<0 || location>length)
    {
        std::cout << "Error!" << std::endl;
    }
    else
    {
        retItem = list[location];
    }
}


template <class T>
void UArrayList<T>::replaceAt(int location, const T &itemReplace)
{
    if (location<0 || location>length)
    {
        std::cout << "Error!" << std::endl;
    }
    else
    {
        list[location] = itemReplace;
    }
}

template <class T>
void UArrayList<T>::clear()
{
    length = 0;
}

}; // namespace ADT

}; // namespace huys

#endif // U_ARRAYLIST_H
