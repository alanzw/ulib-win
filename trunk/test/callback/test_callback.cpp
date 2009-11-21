#include <windows.h>
#include <iostream>
#include <cstring>
#include <cstdio>

using std::cout;
using std::endl;
using std::cin;

typedef void (*EnumProc)();

void EnumFoo(EnumProc proc)
{
	proc();
}

namespace Callback
{

struct NullType
{};


template <typename O, typename CB>
struct CB_Base
{
	CB_Base(O *pObj, CB pfCB)
	{
		s_pObj = pObj;
		s_pfCB = pfCB;
	}

	virtual ~CB_Base()
	{
		s_pObj = 0;
		s_pfCB = 0;
	}

	typedef CB CBType; 

	static O *s_pObj;
	static CB s_pfCB;
};

template <typename O, typename CB>
O * CB_Base<O, CB>::s_pObj = 0;

template <typename O, typename CB>
CB CB_Base<O, CB>::s_pfCB = 0;

template <typename O>
struct CB0 : public CB_Base<O, void (O::*)()>
{
	typedef CB_Base<O, void (O::*)()> base;

	CB0(O *pObj, void (O::*pfCB)())
		: base(pObj, pfCB)
	{}

	static void CALLBACK callback()
	{
		O *pObj = base::s_pObj;
		
		typename base::CBType pFunc = base::s_pfCB;

		(pObj->*pFunc)();
	}
};


template< class O>
void EnumFoo( O* obj, void (O::*pfMemFunc)())
{
	typedef CB0<O> adapter_t;
	adapter_t t(obj, pfMemFunc);
	::EnumFoo((EnumProc)adapter_t::callback);
}


}; // namespace Callback




struct Test
{
	void WindowLister1()
	{
		printf("Window\n");
	}
};

void WindowLister1()
{
	printf("Window\n");
}

int main()
{
	Test t;

	EnumFoo(WindowLister1);

	Callback::EnumFoo(&t, &Test::WindowLister1);

    return 0;
}

