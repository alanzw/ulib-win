#include <windows.h>
#include <tchar.h>

#include <cstdio>
#include <climits>

#include "uini.h"

int main()
{
    UIni uini;

    // uini.open("test.ini");

    uini.parseLine("[section1]");

    uini.parseLine("; Hello Comment");
    uini.parseLine("// Hello Comment");

    uini.parseLine("include ../hello.ini #include");

    uini.parseLine("key=value");

    uini.printAll();

    uini.deleteAll();

    uini.parseFile("test.ini");

    getchar();
    return 0;

}
