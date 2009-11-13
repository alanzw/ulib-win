// FIXME: Failed to run.

#include <iostream>

#include "uthunk32.h"

using namespace std;

// A class into which we want a callback dispatched
class MyClass
{
public:
    UThunk32<MyClass, int (MyClass::*)(int, int)> simpleCallbackThunk;

    MyClass()
    {
        simpleCallbackThunk.initializeThunk(this, &MyClass::simpleCallback); // May throw std::exception
    }
    
    virtual int simpleCallback(int i1, int i2)
    {
        cout << "MyClass::simpleCallback hit" << endl;
        return 10;
    }
};

class MyDerivedClass : public MyClass
{
public:
    virtual int simpleCallback(int i1, int i2)
    {
        cout << "MyDerivedClass::simpleCallback hit, "
        << "heading for parent class" << endl;
        return MyClass::simpleCallback(i1, i2);
    }
};

// A simple callback, simulating those of the win32 api
void someCallbackMechanism(int (__stdcall *func)(int, int))
{
    (*func)(10, 10);
}

int main()
{
    /*
    First demo: Should have the callback mechanism call MyClass::simpleCallback.
    */
    cout << "First demo" << endl;
    MyClass myClassInstance;
    someCallbackMechanism(myClassInstance.simpleCallbackThunk.getCallback());
    cout << endl;

    /*
    Second demo: Should have the callback mechanism call MyDerivedClass::simpleCallback,
    which goes on to call MyClass::simpleCallback.
    */
    cout << "Second demo" << endl;
    MyDerivedClass myDerivedInstance;
    MyClass& myClassReference = myDerivedInstance;
    someCallbackMechanism(myClassReference.simpleCallbackThunk.getCallback());
    cout << endl;

    cout << "Press enter to exit" << endl;	
    cin.get();

    return 0;
}