#include <iostream>

#include "adt/uqueue.h"

namespace QueueDemo
{

    typedef huys::ADT::UQueue<char> TQueue;

    void help()
    {
        std::cout << std::endl
                  << "This program allows the user to enter one command" << std::endl
                  << "(but only one) on each input line." << std::endl
                  << "For example, if the command S is entered, then" << std::endl
                  << "the program will serve the front of the queue." << std::endl
                  << std::endl
                  << "The valid commands are : " << std::endl
                  << "A --- Append the next input character to the queue" << std::endl
                  << "S --- Serve the front of the queue" << std::endl
                  << "R --- Retrieve and print the front entry" << std::endl
                  << "# --- The current size of the queue" << std::endl
                  << "C --- Clear the queue" << std::endl
                  << "P --- Print the queue" << std::endl
                  << "H --- This help screen" << std::endl
                  << "Q --- Quit" << std::endl
                  << "Press < Enter > to continue." << std::endl;
    }

    char get_command()
    {
        char c;
        bool initial_response = true;
        do {
            if (initial_response)
            {
                std::cout << " Enter a command :  " << std::flush;
            }
            else
            {
                help();
            }

            do {
                c = std::cin.get();
            } while (c == '\n' || c == ' ' || c == '\t');

            initial_response = false;

        } while (c != 'A' && c != 'S' && c != 'R' && c != '#' && c != 'C' && c != 'P' && c != 'H' && c != 'Q' &&
                 c != 'a' && c != 's' && c != 'r' && c != 'c' && c != 'p' && c != 'h' && c != 'q');

        return c;
    }

    bool do_command(char c, TQueue &queue)
    {
        bool continue_input = true;
        TQueue::entry_type x;

        switch (c)
        {
        case 'r':
        case 'R':
            if (TQueue::underflow == queue.retrieve(x))
            {
                std::cout << "Queue is empty!" << std::endl;
            }
            else
            {
                std::cout << std::endl << "The first entry is : " << x << std::endl;
            }
            break;
        case 's':
        case 'S':
            break;
        case 'a':
        case 'A':
            break;
        case '#':
            break;
        case 'q':
        case 'Q':
            return false;
        default:
            std::cout << "Command is not implemented!" << std::endl;
        }

        return continue_input;
    }

    void test()
    {
        TQueue queue;
        help();
        while(do_command(get_command(), queue));
    }

}; // QueueDemo


namespace AirPort
{
    enum Runway_activity {idle, land, takoff};

    class Runway
    {
    public:


    };

    void test()
    {

    }

}; // AirPort

int main()
{
    using std::cout;
    using std::endl;

    typedef huys::ADT::UQueue<int> QueueInt;

    QueueInt q;

    std::cout << "Size: " << q.size() << std::endl;

    q.append(11);
    std::cout << "Size: " << q.size() << std::endl;

    q.serve();
    std::cout << "Size: " << q.size() << std::endl;

    QueueDemo::test();

    return 0;
}

