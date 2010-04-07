#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#define SZAPPNAME "basicservice"
#define SZSERVICENAME "KService"
#define SZSERVICEDISPLAYNAME "KService"
#define SZDEPENDENCIES ""

void WINAPI KServiceMain(DWORD argc, LPTSTR * argv);
void InstallService(const char * szServiceName);
void LogEvent(LPCTSTR pFormat, ...);
void Start();
void Stop();

SERVICE_STATUS ssStatus;
SERVICE_STATUS_HANDLE sshStatusHandle;

int main(int argc, char * argv[])
{
   if ((argc==2) && (::strcmp(argv[1]+1, "install")==0))
   {
     InstallService("KService");
     return 0;
   }

   SERVICE_TABLE_ENTRY service_table_entry[] =
   {
     { "KService", KServiceMain },
     { NULL, NULL }
   };
   ::StartServiceCtrlDispatcher(service_table_entry);
   return 0;
}

void InstallService(const char * szServiceName)
{
   SC_HANDLE handle = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
   char szFilename[256];
   ::GetModuleFileName(NULL, szFilename, 255);
   SC_HANDLE hService = ::CreateService(handle, szServiceName,
   szServiceName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
   SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, szFilename, NULL,
   NULL, NULL, NULL, NULL);
   ::CloseServiceHandle(hService);
   ::CloseServiceHandle(handle);
}

SERVICE_STATUS servicestatus;
SERVICE_STATUS_HANDLE servicestatushandle;

void WINAPI ServiceCtrlHandler(DWORD dwControl)
{
   switch (dwControl)
   {
    case SERVICE_CONTROL_PAUSE:
     servicestatus.dwCurrentState = SERVICE_PAUSE_PENDING;
     // TODO: add code to set dwCheckPoint & dwWaitHint
     // This value need to try a lot to confirm 
     // ...
     ::SetServiceStatus(servicestatushandle, &servicestatus);
     // TODO: add code to pause the service
     // not called in this service
     // ...
     servicestatus.dwCurrentState = SERVICE_PAUSED;
     // TODO: add code to set dwCheckPoint & dwWaitHint to 0
     break;

   case SERVICE_CONTROL_CONTINUE:
     servicestatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
     // TODO: add code to set dwCheckPoint & dwWaitHint
     ::SetServiceStatus(servicestatushandle, &servicestatus);
     // TODO: add code to unpause the service
     // not called in this service
     // ...
     servicestatus.dwCurrentState = SERVICE_RUNNING;
     // TODO: add code to set dwCheckPoint & dwWaitHint to 0
    break;

   case SERVICE_CONTROL_STOP:
     servicestatus.dwCurrentState = SERVICE_STOP_PENDING;
     // TODO: add code to set dwCheckPoint & dwWaitHint
    ::SetServiceStatus(servicestatushandle, &servicestatus);
     // TODO: add code to stop the service
     Stop();
     servicestatus.dwCurrentState = SERVICE_STOPPED;
     // TODO: add code to set dwCheckPoint & dwWaitHint to 0
    break;

   case SERVICE_CONTROL_SHUTDOWN:
     // TODO: add code for system shutdown
     // as quick as possible
     break;

   case SERVICE_CONTROL_INTERROGATE:
     // TODO: add code to set the service status
     // ...
     servicestatus.dwCurrentState = SERVICE_RUNNING;
     break;
   }
   ::SetServiceStatus(servicestatushandle, &servicestatus);
}

void WINAPI KServiceMain(DWORD argc, LPTSTR * argv)
{
   servicestatus.dwServiceType = SERVICE_WIN32;
   servicestatus.dwCurrentState = SERVICE_START_PENDING;
   servicestatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;//
   servicestatus.dwWin32ExitCode = 0;
   servicestatus.dwServiceSpecificExitCode = 0;
   servicestatus.dwCheckPoint = 0;
   servicestatus.dwWaitHint = 0;

   servicestatushandle =
   ::RegisterServiceCtrlHandler("KService", ServiceCtrlHandler);
   if (servicestatushandle == (SERVICE_STATUS_HANDLE)0)
   {
     return;
   }

   bool bInitialized = false;
   // Initialize the service
   // ...
   Start();

   bInitialized = true;

   servicestatus.dwCheckPoint = 0;
   servicestatus.dwWaitHint = 0;
   if (!bInitialized)
   {
     servicestatus.dwCurrentState = SERVICE_STOPPED;
     servicestatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
     servicestatus.dwServiceSpecificExitCode = 1;
   }
   else
   {
     servicestatus.dwCurrentState = SERVICE_RUNNING;
   }
   ::SetServiceStatus(servicestatushandle, &servicestatus);
   return;
}


void Start()
{
   LogEvent("Service Starting...");
}

void LogEvent(LPCTSTR pFormat, ...)
{
   TCHAR chMsg[256];
   HANDLE hEventSource;
   LPTSTR lpszStrings[1];
   va_list pArg;

   va_start(pArg, pFormat);
   _vstprintf(chMsg, pFormat, pArg);
   va_end(pArg);

   lpszStrings[0] = chMsg;

   if (1)
   {
     // Get a handle to use with ReportEvent().
     hEventSource = RegisterEventSource(NULL, "KService");
     if (hEventSource != NULL)
     {
     // Write to event log. 
       ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
       DeregisterEventSource(hEventSource);
     }
   }
   else
   {
     // As we are not running as a service, just write the error to the console.
     _putts(chMsg);
   }
}

void Stop()
{
   LogEvent("Service Stoped.");
}
