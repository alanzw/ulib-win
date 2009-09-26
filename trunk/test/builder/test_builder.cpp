#include <iostream>
#include <cstring>
#include <cstdio>

using std::cout;
using std::endl;
using std::cin;

namespace dp_app
{

enum PersistenceType
{
    File, Queue, Pathway
};

struct PersistenceAttribute
{
    PersistenceType type;
    char value[30];
};

class DistrWorkPackage
{
public:
    DistrWorkPackage(const char *type)
    {
        sprintf(_desc, "Distributed Work Package for: %s", type);
    }
    void setFile(const char *f, const char *v)
    {
        sprintf(_temp, "\n  File(%s): %s", f, v);
        strcat(_desc, _temp);
    }
    void setQueue(const char *q, const char *v)
    {
        sprintf(_temp, "\n  Queue(%s): %s", q, v);
        strcat(_desc, _temp);
    }
    void setPathway(const char *p, const char *v)
    {
        sprintf(_temp, "\n  Pathway(%s): %s", p, v);
        strcat(_desc, _temp);
    }
    const char *getState()
    {
        return _desc;
    }
  private:
    char _desc[200], _temp[80];
};

class Builder
{
public:
    virtual void configureFile(const char*) = 0;
    virtual void configureQueue(const char*) = 0;
    virtual void configurePathway(const char*) = 0;
    DistrWorkPackage *getResult()
    {
        return _result;
    }
protected:
    DistrWorkPackage *_result;
};

class UnixBuilder: public Builder
{
public:
    UnixBuilder()
    {
        _result = new DistrWorkPackage("Unix");
    }
    void configureFile(const char *name)
    {
        _result->setFile("flatFile", name);
    }
    void configureQueue(const char *queue)
    {
        _result->setQueue("FIFO", queue);
    }
    void configurePathway(const char *type)
    {
        _result->setPathway("thread", type);
    }
};

class VmsBuilder: public Builder
{
public:
    VmsBuilder()
    {
        _result = new DistrWorkPackage("Vms");
    }
    void configureFile(const char *name)
    {
        _result->setFile("ISAM", name);
    }
    void configureQueue(const char *queue)
    {
        _result->setQueue("priority", queue);
    }
    void configurePathway(const char *type)
    {
        _result->setPathway("LWP", type);
    }
};

class Reader
{
public:
    void setBuilder(Builder *b)
    {
        _builder = b;
    }
    void construct(PersistenceAttribute[], int);
private:
    Builder *_builder;
};

void Reader::construct(PersistenceAttribute list[], int num)
{
    for (int i = 0; i < num; i++)
    {
        if (list[i].type == File)
        {
            _builder->configureFile(list[i].value);
        }
        else if (list[i].type == Queue)
        {
            _builder->configureQueue(list[i].value);
        }
        else if (list[i].type == Pathway)
        {
            _builder->configurePathway(list[i].value);
        }
    }
}

const int NUM_ENTRIES = 6;
PersistenceAttribute input[NUM_ENTRIES] =
{
  {
    File, "state.dat"
  }
  ,
  {
    File, "config.sys"
  }
  ,
  {
    Queue, "compute"
  }
  ,
  {
    Queue, "log"
  }
  ,
  {
    Pathway, "authentication"
  }
  ,
  {
    Pathway, "error processing"
  }
};

void test()
{
    UnixBuilder unixBuilder;
    VmsBuilder vmsBuilder;
    Reader reader;

    reader.setBuilder(&unixBuilder);
    reader.construct(input, NUM_ENTRIES);
    cout << unixBuilder.getResult()->getState() << endl;

    reader.setBuilder(&vmsBuilder);
    reader.construct(input, NUM_ENTRIES);
    cout << vmsBuilder.getResult()->getState() << endl;
}

}; // namespace dp_app

int main()
{
    cout << "------------------------------------" << endl;

    //dp_before::test();

    cout << "------------------------------------" << endl;

    //dp_after::test();

    cout << "------------------------------------" << endl;

    dp_app::test();

    return 0;
}

