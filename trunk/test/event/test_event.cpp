#include <iostream>
#include <map>
#include <typeinfo>
#include <cmath>
#include <cstdlib>

using std::cout;
using std::endl;
using std::type_info;

namespace OldC
{
    enum EventType {
        MSG_EXPLOSION,
        MSG_TRAP
    };

    struct Event
    {
        int type;
        int pos;
        bool damage;
    };

    void handleExplosion(int pos, bool damage)
    {
        cout << "Explosion Pos : " << pos << " == " << "damage : " << damage << endl;
    }

    void handleTrap(int pos)
    {
        cout << "Trap Pos: " << pos << endl;
    }

    void handle_event(Event *event)
    {
        switch(event->type)
        {
        case MSG_EXPLOSION:
            handleExplosion(event->pos, event->damage);
            break;
        case MSG_TRAP:
            handleTrap(event->pos);
            break;
        }
    }

    void test()
    {
        Event event[] = {
            { MSG_EXPLOSION, 11, true },
            { MSG_TRAP, 22, false }
        };
        handle_event(&event[0]);
        handle_event(&event[1]);
    }

}; // namespace OldC

namespace CXX
{

class TypeInfo
{
public:
    explicit TypeInfo(const type_info& info) : _typeInfo(info) {};

    bool operator < (const TypeInfo& rhs) const
    {
        return _typeInfo.before(rhs._typeInfo) != 0;
    }

private:
    const type_info& _typeInfo;
};

class Event
{
protected:
    virtual ~Event() {}
};

class HandlerFunctionBase
{
public:
    virtual ~HandlerFunctionBase() {};
    void exec(const Event* event) {call(event);}

private:
    virtual void call(const Event*) = 0;
};


template <class T, class EventT>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
    typedef void (T::*MemberFunc)(EventT*);
    MemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

    void call(const Event* event)
    {
        (_instance->*_function)(static_cast<EventT*>(event));
    }

private:
    T* _instance;
    MemberFunc _function;
};


class EventHandler
{
public:
    ~EventHandler();
    void handleEvent(const Event*);

    template <class T, class EventT>
    void registerEventFunc(T*, void (T::*memFn)(EventT*));

private:
    typedef std::map<TypeInfo, HandlerFunctionBase*> Handlers;
    Handlers _handlers;
};


template <class T, class EventT>
void EventHandler::registerEventFunc(T* obj, void (T::*memFn)(EventT*))
{
    _handlers[TypeInfo(typeid(EventT))]= new MemberFunctionHandler<T, EventT>(obj, memFn);
}

EventHandler::~EventHandler()
{
    Handlers::iterator it = _handlers.begin();
    while(it != _handlers.end())
    {
        delete it->second;
        ++it;
    }
    _handlers.clear();
}

void EventHandler::handleEvent(const Event* event)
{
    Handlers::iterator it = _handlers.find(TypeInfo(typeid(*event)));
    if(it != _handlers.end())
    {
        it->second->exec(event);
    }
}

class Explosion : public Event
{
public:
    Explosion(int pos) : _pos(pos), _damage(60) {};

    int getPosition()    const    {return _pos;}
    int getDamage()        const    {return _damage;}

private:
    int _pos, _damage;
};

class Enemy
{
public:
    Enemy() : _name("Bad Enemy"), _damage(15) {};

    std::string getName()    const {return _name;}
    int            getDamage()    const {return _damage;}

private:
    std::string _name;
    int _damage;
};

class EnemyHit : public Event
{
public:
    EnemyHit(Enemy* enemy) : _enemy(enemy){}

    Enemy* getEnemy() const        {return _enemy;}

private:
    Enemy* _enemy;
};


class Monster : public EventHandler
{
public:
    Monster() : _pos(20), _hp(100)
    {
        registerEventFunc(this, &Monster::onExplosion);
        registerEventFunc(this, &Monster::onEnemyHit);
    }

    void receiveDamage(int dmg)
    {
        _hp -= dmg;
        std::cout << "hp: " << _hp << std::endl;
        if(_hp <= 0)
        {
            std::cout << "I'm dead" << std::endl;
        }
    }

    void onExplosion(const Explosion* explosion)
    {
        if(abs(_pos - explosion->getPosition()) < 20)
        {
            std::cout << "Hit by explosion!!!" << std::endl;
            receiveDamage(explosion->getDamage());
        }
        else
        {
            std::cout << "Out of explosion range :)" << std::endl;
        }
    }

    void onEnemyHit(const EnemyHit* hit)
    {
        Enemy* enemy = hit->getEnemy();
        std::cout << "Hit by enemy: " << enemy->getName().c_str() << " got damage: " << enemy->getDamage() << std::endl;
        receiveDamage(enemy->getDamage());
    }

