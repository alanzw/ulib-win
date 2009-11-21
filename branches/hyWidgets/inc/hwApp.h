#ifndef HW_APP_H
#define HW_APP_H

#include "hwDefs.h"

class HW_API hwAppConsole
{
public:
    hwAppConsole();
    
    virtual ~hwAppConsole();
    
    virtual bool init(int argc, hwCharPtrPtr argv);
    
    virtual bool onInit();
    
    virtual bool onRun() = 0;
    
    virtual bool onExit();
    
    virtual bool onCleanup();
private:
    int _argc;
    hwCharPtrPtr _argv;
};


#define IMPLEMENT_HWAPP(appname)    \
    hwAppConsole * hwCreateApp()    \
    {                               \
        return new appname;         \
    }

#ifdef HW_MSW
  #include "msw/hwApp.h"
#endif
    
#endif // HW_APP_H
