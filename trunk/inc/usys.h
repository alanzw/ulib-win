#ifndef U_SYS_H
#define U_SYS_H

#include "ulib.h"

#ifndef U_BUFSIZE
#define U_BUFSIZE 512
#endif

class ULIB_API USystem
{
public:
    USystem();
    ~USystem();
    //
    int getScreenWidth();
    int getScreenHeight();
    //
    int getArrange();
    LPCTSTR getArrangeText();
    //
    int getBootMode();
    LPCTSTR getBootModeText();
    //
    int getMonitorNum();

    //
    int getMouseButtonNum();

    /// KeyBoard


    //
    LPCTSTR getComputerName();

    //
    int getWindowCaptionHeight();
protected:
private:
    TCHAR *m_pBuffer;
};

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

//
#define INFO_BUFFER_SIZE     32767
#define VOLUME_SERIAL_LENGTH 10
#define XP_KEY_LENGTH        30
#define OS_NAME_LENGTH       20

// Message
ULIB_API void __stdcall showMsg(const char *szMsg, const char *szTitle="Info");
//
ULIB_API void __stdcall showError(const char *msg);

ULIB_API void __stdcall showErrorInfo(DWORD eNum);

// Volume Information

//
ULIB_API void __stdcall getVolSerialNum(char *sn, const char *vol = "C:\\");
//
ULIB_API int __stdcall getVolumeList(char *vlmList);
// XP cdkey
ULIB_API void __stdcall getXPKey(char *cdkey);

// User Info
ULIB_API void __stdcall getComputerName(char *hostname);

//
ULIB_API void __stdcall getOSVersion(char *osver);
ULIB_API void __stdcall getOSName(char *osname);

//
ULIB_API void __stdcall getMacAddress(char *macAddr);
ULIB_API int __stdcall getAdapterNum();

//
ULIB_API int __stdcall getUserName(char *uname);

// Monitor
ULIB_API int __stdcall turnOffMonitor(HWND hwnd);
ULIB_API int __stdcall turnOnMonitor(HWND hwnd);
ULIB_API int __stdcall showScreenSave(HWND hwnd);

ULIB_API int __stdcall showStartMenu(HWND hwnd);

ULIB_API int __stdcall shutdownWindows();

//
ULIB_API int __stdcall addIcon2Desktop();
ULIB_API int __stdcall addIcon2MyComputer();

//
ULIB_API bool __stdcall setDesktopWallpaper(LPTSTR sFilePath);
ULIB_API bool __stdcall removeDesktopWallpaper();
ULIB_API bool __stdcall resetDesktopWallpaper();

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // U_SYS_H

