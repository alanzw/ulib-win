#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

class VisualizableVector
{
public:
    class proxy
    {
    public:
        proxy(vector<int> *v) : vect(v)
        {
            cout << "Before push_back size is: " << vect->size() << endl;
        }
        vector<int> * operator -> () {
            return vect;
        }
        ~proxy () {
            cout << "After push_back size is: " << vect->size() << endl;
        }

    private:
        vector <int> * vect;
    };

    VisualizableVector (vector<int> *v) : vect(v) {}

    // Overloaded -> operator of visualizableVector creates a temporary proxy object
    proxy operator -> () {
        return proxy (vect);
    }

private:
    vector <int> * vect;
};

template <class NextAspect, class Para>
class Aspect
{
protected:
    Aspect (Para p): para_(p) {}
    Para  para_;
public:
    NextAspect operator -> ()
    {
        return para_;
    }
};

template <class NextAspect, class Para>
struct Visualizing : Aspect <NextAspect, Para>
{
public:
    Visualizing (Para p)
    : Aspect <NextAspect, Para> (p)
    {
        cout << "Before Visualization aspect" << endl;
    }
    ~Visualizing ()
    {
        cout << "After Visualization aspect" << endl;
    }
};

template <class NextAspect, class Para>
struct Locking : Aspect <NextAspect, Para>
{
public:
    Locking (Para p)
    : Aspect <NextAspect, Para> (p)
    {
        cout << "Before Lock aspect" << endl;
    }
    ~Locking ()
    {
        cout << "After Lock aspect" << endl;
    }
};

template <class NextAspect, class Para>
struct Logging : Aspect <NextAspect, Para>
{
public:
    Logging (Para p)
    : Aspect <NextAspect, Para> (p)
    {
        cout << "Before Log aspect" << endl;
    }
    ~Logging ()
    {
        cout << "After Log aspect" << endl;
    }
};

template <class Aspect, class Para>
class AspectWeaver
{
public:
    AspectWeaver (Para p) : para_(p) {}
    Aspect operator -> ()
    {
        return Aspect (para_);
    }
private:
    Para para_;
};

#define AW1(T,U) AspectWeaver <T <U, U>, U >
#define AW2(T,U,V) AspectWeaver <T < U <V, V> , V>, V >
#define AW3(T,U,V,X) AspectWeaver <T < U <V <X, X>, X> , X>, X >


int main()
{
    VisualizableVector vect(new vector<int>);
    vect->push_back (10); // Note use of -> operator instead of . operator
    vect->push_back (20);

    AW3(Visualizing, Locking, Logging, vector <int> *)
    X (new vector<int>);
    //...
    X->push_back (10); // Note use of -> operator instead of . operator
    X->push_back (20);

    return 0;
}

