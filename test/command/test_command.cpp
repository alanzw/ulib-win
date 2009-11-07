/*
* Command
*  -- Invoker
*  -- Client
*  -- Receiver
*/

/* Uses
* -- Multi-level undo
* -- Transactional behavior
* -- Progress bars
* -- Wizards
* -- GUI buttons and menu items
* -- Thread pools
* -- Macro recording
* -- Networking
* -- Parallel Processing
* -- Mobile Code
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ios>
#include <bitset>
#include <sstream>
#include <map>
#include <cctype>

using namespace std;

namespace DP
{

    class Person;

    class Command
    {
        // 1. Create a class that encapsulates an object and a member function
        // a pointer to a member function (the attribute's name is "method")
        Person *object; //
        void(Person:: *method)();
    public:
        Command(Person *obj = 0, void(Person:: *meth)() = 0)
        {
            object = obj; // the argument's name is "meth"
            method = meth;
        }
        void execute();
    };

    class Person
    {
        string name;

        // cmd is a "black box", it is a method invocation
        // promoted to "full object status"
        Command cmd;
    public:
        Person(string n, Command c): cmd(c)
        {
            name = n;
        }
        void talk()
        {
            // "this" is the sender, cmd has the receiver
            cout << name << " is talking" << endl;
            cmd.execute(); // ask the "black box" to callback the receiver
        }
        void passOn()
        {
            cout << name << " is passing on" << endl;

            // 4. When the sender is ready to callback to the receiver,
            // it calls execute()
            cmd.execute();
        }
        void gossip()
        {
            cout << name << " is gossiping" << endl;
            cmd.execute();
        }
        void listen()
        {
            cout << name << " is listening" << endl;
        }
    };

    void Command::execute()
    {
        (object->*method)(); // invoke the method on the object
    }

    void test()
    {
        // Fred will "execute" Barney which will result in a call to passOn()
        // Barney will "execute" Betty which will result in a call to gossip()
        // Betty will "execute" Wilma which will result in a call to listen()
        Person wilma("Wilma", Command());
        // 2. Instantiate an object for each "callback"
        // 3. Pass each object to its future "sender"
        Person betty("Betty", Command(&wilma, &Person::listen));
        Person barney("Barney", Command(&betty, &Person::gossip));
        Person fred("Fred", Command(&barney, &Person::passOn));
        fred.talk();
    }

}; // namespace DP

namespace Before_DP
{

    class Giant
    {
    public:
        enum Type
        {
            Fee, Phi, Pheaux
        };
        Giant()
        {
            m_id = s_next++;
            m_type = (Type)(m_id % 3);
        }
        Type get_type()
        {
            return m_type;
        }
        void fee()
        {
            cout << m_id << "-fee  ";
        }
        void phi()
        {
            cout << m_id << "-phi  ";
        }
        void pheaux()
        {
            cout << m_id << "-pheaux  ";
        }
    private:
        Type m_type;
        int m_id;
        static int s_next;
    };
    int Giant::s_next = 0;

    template <typename T>
    class Queue
    {
    public:
        Queue()
        {
            m_add = m_remove = 0;
        }
        void enque(T *c)
        {
            m_array[m_add] = c;
            m_add = (m_add + 1) % SIZE;
        }
        T *deque()
        {
            int temp = m_remove;
            m_remove = (m_remove + 1) % SIZE;
            return m_array[temp];
        }
    private:
        enum { SIZE = 8 };
        T *m_array[SIZE];
        int m_add, m_remove;
    };

    void test()
    {
        Queue<Giant> que;
        Giant input[6],  *bad_guy;

        for (int i = 0; i < 6; i++)
        {
            que.enque(&input[i]);
        }
        for (int i = 0; i < 6; i++)
        {
            bad_guy = que.deque();
            if (bad_guy->get_type() == Giant::Fee)
            {
                bad_guy->fee();
            }
            else if (bad_guy->get_type() == Giant::Phi)
            {
                bad_guy->phi();
            }
            else if (bad_guy->get_type() == Giant::Pheaux)
            {
                bad_guy->pheaux();
            }
        }
        cout << '\n';
    }

}; // namespace Before_DP

namespace After_DP
{
    class Giant
    {
    public:
        Giant()
        {
            m_id = s_next++;
        }
        void fee()
        {
            cout << m_id << "-fee  ";
        }
        void phi()
        {
            cout << m_id << "-phi  ";
        }
        void pheaux()
        {
            cout << m_id << "-pheaux  ";
        }
    private:
        int m_id;
        static int s_next;
    };
    int Giant::s_next = 0;

    class Command
    {
    public:
        typedef void(Giant:: *Action)();
        Command(Giant *object, Action method)
        {
            m_object = object;
            m_method = method;
        }

        void execute()
        {
            (m_object->*m_method)();
        }
    private:
        Giant *m_object;
        Action m_method;
    };

    template <typename T>
    class Queue
    {
    public:
        Queue()
        {
            m_add = m_remove = 0;
        }

        void enque(T *c)
        {
            m_array[m_add] = c;
            m_add = (m_add + 1) % SIZE;
        }

        T *deque()
        {
            int temp = m_remove;
            m_remove = (m_remove + 1) % SIZE;
            return m_array[temp];
        }
    private:
        enum { SIZE = 8 };
        T *m_array[SIZE];
        int m_add, m_remove;
    };

    void test()
    {
        Queue<Command> que;
        Command *input[] =
        {
            new Command(new Giant, &Giant::fee),
            new Command(new Giant, &Giant::phi),
            new Command(new Giant, &Giant::pheaux),
            new Command(new Giant, &Giant::fee),
            new Command(new Giant, &Giant::phi),
            new Command(new Giant, &Giant::pheaux)
        };

        for (int i = 0; i < 6; i++)
        {
            que.enque(input[i]);
        }
        for (int i = 0; i < 6; i++)
        {
            que.deque()->execute();
        }
        cout << '\n';
    }


}; // namespace After_DP

namespace Menu
{

class MenuCommand
{
public:
    MenuCommand(const string name)
    : _sName(name)
    {}

    virtual ~MenuCommand() {};
    virtual bool execute() = 0;
    string get_name()
    {
        return _sName;
    }
private:
    //Protect copy ctor and assignment.
    MenuCommand(const MenuCommand&);
    MenuCommand& operator=(const MenuCommand& rho);
    const string _sName;
};

class QuitCmd : public MenuCommand
{
public:
    QuitCmd()
    : MenuCommand("Quit Application")
    {}

    bool execute()
    {
        return false;
    }
private:
    QuitCmd(const QuitCmd&);
    QuitCmd& operator=(const QuitCmd& rho);
};

class ReturnCmd : public MenuCommand
{
public:
    ReturnCmd()
    : MenuCommand("Return To Previous Menu")
    {}
    // if bool = false: return to previous level.
    //If top level then quit app.
    bool execute()
    {
        return false;
    }
private:
    ReturnCmd(const ReturnCmd&);
    ReturnCmd& operator=(const ReturnCmd& rho);
};

class PrintCmd : public MenuCommand
{
public:
    PrintCmd()
    : MenuCommand("Print")
    {}

    bool execute()
    {
        cout << "I am printing stuff :)" << endl;
        return true;
    }
private:
    PrintCmd(const PrintCmd&);
    PrintCmd& operator=(const PrintCmd& rho);
};

class MenuManager : public MenuCommand
{
    struct Delete
    {
        template <typename T>
        void operator()(const T* ptr) const
        {
            std::cout << "\tDeleting address: " << std::ios::hex  << ptr << std::endl;
            delete ptr;
        }
    };
public:
    MenuManager(const string name)
    : MenuCommand(name)
    {}

    ~MenuManager()
    {
        std::cout << "Deleting: " << get_name() << std::endl;
        std::for_each(_vecMenuItemCommands.begin(), _vecMenuItemCommands.end(), Delete());
    }

    void add(MenuCommand* pCommand)
    {
        _vecMenuItemCommands.push_back(pCommand);
    }

    bool execute()
    {
        MenuCommand* pCmd;
        do
        {
            show();
            int userInput = get_user_input();
            pCmd = select( userInput );
        } while( pCmd->execute() );
        return true;
    }
private:
    MenuManager(const MenuManager&);
    MenuManager& operator=(const MenuManager& rho);

    MenuCommand* select(int index)
    {
        return _vecMenuItemCommands[index-1];
    }

    void show()
    {
        cout << endl << "*** " << get_name() << " ***" << endl;
        std::size_t size = _vecMenuItemCommands.size();
        for(std::size_t i=0; i < size; ++i)
        {
            cout << i+1 << ". " << _vecMenuItemCommands[i]->get_name() << endl;
        }
    }

    int get_user_input()
    {
        unsigned input = 0;
        while(input < 1 || input > ( _vecMenuItemCommands.size() ) )
        {
            cout << "Please select an item 1-" << (_vecMenuItemCommands.size()) << endl << ">";
            cin >> input;
        }
        return input;
    }

    vector<MenuCommand*> _vecMenuItemCommands;
};

void test()
{
    //Setup main menu first. Add submenu's here later.
    MenuManager MainMenu("Main Menu");

    //Setup a single sub menu.
    MenuManager* FileMenu = new MenuManager("File Menu");
    //Add concrete menu's to the sub menu.
    FileMenu->add(new PrintCmd);
    FileMenu->add(new ReturnCmd);
    //And add the sub menu to the main menu.
    MainMenu.add(FileMenu);
    //Also let's add a concrete quit command to the main menu to exit the app.
    MainMenu.add(new QuitCmd);
    //Run the menu.
    MainMenu.execute();
}

}; // namespace Menu

namespace Convert
{

enum { CHAR_BIT = 8 };

class converter
{
public:
    virtual std::string convert(int) = 0;
    virtual ~converter() {}
};

class hex_converter : public converter
{
public:
    std::string convert(int i)
    {
        std::stringstream ss;
        ss << std::hex << i;
        return ss.str();
    }
};

class oct_converter : public converter
{
public:
    std::string convert(int i)
    {
        std::stringstream ss;
        ss << std::oct << i;
        return ss.str();
    }
};

class bin_converter : public converter
{
public:
    std::string convert(int i)
    {
        std::bitset< sizeof(i) * CHAR_BIT > bits(i);
        std::stringstream ss;
        ss << bits;
        return ss.str();
    }
};

class dictionary
{
    std::map<char, converter*> dict;
public:
    dictionary()
    {
        dict.insert( std::make_pair( 'B', new bin_converter ) );
        dict.insert( std::make_pair( 'O', new oct_converter ) );
        dict.insert( std::make_pair( 'H', new hex_converter ) );
    }

    converter* lookup(char x)
    {
        std::map<char, converter*>::const_iterator iter;
        iter = dict.find( toupper(x) );
        if( iter != dict.end() )
            return iter->second;
        else
            return NULL;
    }

    ~dictionary()
    {
        while( dict.begin() != dict.end() )
        {
            delete dict.begin()->second;
            dict.erase( dict.begin() );
        }
    }
};

void test()
{
    using namespace std;

    int num = 1234; 
    char ch = 'h';
    string output = "";
    dictionary dict;

    converter* con = dict.lookup( ch );

    if( con != NULL )
        output = con->convert( num );
    else
        output = "Invalid";

    cout << "Result: " << output;
}

}; // namespace Convert


int main()
{
    DP::test();

    cout << "--------------------------------------------" << endl;

    Before_DP::test();

    cout << "--------------------------------------------" << endl;

    After_DP::test();

    cout << "--------------------------------------------" << endl;

    Menu::test();

    cout << "--------------------------------------------" << endl;

    Convert::test();

    return 0;
}
