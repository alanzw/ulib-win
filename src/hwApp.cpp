#include "hwApp.h"

hwAppConsole::hwAppConsole()
{}

hwAppConsole::~hwAppConsole()
{}

bool hwAppConsole::init(int argc, hwChar **argv)
{
    _argc = argc;
    _argv = argv;

    return onInit();
}

bool hwAppConsole::onInit()
{
    return true;
}

bool hwAppConsole::onExit()
{
    return true;
}

bool hwAppConsole::onCleanup()
{
    return true;
}
