#ifndef U_PLUGIN_H
#define U_PLUGIN_H

#include "ulib.h"

#include <string>

using std::string;

UINTERFACE IPluginHost;

UINTERFACE ULIB_API IPlugin
{
    virtual string getName() const = 0;
    virtual void setName(const char *name) = 0;
    //
    virtual IPluginHost getHost() const = 0;
    virtual void setHost(const IPluginHost &iph) = 0;
    //
    virtual void go() = 0;
};

UINTERFACE ULIB_API IPluginHost
{
    bool registerIt(IPlugin *ip);
};

typedef IPlugin *(*LPRetrieveIP)();

#endif // U_PLUGIN_H
