#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

void handle_unknown(char c)
{
    cout << "!!! unknow command !!!" << endl;
}

void handle_quit()
{
    cout << "Quit ... " << endl;
    exit(0);
}

void dispatch(char c)
{
    switch (c)
    {
    case 'q':
    case 'Q':
        handle_quit();
        break;
    default:
        handle_unknown(c);
    }
}

int main()
{
    char cmd;
    while (1)
    {
        cin >> cmd;
        dispatch(cmd);
        cin.ignore( 1024, '\n' );
    }
    return 0;
}
