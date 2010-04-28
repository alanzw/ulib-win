#include "hwApp.h"

int hwMain(int argc, hwCharPtrPtr argv)
{
    hwAppConsole *app;

    if (!hwAppInitializer::_p)
    {
        app = new hwAppConsole();
    }
    else
    {
        app = (*hwAppInitializer::_p)();
    }

    if (!app->init(argc, argv))
    {
        return -1;
    }

    if (!app->onRun())
    {
        return -2;
    }

    if (!app->onCleanup())
    {
        return -3;
    }

    if (!app->onExit())
    {
        return -4;
    }

    return 0;
}

PtrFunc hwAppInitializer::_p = NULL;

hwAppConsole * hwAppConsole::ms_appInstance = NULL;

hwAppConsole::hwAppConsole()
{}

hwAppConsole::~hwAppConsole()
{}

bool hwAppConsole::init(int argc, hwCharPtrPtr argv)
{
    m_argc = argc;
    m_argv = argv;

    return onInit();
}

int hwAppConsole::getArgc() const
{
    return m_argc;
}

hwCharPtrPtr hwAppConsole::getArgv() const
{
    return m_argv;
}

bool hwAppConsole::onInit()
{
    return true;
}

bool hwAppConsole::onExit()
{
    return true;
}

bool hwAppConsole::onRun()
{
    return true;
}

bool hwAppConsole::onCleanup()
{
    return true;
}

hwAppGUI::hwAppGUI()
{

}

hwAppGUI::~hwAppGUI()
{

}

