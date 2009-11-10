#include <iostream>
#include <typeinfo>

#include "dp/udelegate.h"
#include "adt/uvector.h"

typedef huys::dp::utility::Delegate * DelegatePtr;
typedef huys::ADT::UVector<DelegatePtr> UVec;

void Test(int iParam)
{
    std::cout << "Test Invoked\r\n";
}

class A
{
public:
    void Test(int iParam)
    {
        std::cout << "A::Test Invoked\r\n";
    }
};

void test()
{
    typedef huys::dp::utility::NonTypeDelegate NTDelegate;
    typedef huys::dp::utility::TypeDelegate<A> TDelegate;

    NTDelegate ntDlt(Test, 1);

    A a;
    TDelegate tDlt(a, &A::Test, 2);

    UVec v;
    v.push_back(&ntDlt);
    v.push_back(&tDlt);

    UVec::const_iterator it;
    for( it = v.begin(); it != v.end(); ++it)
    {
        (*it)->invoke();
    }
}

namespace UDelegate
{

    template <typename TArg1, typename TArg2>
    struct ClosureBase
    {
        virtual void operator()(TArg1& arg1, TArg2& arg2) = 0;
    };

    template <class T, typename TArg1, typename TArg2>
    class Closure : public ClosureBase<TArg1, TArg2>
    {
    public:
        typedef void (T::*TMethod)(TArg1&, TArg2&);
        typedef T * TObjectPtr;

    public:
        Closure(TObjectPtr objptr, TMethod method)
        : _method(method), _objptr(objptr)
        {}

        ~Closure()
        {}

        virtual void operator()(TArg1& arg1, TArg2& arg2)
        {
            (_objptr->*_method)(arg1, arg2);
        }
    private:
        TMethod _method;
        TObjectPtr _objptr;
    };

    // Create a generator for Closure objects to get rid of template-parameters
    template <class T, typename TArg1, typename TArg2>
    Closure<T, TArg1, TArg2> * CreateClosure(T * objptr, void (T::*method)(TArg1&, TArg2&))
    {
        return new Closure<T, TArg1, TArg2>(objptr, method);
    }

    class UButton;

    class UWinMan
    {
    public:
        static UWinMan & instance()
        {
            static UWinMan _winman;
            return _winman;
        }
        void add(UButton *btnPtr)
        {
            _button = btnPtr;
        }
        void fireButtonClick(int x, int y);
    private:
        UWinMan(): _button(0) {}
        UButton *_button;
    };


    class UButton
    {
    public:
        struct ClickEventArguments
        {
            int xCoord_, yCoord_;
            ClickEventArguments(int xCoord = 0, int yCoord = 0)
            : xCoord_(xCoord), yCoord_(yCoord)
            {}
        };

        typedef huys::ADT::UVector< ClosureBase<UButton, ClickEventArguments> * > ClickEvents;
        ClickEvents _clickEvents;

    public:
        UButton()
        {
            UWinMan::instance().add(this);
        }

        virtual ~UButton()
        {
            for (ClickEvents::iterator iter = _clickEvents.begin(); iter != _clickEvents.end(); ++iter)
            {
                delete *iter;
            }
        }

        void FireClickEvents(int xc, int yc)
        {
            ClickEventArguments args(xc, yc);
            for(ClickEvents::iterator iter = _clickEvents.begin(); iter != _clickEvents.end(); iter++)
            {
                (*(*iter))(*this, args);
            }
        }
    };

    class ButtonContainer {
        UButton _button;

        void ButtonClickHandler(UButton &sender, UButton::ClickEventArguments &arguments)
        {
            std::cout <<  "Delegate invoked with sender "
                      <<  typeid(sender).name() << ", and eventArgs ("
                      <<  arguments.xCoord_ << ", " << arguments.yCoord_
                      << ")" << std::endl;
        }

    public:
        ButtonContainer()
        {
            _button._clickEvents.push_back(CreateClosure(this, &ButtonContainer::ButtonClickHandler));
        }
    };

    void UWinMan::fireButtonClick(int xc, int yc)
    {
        _button->FireClickEvents(xc, yc);
    }

    void test()
    {
        ButtonContainer handler;

        // This is supposed to be running on a different thread
        // asynchronously and under the very control of the operating system
        UWinMan::instance().fireButtonClick(10, 5);
    }

}; // namespace UDelegate

int main()
{
    test();

    UDelegate::test();

    std::cin.ignore();

    return 0;
}

