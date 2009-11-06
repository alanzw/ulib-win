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

	int getPosition()	const	{return _pos;}
	int getDamage()		const	{return _damage;}

private:
	int _pos, _damage;
};

class Enemy
{
public:
	Enemy() : _name("Bad Enemy"), _damage(15) {};
	
	std::string getName()	const {return _name;}
	int			getDamage()	const {return _damage;}

private:
	std::string _name;
	int _damage;
};

class EnemyHit : public Event
{
public:
	EnemyHit(Enemy* enemy) : _enemy(enemy){}

	Enemy* getEnemy() const		{return _enemy;}

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


int main()
{
    cout << "======= C++ Event  Handling Testing ==================" << endl;

    OldC::test();
    
    cout << "-------------------------------------------------------------" << endl;
    
    CXX::test();

    return 0;
}

