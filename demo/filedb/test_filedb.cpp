#include "resource.h"

#include <windows.h>
#include <tchar.h>

#include "ufiledb.h"

int main(int argc, char *argv[])
{

    UFileDB udb;
    UFileDB::DBData data = {
        "test", "go"
    };
    udb.add(data);

    udb.save();

    return 0;
}

