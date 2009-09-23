//---------------------------------------------------------------------------
//
// ProcObsrv.c
//
// SUBSYSTEM:
//                System monitor
// MODULE:
//                Driver for monitoring NT process and DLLs mapping
//              monitoring.
//
// DESCRIPTION:
//              This code is based on the James Finnegan抯 sample
//              (MSJ January 1999).
//
// Ivo Ivanov, January 2002
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Includes
//
//---------------------------------------------------------------------------
#ifdef __GNUC__
  #include <ddk/ntddk.h>
#else
  #include <ntddk.h>
#endif

//---------------------------------------------------------------------------
//
// Defines
//
//---------------------------------------------------------------------------

#define FILE_DEVICE_UNKNOWN             0x00000022
#define IOCTL_UNKNOWN_BASE              FILE_DEVICE_UNKNOWN
#define IOCTL_PROCOBSRV_ACTIVATE_MONITORING    \
    CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_PROCOBSRV_GET_PROCINFO    \
    CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
//---------------------------------------------------------------------------
//
// Forward declaration
//
//---------------------------------------------------------------------------
void DDKAPI UnloadDriver(
    PDRIVER_OBJECT DriverObject
    );
NTSTATUS DDKAPI DispatchCreateClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );
NTSTATUS DDKAPI DispatchIoctl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );
//
// Process function callback
//
VOID DDKAPI ProcessCallback(
    IN HANDLE  hParentId,
    IN HANDLE  hProcessId,
    IN BOOLEAN bCreate
    );
//
// Structure for holding info about activating/deactivating the driver
//
typedef struct _ActivateInfo
{
    BOOLEAN  bActivated;
} ACTIVATE_INFO, *PACTIVATE_INFO;

//
// Structure for process callback information
//
typedef struct _ProcessCallbackInfo
{
    HANDLE  hParentId;
    HANDLE  hProcessId;
    BOOLEAN bCreate;
} PROCESS_CALLBACK_INFO, *PPROCESS_CALLBACK_INFO;

//
// Private storage for process retreiving
//
typedef struct _DEVICE_EXTENSION
{
    PDEVICE_OBJECT DeviceObject;
    //
    // Shared section
    //
    HANDLE  hProcessId;
    //
    // Process section data
    //
    PKEVENT ProcessEvent;
    HANDLE  hParentId;
    BOOLEAN bCreate;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//
// Global variables
//
PDEVICE_OBJECT g_pDeviceObject;
ACTIVATE_INFO  g_ActivateInfo;

//
// The main entry point of the driver module
//
NTSTATUS DDKAPI DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{
    NTSTATUS                  ntStatus;
    UNICODE_STRING            uszDriverString;
    UNICODE_STRING            uszDeviceString;
    UNICODE_STRING            uszProcessEventString;
    PDEVICE_OBJECT            pDeviceObject;
    PDEVICE_EXTENSION         extension;
    HANDLE                    hProcessHandle;

    //
    // Point uszDriverString at the driver name
    //
    RtlInitUnicodeString(&uszDriverString, L"\\Device\\ProcObsrv");
    //
    // Create and initialize device object
    //
    ntStatus = IoCreateDevice(
        DriverObject,
        sizeof(DEVICE_EXTENSION),
        &uszDriverString,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &pDeviceObject
        );
    if(ntStatus != STATUS_SUCCESS)
        return ntStatus;
    //
    // Assign extension variable
    //
    extension = pDeviceObject->DeviceExtension;
    //
    // Point uszDeviceString at the device name
    //
    RtlInitUnicodeString(&uszDeviceString, L"\\DosDevices\\ProcObsrv");
    //
    // Create symbolic link to the user-visible name
    //
    ntStatus = IoCreateSymbolicLink(&uszDeviceString, &uszDriverString);

    if(ntStatus != STATUS_SUCCESS)
    {
        //
        // Delete device object if not successful
        //
        IoDeleteDevice(pDeviceObject);
        return ntStatus;
    }
    //
    // Assign global pointer to the device object for use by the callback functions
    //
    g_pDeviceObject = pDeviceObject;
    //
    // Setup initial state
    //
    g_ActivateInfo.bActivated = FALSE;
    //
    // Load structure to point to IRP handlers
    //
    DriverObject->DriverUnload                         = UnloadDriver;
    DriverObject->MajorFunction[IRP_MJ_CREATE]         = DispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]          = DispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    //
    // Create event for user-mode processes to monitor
    //
    RtlInitUnicodeString(
        &uszProcessEventString,
        L"\\BaseNamedObjects\\ProcObsrvProcessEvent"
        );
    extension->ProcessEvent = IoCreateNotificationEvent(
        &uszProcessEventString,
        &hProcessHandle
        );
    //
    // Clear it out
    //
    KeClearEvent(extension->ProcessEvent);
    //
    // Return success
    //
    return ntStatus;
}

//
// Create and close routine
//
NTSTATUS DDKAPI DispatchCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

