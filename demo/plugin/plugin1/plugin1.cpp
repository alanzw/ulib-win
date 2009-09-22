#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "plugin1.h"

UPlugin::UPlugin()
: m_name("dynamic")
{}

UPlugin::~UPlugin()
{}

string UPlugin::getName() const
{
    return m_name;
}

void UPlugin::setName(const char *name)
{
    m_name = name;
}

IPluginHost UPlugin::getHost() const
{
    return m_iph;
}

void UPlugin::setHost(const IPluginHost &iph)
{
    m_iph = iph;
    m_iph.registerIt(this);
}

void UPlugin::go()
{
    printf("Hello from plugin : %s\n", m_name.c_str());
}

IPlugin *retrieveIPP()
{
    return new UPlugin;
}
