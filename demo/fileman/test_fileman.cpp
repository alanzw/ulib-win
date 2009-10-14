#include "resource.h"

#include "ufileman.h"

int main(int argc, char *argv[])
{

    UFileMan ufm;

    ufm.printFile("test.txt");
    
    ufm.listDirFiles("C:\\*");

    return 0;
}

