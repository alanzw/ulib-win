#ifndef U_SUI_H
#define U_SUI_H

#include <utility>
#include <map>
#include <string>

#include "ulua.h"

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

/* 1 Argument, no return value */
template <class P1> class Functor1 : public FunctorBase
{
public:
	Functor1() {}
	void operator () (P1 p1) const
	{
		thunk (*this, p1);
	}
protected:
	typedef void (*Thunk) (const FunctorBase &, P1);
	Functor1 (Thunk t, const void *c, PFunc f, const void *mf, size_t sz)
		: FunctorBase (c, f, mf, sz), thunk (t) { }
private:
	Thunk thunk;
};

template <class P1, class Callee, class MemFunc>
class MemberTranslator1 : public Functor1<P1>
{
public:
	MemberTranslator1 (Callee & c, const MemFunc & m)
		: Functor1<P1> (thunk, &c, 0, &m, sizeof (MemFunc)) { }
	static void thunk (const FunctorBase & ftor, P1 p1)
	{
		Callee *callee = (Callee *) ftor.getCallee ();
		MemFunc & memFunc (*(MemFunc *) (void *)(ftor.getMemFunc ()));
		(callee->*memFunc) (p1);
	}
};

template <class P1, class Func> 
class FunctionTranslator1 : public Functor1 <P1>
{
public:
	FunctionTranslator1 (Func f) : Functor1 <P1> (thunk, 0, f, 0, 0) { }
	static void thunk (const FunctorBase & ftor, P1 p1)
	{
		(Func (ftor.func)) (p1);
	}
};

template <class P1, class Callee, class TRT, class CallType>
inline MemberTranslator1 <P1, Callee, TRT (CallType::*)(P1)>
makeFunctor (Callee & c, TRT (CallType::* f) (P1))
{
	typedef TRT (CallType::*MemFunc) (P1);
	return MemberTranslator1 <P1, Callee, MemFunc> (c, f);
}

template <class P1, class Callee, class TRT, class CallType, class TP1>
inline MemberTranslator1 <P1, const Callee, TRT (CallType::*) (TP1) const>
makeFunctor (const Callee & c, TRT (CallType::*const &f) (TP1) const)
{
	typedef TRT (CallType::*MemFunc) (TP1) const;
	return MemberTranslator1 <P1, const Callee, MemFunc> (c, f);
}

template <class P1, class TRT, class TP1>
inline FunctionTranslator1 <P1, TRT (*)(TP1)>
makeFunctor (TRT (*f) (TP1))
{
	return FunctionTranslator1 <P1, TRT (*)(TP1) > (f);
}

template <class P1, class MemFunc> class MemberOf1stArgTranslator1 
: public Functor1 <P1>
{
public:
	MemberOf1stArgTranslator1 (const MemFunc & m) :
	  Functor1 < P1 > (thunk, (void *)1, 0, &m, sizeof (MemFunc)) { }
	  static void thunk (const FunctorBase & ftor, P1 p1)
	  {
		  MemFunc & memFunc (*(MemFunc *) (void *)(ftor.memFunc));
		  (p1.*memFunc) ();
	  }
};

template <class P1, class TRT, class CallType>
inline MemberOf1stArgTranslator1 <P1, TRT (CallType::*)()>
makeFunctor (TRT (CallType::* &f) ())
{
	typedef TRT (CallType::*MemFunc) ();
	return MemberOf1stArgTranslator1 <P1, MemFunc> (f);
}

template <class P1, class TRT, class CallType>
inline MemberOf1stArgTranslator1 < P1, TRT (CallType::*)() const>
makeFunctor (TRT (CallType::*const &f) () const)
{
	typedef TRT (CallType::*MemFunc) () const;
	return MemberOf1stArgTranslator1 <P1, MemFunc> (f);
}

//template <class T>
class USui
{
public:
    //typedef lua_CFunction sui_cb;
    //typedef void (T::*cbFoo)(bool param);
    //static bool comp(int l, int r) {return false;}
    //typedef bool (T::*cbFunc)();

    static int lua_event(lua_State* luaVM)
    {
        USui *p = USui::instance();

        //if (NULL == p)
        //{
        //    return -1;
        //}
        //T *p = (T *)lua_touserdata(luaVM, 1);
        const char *fooName = lua_tostring(luaVM, 1);
        bool param = lua_toboolean(luaVM, 2);
        p->call(fooName, param);

        return 0;
    }

    static USui * instance()
    {
        static USui usui;

        return &usui;
    }

    /*
    void set(T *p)
    {
        m_pClass = p;
    }
    */
    int call(const char *sNameCb, bool param)
    {
        std::string str = sNameCb;
        //if (std::map<std::string, cbFoo>::end == cb_table.find(str))
        //{
        //    return -1;
        //}

        //(m_pClass->*cb_table[str])(param);
        cb_table[str](param);
        return 0;
    }

    int insert(const char *sNameCb, Functor1<bool> f)
    {
        cb_table.insert(std::pair<std::string, Functor1<bool> >(sNameCb, f));
        return 0;
    }  
    
    /*
    int insert(const char *sNameCb, cbFoo f)
    {
        cb_table.insert(std::pair<std::string, cbFoo>(sNameCb, f));
        return 0;
    }
    */
    
    int doString(const char *s)
    {
        return m_l.doString(s);
    }

    int doFile(const char *sFilename)
    {
        return m_l.doFile(sFilename);
    }

private:
    ULua m_l;
    //T * m_pClass;

    //std::map<std::string, cbFoo> cb_table;
    std::map<std::string, Functor1<bool> > cb_table;
private:
    USui()
    //: m_pClass(NULL)
    {
        m_l.initialize();
        m_l.registerCallback("USui_Event", &USui::lua_event);
    }

    ~USui()
    {
        m_l.finalize();
    }
};


#endif // U_SUI_H
