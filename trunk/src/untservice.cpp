#include <windows.h>
#include <tchar.h>

#include <stdio.h>

#include "untservice.h"

UNTService::UNTService()
{}

UNTService::~UNTService()
{}

UNTServiceMan::UNTServiceMan()
{}

UNTServiceMan::~UNTServiceMan()
{}

BOOL UNTServiceMan::installSvc(LPCTSTR lpSvcName)
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return FALSE;
    }

    // Get a handle to the SCM database.
    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database
        SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return FALSE;
    }

    // Create the service.
    schService = ::CreateService(
        schSCManager,              // SCM database
        lpSvcName,                 // name of service
        lpSvcName,                 // service name to display
        SERVICE_ALL_ACCESS,        // desired access
        SERVICE_WIN32_OWN_PROCESS, // service type
        SERVICE_DEMAND_START,      // start type
        SERVICE_ERROR_NORMAL,      // error control type
        szPath,                    // path to service's binary
        NULL,                      // no load ordering group
        NULL,                      // no tag identifier
        NULL,                      // no dependencies
        NULL,                      // LocalSystem account
        NULL);                     // no password

    if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    else
    {
        printf("Service installed successfully\n");
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);


    return TRUE;
}

BOOL UNTServiceMan::deleteSvc(LPCTSTR lpSvcName)
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    SERVICE_STATUS ssStatus; 

    // Get a handle to the SCM database. 
 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return FALSE;
    }

    // Get a handle to the service.

    schService = OpenService( 
        schSCManager,       // SCM database 
        lpSvcName,          // name of service 
        DELETE);            // need delete access 
 
    if (schService == NULL)
    { 
        printf("OpenService failed (%d)\n", GetLastError()); 
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // Delete the service.
 
    if (! DeleteService(schService) ) 
    {
        printf("DeleteService failed (%d)\n", GetLastError()); 
    }
    else printf("Service deleted successfully\n"); 
 
    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);
    
    return TRUE;
}

BOOL UNTServiceMan::startSvc(LPCTSTR lpSvcName)
{
    return TRUE;
}

BOOL UNTServiceMan::stopSvc(LPCTSTR lpSvcName)
{
    return TRUE;
}

int UNTServiceMan::queryState(LPCTSTR lpSvcName)
{
    return 0;
}

