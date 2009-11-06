#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

template <class T>
struct compare_class
{
    virtual bool operator()(const T &a, const T &b) = 0;
};

template <class T>
struct less_than : public compare_class<T>
{
    bool operator()(const T &a, const T &b)
    {
        return a<b;
    }
};

template <class T>
struct greater_than : public compare_class<T>
{
    bool operator()(const T &a, const T &b)
    {
        return a>b;
    }
};

// Declaration of C++ sorting function.
template <class ComparisonFunctor>
void sort_ints(int* array, int n, ComparisonFunctor cf)
{
    int i, j;
    int temp;
    int test; /*use this only if unsure whether the list is already sorted or not*/
    for(i = n-1; i>0; i--)
    {
        test=0;
        for(j=0; j<i; j++)
        {
            if(cf(array[j], array[j+1])) /* compare neighboring elements */
            {
                temp = array[j];    /* swap array[j] and array[j+1] */
                array[j] = array[j+1];
                array[j+1] = temp;
                test=1;
            }
        } /*end for j*/
        if(test==0) break; /*will exit if the list is sorted!*/
    } /*end for i*/
}

void print(int a[], int n)
{
    for (int i = 0; i < n-1; i++)
    {
        cout << a[i] << "-";
    }
    cout << a[n-1];
    cout << endl;
}


class FunctorBase
{
protected:
    typedef void (FunctorBase::*PMemFunc)();
    typedef void (*PFunc)();
    enum { MEM_FUNC_SIZE = sizeof(PMemFunc) };
    union{
        PFunc func;
        char memFunc[MEM_FUNC_SIZE];
    };
    void *callee;

    FunctorBase()
    :func(0),callee(0)
    {}

    FunctorBase(const void *c, PFunc f, const void *mf, size_t sz)
    :callee((void *)c)
    {
        if(c)
        {
            memcpy(memFunc, mf, sz);
            if(sz<MEM_FUNC_SIZE)
                memset(memFunc+sz,0,MEM_FUNC_SIZE-sz);
        }
        else func = f;
    }
public:
    operator bool()const{return callee||func;}
    friend bool operator==(const FunctorBase &lhs,const FunctorBase &rhs);
    friend bool operator!=(const FunctorBase &lhs,const FunctorBase &rhs);
    friend bool operator<(const FunctorBase &lhs,const FunctorBase &rhs);
    PFunc    getFunc() const {return func;}
    void *getCallee() const {return callee;}
    const char *getMemFunc() const {return memFunc;}
};

class Functor0 : public FunctorBase
{
private:
    typedef void (*Thunk)(const FunctorBase &);
    Thunk _thunk;
public:
    void operator()() const
    {
       _thunk(*this);
    }

    Functor0() {}

protected:
    Functor0(Thunk t, const void *c, PFunc f, const void *mf, size_t s)
    : FunctorBase(c, f, mf, s), _thunk(t)
    {}
};

template <class Callee, class MemFunc>
class MemberTranslator0 : public Functor0
{
public:
    MemberTranslator0(Callee &c,const MemFunc &m)
    : Functor0(thunk, &c, 0, &m, sizeof(MemFunc))
    {}

    static void thunk(const FunctorBase &ftor)
    {
        Callee *callee = (Callee *)ftor.getCallee();
        MemFunc &memFunc = (*(MemFunc*)(void *)(ftor.getMemFunc()));
        (callee->*memFunc)();
    }
};

template <class Func>
class FunctionTranslator0 : public Functor0
{
public:
    FunctionTranslator0(Func f)
    : Functor0(thunk,0,(PFunc)f,0,0)
    {}

    static void thunk(const FunctorBase &ftor)
    {
        (Func(ftor.getFunc()))();
    }
};

template <class Callee, class TRT, class CallType>
inline MemberTranslator0<Callee,TRT (CallType::*)()>
makeFunctor(Callee &c,TRT (CallType::* f)())
{
    typedef TRT (CallType::*MemFunc)();
    return MemberTranslator0<Callee,MemFunc>(c,f);
}


template <class Callee,class TRT,class CallType>
inline MemberTranslator0<const Callee,TRT (CallType::*)()const>
makeFunctor(const Callee &c, TRT (CallType::* f)()const)
{
    typedef TRT (CallType::*MemFunc)()const;
    return MemberTranslator0<const Callee,MemFunc>(c,f);
}

template <class TRT>
inline FunctionTranslator0<TRT (*)()>
makeFunctor(TRT (*f)())
{
    return FunctionTranslator0<TRT (*)()>(f);
}


class Button
{
public:
    Button(const Functor0 &uponClickDoThis)
    : notify(uponClickDoThis)
    {}

    void click()
    {
        notify();    //a call to operator()
    }
private:
    Functor0 notify;    //note - held by value
};

//Some application stuff we'd like to connect to Button:

class CDPlayer
{
public:
    void play()
    {
        cout<<"Playing"<<endl;
    }

    void stop()
    {
        cout<<"Stopped"<<endl;
    }
};

void wow()
{
    std::cout << "Wow!" << std::endl;
}

int ccc()
{
    std::cout << "cccc" << std::endl;
}

int main()
{
    cout << "=========================" << endl;

    int items[] = {4, 3, 1, 2};
    less_than<int> functor;

    print(items, 4);

    sort_ints(items, sizeof(items)/sizeof(int), functor);

    print(items, 4);

    greater_than<int> ftor;

    sort_ints(items, sizeof(items)/sizeof(int), ftor);

    print(items, 4);


    CDPlayer cd;

    //makeFunctor from object and ptr-to-member-function

    Button playButton(makeFunctor(cd,&CDPlayer::play));
    Button stopButton(makeFunctor(cd,&CDPlayer::stop));

    //makeFunctor from pointer-to-function

    Button wowButton(makeFunctor(&wow));

    Button cccButton(makeFunctor(&ccc));

    playButton.click();    //calls cd.play()
    stopButton.click();    //calls cd.stop()
    wowButton.click();    //calls wow()
    cccButton.click();

    return 0;
}