    int _pos;
    int _hp;
};

class Tank
{
public:
    Tank() : _eventHandler(this) {}

    void handleEvent(const Event* event)
    {
        _eventHandler.handleEvent(event);
    }

private:
    class TankEventHandler : public EventHandler
    {
    public:
        TankEventHandler(Tank* tank) : _tank(tank)
        {
            registerEventFunc(_tank, &Tank::onDamagedByExplosion);
        }
        Tank* _tank;
    };
    friend class TankEventHandler;
    void onDamagedByExplosion(const Explosion* explosion)
    {
        std::cout << "Hit by explosion. Whatever" << std::endl;
    }
    TankEventHandler _eventHandler;
};


void test()
{
    std::cout << "=== Monster ===" << std::endl;
    Monster monster;
    monster.handleEvent(new Explosion(40));
    monster.handleEvent(new Explosion(10));
    monster.handleEvent(new EnemyHit(new Enemy));
    monster.handleEvent(new EnemyHit(new Enemy));
    monster.handleEvent(new EnemyHit(new Enemy));

    std::cout << "\n=== Tank ===" << std::endl;
    Tank tank;
    tank.handleEvent(new Explosion(40));
    tank.handleEvent(new Explosion(10));
    tank.handleEvent(new EnemyHit(new Enemy));
}

}; // namespace CXX

namespace CXXEvent
{
typedef  int CppEventHandler;

template <typename ReturnT,typename ParamT>
class EventHandlerBase1
{
public:
    virtual ReturnT notify(ParamT param) = 0;
};

template <typename ListenerT,typename ReturnT,typename ParamT>
class EventHandler1 : public EventHandlerBase1<ReturnT,ParamT>
{
    typedef ReturnT (ListenerT::*PtrMember)(ParamT);
    ListenerT* m_object;
    PtrMember m_member;

public:

    EventHandler1(ListenerT* object, PtrMember member)
        : m_object(object), m_member(member)
    {}

    ReturnT notify(ParamT param)
    {
        return (m_object->*m_member)(param);
    }
};

template <typename ReturnT,typename ParamT>
class CppEvent1
{
    typedef std::map<int,EventHandlerBase1<ReturnT,ParamT> *> HandlersMap;
    HandlersMap m_handlers;
    int m_count;

public:


    CppEvent1()
        : m_count(0) {}

    template <typename ListenerT>
    CppEventHandler attach(ListenerT* object,ReturnT (ListenerT::*member)(ParamT))
    {
        typedef ReturnT (ListenerT::*PtrMember)(ParamT);
        m_handlers[m_count] = (new EventHandler1<ListenerT,ReturnT,ParamT>(object,member));
        m_count++;
        return m_count-1;
    }

    bool detach(CppEventHandler id)
    {
        typename HandlersMap::iterator it = m_handlers.find(id);

        if(it == m_handlers.end())
            return false;

        delete it->second;
        m_handlers.erase(it);
        return true;
    }

    ReturnT notify(ParamT param)
    {
        typename HandlersMap::iterator it = m_handlers.begin();
        for(; it != m_handlers.end(); it++)
        {
            it->second->notify(param);
        }

        return true;
    }
};

// do the same for delegates with 2 parameters
template <typename ReturnT,typename ParamT,typename ParamT1>
class EventHandlerBase2
{
public:
    virtual ReturnT notify(ParamT param,ParamT1 param1) = 0;
};

template <typename ListenerT,typename ReturnT,typename ParamT,typename ParamT1>
class EventHandler2 : public EventHandlerBase2<ReturnT, ParamT, ParamT1>
{
    typedef ReturnT (ListenerT::*PtrMember)(ParamT,ParamT1);
    ListenerT* m_object;
    PtrMember m_member;

public:

    EventHandler2(ListenerT* object, PtrMember member)
        : m_object(object), m_member(member)
    {}

    ReturnT notify(ParamT param,ParamT1 param1)
    {
        return (m_object->*m_member)(param,param1);
    }
};

template <typename ReturnT,typename ParamT,typename ParamT1>
class CppEvent2
{
    typedef std::map<int,EventHandlerBase2<ReturnT,ParamT,ParamT1> *> HandlersMap;
    HandlersMap m_handlers;
    int m_count;

public:


    CppEvent2()
        : m_count(0) {}

    template <typename ListenerT>
    CppEventHandler attach(ListenerT* object,ReturnT (ListenerT::*member)(ParamT,ParamT1))
    {
        typedef ReturnT (ListenerT::*PtrMember)(ParamT,ParamT1);
        m_handlers[m_count] = (new EventHandler2<ListenerT,ReturnT,ParamT,ParamT1>(object,member));
        m_count++;
        return m_count-1;
    }

