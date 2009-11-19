/*
* =====================================================================================
*
*       Filename:  test_proxy.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  09/27/2009 08:38:34 PM
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  huys (hys), huys03@gmail.com
*        Company:  hu
*
* =====================================================================================
*/
#include <iostream>
#include <string>

using namespace std;


namespace Before_DP
{
    class Image
    {
        int m_id;
        static int s_next;
    public:
        Image()
        {
            m_id = s_next++;
            cout << "	$$ ctor: " << m_id << '\n';
        }
        ~Image()
        {
            cout << "	dtor: " << m_id << '\n';
        }
        void draw()
        {
            cout << "	drawing image " << m_id << '\n';
        }
    };
    int Image::s_next = 1;
    
    void test()
    {
        Image images[5];
        
        for (int i; true;)
        {
            cout << "Exit[0], Image[1-5]: ";
            cin >> i;
            if (i == 0)
            break;
            images[i - 1].draw();
        }
    }


}; // namespace Before_DP


namespace After_DP
{

    class RealImage
    {
        int m_id;
    public:
        RealImage(int i)
        {
            m_id = i;
            cout << "   $$ ctor: " << m_id << '\n';
        }
        ~RealImage()
        {
            cout << "   dtor: " << m_id << '\n';
        }
        void draw()
        {
            cout << "   drawing image " << m_id << '\n';
        }
    };

    // 1. Design an "extra level of indirection" wrapper class
    class Image
    {
        // 2. The wrapper class holds a pointer to the real class
        RealImage *m_the_real_thing;
        int m_id;
        static int s_next;
    public:
        Image()
        {
            m_id = s_next++;
            // 3. Initialized to null
            m_the_real_thing = 0;
        }
        ~Image()
        {
            delete m_the_real_thing;
        }
        void draw()
        {
            // 4. When a request comes in, the real object is
            //    created "on first use"
            if (!m_the_real_thing)
            {
                m_the_real_thing = new RealImage(m_id);
            }
            // 5. The request is always delegated
            m_the_real_thing->draw();
        }
    };
    int Image::s_next = 1;

    void test()
    {
        Image images[5];

        for (int i; true;)
        {
            cout << "Exit[0], Image[1-5]: ";
            cin >> i;
            if (i == 0)
            break;
            images[i - 1].draw();
        }
    }


}; // namespace After_DP

namespace Test
{

    class Person
    {
        string nameString;
        static string list[];
        static int next;
    public:
        Person()
        {
            nameString = list[next++];
        }
        string name()
        {
            return nameString;
        }
    };

    string Person::list[] =
    {
        "Tom", "Dick", "Harry", "Bubba"
    };

    int Person::next = 0;

    class PettyCashProtected
    {
        int balance;
    public:
        PettyCashProtected()
        {
            balance = 500;
        }
        bool withdraw(int amount)
        {
            if (amount > balance)
            return false;
            balance -= amount;
            return true;
        }
        int getBalance()
        {
            return balance;
        }
    };

    class PettyCash
    {
        PettyCashProtected realThing;
    public:
        bool withdraw(Person &p, int amount)
        {
            if (p.name() == "Tom" || p.name() == "Harry" || p.name() == "Bubba")
            {
                return realThing.withdraw(amount);
            }
            else
            {
                return false;
            }
        }
        int getBalance()
        {
            return realThing.getBalance();
        }
    };


    void test()
    {
        PettyCash pc;
        Person workers[4];
        for (int i = 0, amount = 100; i < 4; i++, amount += 100)
        {
            if (!pc.withdraw(workers[i], amount))
            {
                cout << "No money for " << workers[i].name() << '\n';
            }
            else
            {
                cout << amount << " dollars for " << workers[i].name() << '\n';
            }
        }
        cout << "Remaining balance is " << pc.getBalance() << '\n';

    }

}; // namespace Test


namespace DP_Ext
{

    class Subject
    {
    public:
        virtual void execute() = 0;
    };

    class RealSubject: public Subject
    {
        string str;
    public:
        RealSubject(string s)
        {
            str = s;
        }
        /*virtual*/void execute()
        {
            cout << str << '\n';
        }
    };

    class ProxySubject: public Subject
    {
        string first, second, third;
        RealSubject *ptr;
    public:
        ProxySubject(string s)
        {
            int num = s.find_first_of(' ');
            first = s.substr(0, num);
            s = s.substr(num + 1);
            num = s.find_first_of(' ');
            second = s.substr(0, num);
            s = s.substr(num + 1);
            num = s.find_first_of(' ');
            third = s.substr(0, num);
            s = s.substr(num + 1);
            ptr = new RealSubject(s);
        }
        ~ProxySubject()
        {
            delete ptr;
        }
        RealSubject *operator->()
        {
            cout << first << ' ' << second << ' ';
            return ptr;
        }
        /*virtual*/void execute()
        {
            cout << first << ' ' << third << ' ';
            ptr->execute();
        }
    };

    void test()
    {
        ProxySubject obj(string("the quick brown fox jumped over the dog"));
        obj->execute();
        obj.execute();
    }

}; // DP_Ext

int main()
{
    Before_DP::test();

    After_DP::test();

    Test::test();
    
    DP_Ext::test();
    
    return 0;
}


