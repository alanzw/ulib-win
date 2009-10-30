#include <iostream>

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

struct FunctorBase
{
    virtual void operator()() = 0;
};

struct Functor0 : public FunctorBase
{
    typedef void (*func_type)();
    void operator()()
    {
       _func();
    }

    Functor0(Functor0 const &f)
    : _func(f._func)
    {}

    Functor0(func_type pf)
    : _func(pf)
    {}

    func_type _func;
};

template <typename T>
Functor0 makeFunctor(T pf)
{
    return Functor0(pf);
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

    //Button playButton(makeFunctor(cd,&CDPlayer::play));
    //Button stopButton(makeFunctor(cd,&CDPlayer::stop));

    //makeFunctor from pointer-to-function

    Button wowButton(makeFunctor(&wow));

    //playButton.click();    //calls cd.play()
    //stopButton.click();    //calls cd.stop()
    wowButton.click();    //calls wow()

    return 0;
}

