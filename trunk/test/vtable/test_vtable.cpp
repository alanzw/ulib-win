#include <iostream>

using std::cout;
using std::endl;

class B1
{
public:
    void f0()
    {
        cout << "B1 : " << this << " --- " << " f0" << endl;
    }
    virtual void f1() {}
    int int_in_b1;
};

class B2
{
public:
    virtual void f2() {}
    int int_in_b2;
};

class D : public B1, public B2
{
public:
    void d() {}
    void f2() {}  // override B2::f2()
    int int_in_d;
};

class Base {

public:

    virtual void f() { cout << "Base::f" << endl; }

    virtual void g() { cout << "Base::g" << endl; }

    virtual void h() { cout << "Base::h" << endl; }

};


int main()
{
    cout << "======= Virtual Method Table Testing ==================" << endl;

    B2 *b2 = new B2();
    D  *d  = new D();

    cout << "sizeof(B2) : " << sizeof(B2) << endl;
    cout << "B2: " << b2 << endl;
    cout << "int_in_b2 : +" << (char *)(&b2->int_in_b2) - (char *)b2  << endl;
    cout << "B2::f2() : " << &B2::f2 << endl;

    cout << "--------------------------------------------" << endl;
    cout << "sizeof(D) : " << sizeof(D) << endl;
    cout << "D : " << d << endl;
    cout << "dynamic_cast<B1*>(d) : " << dynamic_cast<B1*>(d) << endl;
    cout << "dynamic_cast<B2*>(d) : " << dynamic_cast<B2*>(d) << endl;
    cout << "d.int_in_b1 : +" << (char *)(&d->int_in_b1) - (char *)d  << endl;
    cout << "d.int_in_b2 : +" << (char *)(&d->int_in_b2) - (char *)d  << endl;
    cout << "d.int_in_d: +" << (char *)(&d->int_in_d) - (char *)d  << endl;
    cout << "D::d() : " << &D::d << endl;
    cout << "D::f1() : " << &D::f1 << endl;
    cout << "D::f2() : " << &D::f2 << endl;

    delete b2;
    delete d;

    cout << "--------------------------------------------" << endl;
    typedef void(*Fun)(void);
    Base b;
    Fun *ptr=(Fun*&)b;//
    cout << "sizeof(Base) : " << sizeof(Base) << endl; //
    cout << "VTable address: " << (int *)&b << endl;
    cout << (int *)(*(int *)(&b)) << endl;//
    cout << ptr << endl;//
    cout << &Base::f << endl;
    cout << ptr[0] <<endl;//
    ptr[0]();//


    return 0;
}

