#include <windows.h>
#include <tchar.h>

#include <cstdio>
#include <climits>

#include "uini.h"

int main()
{
    UIni uini;

    // uini.open("test.ini");
    uini.parseLine("include ./test.inc #include");

    uini.parseLine("[section1]");

    uini.parseLine("; Hello Comment");
    uini.parseLine("// Hello Comment");

    uini.parseLine("key = value");

    uini.printAll();

    uini.writeFile("./new_test.ini", true);
    printf("=========================================\n");
    const char *val = NULL;
    int nRet = uini.query("key", val);
    printf("Query > %s << %d >> %s >\n", "key", nRet, val);
    val = NULL;
    nRet = uini.query("happy", val);
    printf("Query > %s << %d >> %s >\n", "happy", nRet, val);
    printf("=========================================\n");

    uini.deleteAll();

    uini.parseFile("test.ini");

    printf("=========================================\n");
    val = NULL;
    nRet = uini.query("key", val);
    printf("Query > %s << %d >> %s >\n", "key", nRet, val);
    val = NULL;
    nRet = uini.query("key1", val);
    printf("Query > %s << %d >> %s >\n", "key1", nRet, val);
    val = NULL;
    nRet = uini.query("LogOutput", val);
    printf("Query > %s << %d >> %s >\n", "key1", nRet, val);
    printf("=========================================\n");

    getchar();
    return 0;
}
