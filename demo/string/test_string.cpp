#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include <iostream>

using namespace std;

#include "uwinchar.h"

template <typename T>
struct is_char
{
    static const bool value = false;
};

template <>
struct is_char<char>
{
    static const bool value = true;
};

template <typename T>
bool check_type(T val)
{
    return is_char<T>::value;
}

int main(int argc, char *argv[])
{
    char aChar[] = "SAX";
    char wChar[] = _T("жа");

    if (huys::IsCharAlpha(aChar[0]))
    {
        cout << "OK!" << endl;
        cout << aChar;
        cout << " --> ";
        cout << huys::CharLower(aChar) << endl;
    }
    else
    {
        cout << "Failed!" << endl;
    }

    if (huys::IsCharAlpha(wChar[0]))
    {
        cout << "OK!" << endl;
    }
    else
    {
        cout << "Failed!" << endl;
    }

    if (!check_type(1))
    {
        cout << "Not char!" << endl;
    }

    if (check_type('1'))
    {
        cout << "Oh, it is a char!" << endl;
    }

    cin.ignore();
    return 0;
}


