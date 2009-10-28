/*
 * =====================================================================================
 *
 *       Filename:  ulinkedlist.h
 *
 *    Description:  ADT for  Linked List
 *
 *        Version:  1.0
 *        Created:  06/20/2008 08:21:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hu), huys03@gmail.com
 *        Company:  HUYS .Inc
 *
 * =====================================================================================
 */

#ifndef U_LINKEDLIST_H
#define U_LINKEDLIST_H

namespace huys
{

namespace ADT
{

template <class T>
class ULinkedList
{
    struct Node
    {
        T info;
        Node *link;
    };


public:
    ULinkedList();
    ULinkedList(const ULinkedList<T> &other);
    ~ULinkedList();
    //
    void init();
    bool isEmpty();
    bool isFull();
    void print();
    int length();
    void destroy();
    void retrieveFirst(T &firstElement);
    void search(const T &item);
    void insertFirst(const T &item);
    void insertLast(const T &item);
    void deleteNode(const T &item);
protected:
    Node *first;
    Node *last;
private:
}; // class ULinkedList


template <class T>
ULinkedList<T>::ULinkedList()
: first(NULL), last(NULL)
{}

template <class T>
void init()
{
    destroy();
}

template <class T>
bool ULinkedList<T>::isEmpty()
{
    return (first==NULL);
}

template <class T>
bool ULinkedList<T>::isFull()
{
    return false;
}

template <class T>
void ULinkedList<T>::print()
{
    Node<T> *current;
    current = first;
    while (current != NULL)
    {
        std::cout << current->info << " ";
        current = current->link;
    }
}

template <class T>
void ULinkedList<T>::destroy()
{
    Node<T> *temp;
    while (first != NULL)
    {
        temp = first;
        first = first->link;
        delete temp;
    }
    last = NULL;
}

template <class T>
int ULinkedList<T>::length()
{
    int count = 0;
    Node<T> *current;

    current = first;
    while (current != NULL)
    {
        count++;
        current = current->link;
    }
    return count;
}

template <class T>
void ULinkedList<T>::retrieveFirst(T &firstElement)
{
    firstElement = first->info;
}

template <class T>
void ULinkedList<T>::search(const T &item)
{
    Node<T> *current;
    bool found;

    if (first == NULL)
    {
        std::cout << "Cannot search an empty list." << std::endl;
    }
    else
    {
        current = first;
        found = false;
    }
}

}; // namespace ADT

}; // namespace huys

#endif // U_LINKEDLIST_H
