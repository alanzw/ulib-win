#include <iostream>
#include <cstring>

#include "dp/ufactory.h"

#include "adt/uvector.h"

using std::cout;
using std::endl;
using std::cin;

namespace dp_before
{

class Stooge
{
public:
    virtual void slap_stick() = 0;
};

class Larry: public Stooge
{
public:
    void slap_stick()
    {
        cout << "Larry: poke eyes\n";
    }
};

class Moe: public Stooge
{
public:
    void slap_stick()
    {
        cout << "Moe: slap head\n";
    }
};

class Curly: public Stooge
{
public:
    void slap_stick()
    {
        cout << "Curly: suffer abuse\n";
    }
};

void test()
{
    huys::ADT::UVector<Stooge*> roles;
    int choice;

    while (true)
    {
        cout << "Larry(1) Moe(2) Curly(3) Go(0): ";
        cin >> choice;
        if (choice == 0)
            break;
        else if (choice == 1)
            roles.push_back(new Larry);
        else if (choice == 2)
            roles.push_back(new Moe);
        else
            roles.push_back(new Curly);
    }
    for (int i = 0; i < roles.size(); i++)
        roles[i]->slap_stick();
    for (int i = 0; i < roles.size(); i++)
        delete roles[i];
}

}; // namespace dp_before


namespace dp_after
{

class Stooge
{
public:
    // Factory Method
    static Stooge *make_stooge(int choice);
    virtual void slap_stick() = 0;
};

class Larry: public Stooge
{
public:
    void slap_stick()
    {
        cout << "Larry: poke eyes\n";
    }
};

class Moe: public Stooge
{
public:
    void slap_stick()
    {
        cout << "Moe: slap head\n";
    }
};

class Curly: public Stooge
{
public:
    void slap_stick()
    {
        cout << "Curly: suffer abuse\n";
    }
};

Stooge *Stooge::make_stooge(int choice)
{
    if (choice == 1)
        return new Larry;
    else if (choice == 2)
        return new Moe;
    else
        return new Curly;
}

void test()
{
    huys::ADT::UVector<Stooge*> roles;
    int choice;
    while (true)
    {
        cout << "Larry(1) Moe(2) Curly(3) Go(0): ";
        cin >> choice;
        if (choice == 0)
            break;
        roles.push_back(Stooge::make_stooge(choice));
    }
    for (int i = 0; i < roles.size(); i++)
        roles[i]->slap_stick();
    for (int i = 0; i < roles.size(); i++)
        delete roles[i];
}

}; // namespace dp_after

namespace dp_app
{

/* Abstract base class declared by framework */
class Document
{
public:
    Document(char *fn)
    {
        strcpy(name, fn);
    }
    virtual void Open() = 0;
    virtual void Close() = 0;
    char *GetName()
    {
        return name;
    }
private:
    char name[20];
};

/* Concrete derived class defined by client */
class MyDocument: public Document
{
public:
    MyDocument(char *fn): Document(fn){}
    void Open()
    {
        cout << "   MyDocument: Open()" << endl;
    }
    void Close()
    {
        cout << "   MyDocument: Close()" << endl;
    }
};

/* Framework declaration */
class Application
{
public:
    Application(): _index(0)
    {
        cout << "Application: ctor" << endl;
    }
    /* The client will call this "entry point" of the framework */
    void NewDocument(char *name)
    {
        cout << "Application: NewDocument()" << endl;
        /* Framework calls the "hole" reserved for client customization */
        _docs[_index] = CreateDocument(name);
        _docs[_index++]->Open();
    }
    void OpenDocument(){}
    void ReportDocs();
    /* Framework declares a "hole" for the client to customize */
    virtual Document *CreateDocument(char*) = 0;
  private:
    int _index;
    /* Framework uses Document's base class */
    Document *_docs[10];
};

void Application::ReportDocs()
{
    cout << "Application: ReportDocs()" << endl;
    for (int i = 0; i < _index; i++)
        cout << "   " << _docs[i]->GetName() << endl;
}

/* Customization of framework defined by client */
class MyApplication: public Application
{
public:
    MyApplication()
    {
        cout << "MyApplication: ctor" << endl;
    }
    /* Client defines Framework's "hole" */
    Document *CreateDocument(char *fn)
    {
        cout << "   MyApplication: CreateDocument()" << endl;
        return new MyDocument(fn);
    }
};

void test()
{
    /* Client's customization of the Framework */
    MyApplication myApp;

    myApp.NewDocument("foo");
    myApp.NewDocument("bar");
    myApp.ReportDocs();
}

}; // namespace dp_app

int main()
{

    typedef huys::dp::creational::Factory<int> Factory;
    Factory fa;

    Factory::ProductType *p = fa.produce();

    p->set(11);

    cout << p->get() << endl;

    cout << "------------------------------------" << endl;

    dp_before::test();

    cout << "------------------------------------" << endl;

    dp_after::test();
    
    cout << "------------------------------------" << endl;

    dp_app::test();

    return 0;
}

