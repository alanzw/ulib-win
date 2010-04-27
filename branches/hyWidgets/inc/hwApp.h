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

    virtual bool onRun();

    virtual bool onExit();

    virtual bool onCleanup();

public:
    //static hwAppConsole *GetInstance() { return ms_appInstance; }
    //static void SetInstance(hwAppConsole *p) { ms_appInstance = p;}

    static hwAppConsole *ms_appInstance;
private:
    int _argc;
    hwCharPtrPtr _argv;
};

HW_API int hwMain(int argc, hwCharPtrPtr argv);

class HW_API hwAppGUI : public hwAppConsole
{
public:
    hwAppGUI();

    virtual ~hwAppGUI();
};

typedef hwAppConsole * (*PtrFunc)();

class HW_API hwAppInitializer
{
public:
    hwAppInitializer(){}

    hwAppInitializer(PtrFunc p)
    {
        _p = p;
    };

    static PtrFunc _p;
};

#define IMPLEMENT_HWAPP_NOMAIN(appname)                         \
    hwAppConsole * hwCreateApp()                         \
    {                                                    \
        return new appname;                              \
    };                                                   \
    hwAppInitializer g_hwAppInit(&hwCreateApp);

#ifdef HW_CONSOLE
#define IMPLEMENT_HWAPP_WINMAIN \
    extern "C" int _tmain(int argc, TCHAR **argv) \
{ \
   return hwMain(argc, argv); \
}
#else
#define IMPLEMENT_HWAPP_WINMAIN \
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int nCmdShow)
{
    return hwWinMain(hInstance, lpCmdLine, nCmdShow);
}
#endif

#if  defined(USE_DLL)
  #define IMPLEMENT_HWAPP(appname) \
     IMPLEMENT_HWAPP_NOMAIN(appname) \
     IMPLEMENT_HWAPP_WINMAIN
#else
  #define IMPLEMENT_HWAPP(appname) \
     IMPLEMENT_HWAPP_NOMAIN(appname)
#endif

#ifdef HW_MSW
  #include "msw/hwApp.h"
#endif

#endif // HW_APP_H
