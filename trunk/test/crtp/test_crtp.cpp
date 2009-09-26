#include <iostream>

template <class TDerived>
struct Base
{
    void interface()
    {
        static_cast<TDerived *>(this)->implementation();
    }

    static void static_func()
    {
        TDerived::static_sub_func();
    }
};

struct Derived : public Base<Derived>
{
    void implementation()
    {
        std::cout << "Derived : implementation" << std::endl;
    }

    static void static_sub_func()
    {
        std::cout << "Derived : static_sub_func" << std::endl;;
    }
};

template <class T>
void foo(Base<T> * p)
{
    p->interface();
}

template <typename T>
struct counter
{
    counter()
    {
        objects_created++;
        objects_alive++;
    }

    virtual ~counter()
    {
        --objects_alive;
    }
    static int objects_created;
    static int objects_alive;
};

template <typename T> int counter<T>::objects_created( 0 );
template <typename T> int counter<T>::objects_alive( 0 );

class X : counter<X>
{
    // ...
};

class Y : counter<Y>
{
    // ...
};

void test()
{
    Derived d;
    d.interface();
    foo(&d);

    X x1;

    X *pX2 = new X;
    delete pX2;
    pX2 = NULL;

    std::cout << "X alive: " << counter<X>::objects_alive << std::endl;
    std::cout << "X created: " << counter<X>::objects_created << std::endl;
}

int main()
{
    test();
    return 0;
}

