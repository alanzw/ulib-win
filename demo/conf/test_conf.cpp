#include "resource.h"

#include <iostream>
#include <string>

using namespace std;

#include "uconf.h"

int main(int argc, char *argv[])
{
    UConfig uc;
    uc.read();
    uc.getOption("xx");

    cout << "=================" << endl;

    uc.changeOptionValue("xx", "44");
    uc.getOption("xx");
    uc.changeOptionValue("tt", "jjjj");
    uc.getOption("tt");
    uc.writeNewFile("new.ini");

    cin.ignore();
    return 0;
}