    bool detach(CppEventHandler id)
    {
        typename HandlersMap::iterator it = m_handlers.find(id);

        if(it == m_handlers.end())
            return false;

        delete it->second;
        m_handlers.erase(it);
        return true;
    }

    ReturnT notify(ParamT param,ParamT1 param1)
    {
        typename HandlersMap::iterator it = m_handlers.begin();
        for(; it != m_handlers.end(); it++)
        {
            it->second->notify(param,param1);
        }

        return true;
    }
};

// do the same for delegates with 3 parameters
template <typename ReturnT,typename ParamT,typename ParamT1,typename ParamT2>
class EventHandlerBase3
{
public:
    virtual ReturnT notify(ParamT param,ParamT1 param1,ParamT2 param2) = 0;
};

template <typename ListenerT,typename ReturnT,typename ParamT,typename ParamT1,typename ParamT2>
class EventHandler3 : public EventHandlerBase3<ReturnT,ParamT,ParamT1,ParamT2>
{
    typedef ReturnT (ListenerT::*PtrMember)(ParamT,ParamT1,ParamT2);
    ListenerT* m_object;
    PtrMember m_member;

public:

    EventHandler3(ListenerT* object, PtrMember member)
        : m_object(object), m_member(member)
    {}

    ReturnT notify(ParamT param,ParamT1 param1,ParamT2 param2)
    {
        return (m_object->*m_member)(param,param1,param2);
    }
};

template <typename ReturnT,typename ParamT,typename ParamT1,typename ParamT2>
class CppEvent3
{
    typedef std::map<int,EventHandlerBase3<ReturnT,ParamT,ParamT1,ParamT2> *> HandlersMap;
    HandlersMap m_handlers;
    int m_count;

public:


    CppEvent3()
        : m_count(0) {}

    template <typename ListenerT>
    CppEventHandler attach(ListenerT* object,ReturnT (ListenerT::*member)(ParamT,ParamT1,ParamT2))
    {
        typedef ReturnT (ListenerT::*PtrMember)(ParamT,ParamT1,ParamT2);
        m_handlers[m_count] = (new EventHandler3<ListenerT,ReturnT,ParamT,ParamT1,ParamT2>(object,member));
        m_count++;
        return m_count-1;
    }

    bool detach(CppEventHandler id)
    {
        typename HandlersMap::iterator it = m_handlers.find(id);

        if(it == m_handlers.end())
            return false;

        delete it->second;
        m_handlers.erase(it);
        return true;
    }

    ReturnT notify(ParamT param,ParamT1 param1,ParamT2 param2)
    {
        typename HandlersMap::iterator it = m_handlers.begin();
        for(; it != m_handlers.end(); it++)
        {
            it->second->notify(param,param1,param2);
        }

        return true;
    }
};

class MySubject
{
public:

    CppEvent1<bool,int> int_event;
    CppEvent2<bool,double,int> double_event;
    CppEvent3<bool,double,int,const char*> triple_event;

    void submit_int()
    {
        int_event.notify(1);
    }

    void submit_double()
    {
        double_event.notify(10.5,100);
    }

    void submit_triple()
    {
        triple_event.notify(10.5,100,"Oh ye");
    }
};

class MyListener1
{
public:

    bool update_int(int p)
    {
        cout << "int update listener 1" << endl;
        return true;
    }

    bool update_double(double p,int p1)
    {
        cout << "double update listener 1" << endl;
        return true;
    }

    bool update_triple(double p,int p1,const char* str)
    {
        cout << "triple update listener 1" << endl;
        return true;
    }
};

class MyListener2
{
public:

    bool fun(int p)
    {
        cout << "fun update listener 2" << endl;
        return true;
    }
};

void test()
{
    // TODO: Please replace the sample code below with your own.
    cout << "Hello World" << endl;
    MyListener1* listener1 = new MyListener1;
    MyListener2* listener2 = new MyListener2;

    MySubject subject;

    CppEventHandler h1 = subject.int_event.attach(listener1,&MyListener1::update_int);
    CppEventHandler h2 = subject.int_event.attach(listener2,&MyListener2::fun);

    CppEventHandler h3 = subject.double_event.attach(listener1,&MyListener1::update_double);
    CppEventHandler h4 = subject.triple_event.attach(listener1,&MyListener1::update_triple);

    subject.submit_int();
    subject.submit_double();
    subject.submit_triple();

    // detach handlers
    subject.int_event.detach(h1);
    subject.int_event.detach(h2);
    subject.double_event.detach(h3);
    subject.triple_event.detach(h4);
}

};

int main()
{
    cout << "======= C++ Event  Handling Testing ==================" << endl;

    OldC::test();

    cout << "-------------------------------------------------------------" << endl;

    CXX::test();

    cout << "-------------------------------------------------------------" << endl;

    CXXEvent::test();

    return 0;
}

