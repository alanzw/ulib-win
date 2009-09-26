#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include "plugin1.h"

static UPlugin * s_ipp = NULL;

UPlugin::UPlugin()
: m_name("dynamic")
{}

UPlugin::~UPlugin()
{}

const char * UPlugin::getName() const
{
    return m_name.c_str();
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
    assert(NULL == s_ipp);
    return (s_ipp = new UPlugin);
}

void freeIPP()
{
    CHECK_PTR(s_ipp);
}