//
// Process function callback
//
VOID DDKAPI ProcessCallback(
    IN HANDLE  hParentId,
    IN HANDLE  hProcessId,
    IN BOOLEAN bCreate
    )
{
    PDEVICE_EXTENSION extension;
    //
    // Assign extension variable
    //
    extension = g_pDeviceObject->DeviceExtension;
    //
    // Assign current values into device extension.
    // User-mode apps will pick it up using DeviceIoControl calls.
    //
    extension->hParentId  = hParentId;
    extension->hProcessId = hProcessId;
    extension->bCreate    = bCreate;
    //
    // Signal the event thus the user-mode apps listening will be aware
    // that something interesting has happened.
    //
    KeSetEvent(extension->ProcessEvent, 0, FALSE);
    KeClearEvent(extension->ProcessEvent);
}

//
// IOCTL handler for setting the callback
//
NTSTATUS DDKAPI ActivateMonitoringHanlder(
    IN PIRP           Irp
    )
{
    NTSTATUS               ntStatus = STATUS_UNSUCCESSFUL;
    PIO_STACK_LOCATION     irpStack  = IoGetCurrentIrpStackLocation(Irp);
    PACTIVATE_INFO         pActivateInfo;

    if (irpStack->Parameters.DeviceIoControl.InputBufferLength >=
       sizeof(ACTIVATE_INFO))
    {
        pActivateInfo = Irp->AssociatedIrp.SystemBuffer;
        if (g_ActivateInfo.bActivated != pActivateInfo->bActivated)
        {
            if (pActivateInfo->bActivated)
            {
                //
                // Set up callback routines
                //
                ntStatus = PsSetCreateProcessNotifyRoutine(ProcessCallback, FALSE);
                if (ntStatus != STATUS_SUCCESS)
                {
                    return ntStatus;
                }
                //
                // Setup the global data structure
                //
                g_ActivateInfo.bActivated = pActivateInfo->bActivated;
            } // if
            else
            {
                //
                // restore the call back routine, thus givinig chance to the
                // user mode application to unload dynamically the driver
                //
                ntStatus = PsSetCreateProcessNotifyRoutine(ProcessCallback, TRUE);
                if (ntStatus != STATUS_SUCCESS)
                    return ntStatus;
                else
                    g_ActivateInfo.bActivated = FALSE;
            }
            ntStatus = STATUS_SUCCESS;
        } // if
    } // if

    return ntStatus;
}

//
// The dispatch routine
//
NTSTATUS DDKAPI DispatchIoctl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
{
    NTSTATUS               ntStatus = STATUS_UNSUCCESSFUL;
    PIO_STACK_LOCATION     irpStack  = IoGetCurrentIrpStackLocation(Irp);
    PDEVICE_EXTENSION      extension = DeviceObject->DeviceExtension;
    PPROCESS_CALLBACK_INFO pProcCallbackInfo;
    //
    // These IOCTL handlers are the set and get interfaces between
    // the driver and the user mode app
    //
    switch(irpStack->Parameters.DeviceIoControl.IoControlCode)
    {
        case IOCTL_PROCOBSRV_ACTIVATE_MONITORING:
            {
                ntStatus = ActivateMonitoringHanlder( Irp );
                break;
            }
        case IOCTL_PROCOBSRV_GET_PROCINFO:
            {
                if (irpStack->Parameters.DeviceIoControl.OutputBufferLength >=
                   sizeof(PROCESS_CALLBACK_INFO))
                {
                    pProcCallbackInfo = Irp->AssociatedIrp.SystemBuffer;
                    pProcCallbackInfo->hParentId  = extension->hParentId;
                    pProcCallbackInfo->hProcessId = extension->hProcessId;
                    pProcCallbackInfo->bCreate    = extension->bCreate;

                    ntStatus = STATUS_SUCCESS;
                }
                break;
            }

        default:
            break;
    }

    Irp->IoStatus.Status = ntStatus;
    //
    // Set number of bytes to copy back to user-mode
    //
    if(ntStatus == STATUS_SUCCESS)
        Irp->IoStatus.Information =
            irpStack->Parameters.DeviceIoControl.OutputBufferLength;
    else
        Irp->IoStatus.Information = 0;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return ntStatus;
}

//
// Driver unload routine
//
void DDKAPI UnloadDriver(
    IN PDRIVER_OBJECT DriverObject
    )
{
    UNICODE_STRING  uszDeviceString;
    //
    //  By default the I/O device is configured incorrectly or the
    // configuration parameters to the driver are incorrect.
    //
    NTSTATUS        ntStatus = STATUS_DEVICE_CONFIGURATION_ERROR;

    if (g_ActivateInfo.bActivated)
        //
        // restore the call back routine, thus givinig chance to the
        // user mode application to unload dynamically the driver
        //
        ntStatus = PsSetCreateProcessNotifyRoutine(ProcessCallback, TRUE);

    IoDeleteDevice(DriverObject->DeviceObject);

    RtlInitUnicodeString(&uszDeviceString, L"\\DosDevices\\ProcObsrv");
    IoDeleteSymbolicLink(&uszDeviceString);
}

//----------------------------End of the file -------------------------------

