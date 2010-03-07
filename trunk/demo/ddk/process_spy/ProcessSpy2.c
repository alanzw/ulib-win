#ifdef __GNUC__
  #include <ddk/ntddk.h>
#else
  #include <ntddk.h>
#endif // __GNUC__

#include <windef.h>

#include "defines.h"

#define SYSNAME "System"
#define VERSIONLEN 100

const WCHAR devLink[]  = L"\\\\.\\MyEvent";
const WCHAR devName[]  = L"\\Device\\MyEvent";
UNICODE_STRING devNameUnicd;
UNICODE_STRING devLinkUnicd;
PVOID gpEventObject = NULL; //
ULONG ProcessNameOffset = 0;
PVOID outBuf[255];
BOOL g_bMainThread;
ULONG g_dwParentId;
CHECKLIST CheckList;
ULONG BuildNumber;
ULONG SYSTEMID;
PWCHAR Version[VERSIONLEN];

NTSTATUS DDKAPI PsLookupProcessByProcessId(IN ULONG ulProcId, OUT PEPROCESS *pEProcess);

ULONG GetProcessNameOffset()
{
    PEPROCESS curproc;
    int i;

    curproc = PsGetCurrentProcess();

    for( i = 0; i < 3*PAGE_SIZE; i++ )
    {
        if( !strncmp( SYSNAME, (PCHAR) curproc + i, strlen(SYSNAME) ))
        {
            return i;
        }
    }

    return 0;
}

NTSTATUS GetRegValue(PCWSTR RegPath,PCWSTR ValueName,PWCHAR Value)
{
    int ReturnValue = 0;
    NTSTATUS Status;
    OBJECT_ATTRIBUTES ObjectAttributes;
    HANDLE KeyHandle;
    PKEY_VALUE_PARTIAL_INFORMATION valueInfoP;
    ULONG valueInfoLength,returnLength;
    UNICODE_STRING UnicodeRegPath;
    UNICODE_STRING UnicodeValueName;

    RtlInitUnicodeString(&UnicodeRegPath, RegPath);
    RtlInitUnicodeString(&UnicodeValueName, ValueName);

    InitializeObjectAttributes(&ObjectAttributes,
    	&UnicodeRegPath,
    	OBJ_CASE_INSENSITIVE, // Flags
    	NULL, // Root directory
    	NULL); // Security descriptor

    Status = ZwOpenKey(&KeyHandle, KEY_ALL_ACCESS, &ObjectAttributes);
    if (Status != STATUS_SUCCESS)
    {
        DbgPrint("ZwOpenKey Wrong\n");
        return 0;
    }

    valueInfoLength = sizeof(KEY_VALUE_PARTIAL_INFORMATION)+VERSIONLEN;
    valueInfoP = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(NonPagedPool, valueInfoLength);
    Status = ZwQueryValueKey(KeyHandle,
    	&UnicodeValueName,
    	KeyValuePartialInformation,
    	valueInfoP,
    	valueInfoLength,
    	&returnLength);

    if (!NT_SUCCESS(Status))
    {
        DbgPrint("ZwQueryValueKey Wrong:%08x\n",Status);
        return Status;
    }
    else
    {
        RtlCopyMemory((PCHAR)Value, (PCHAR)valueInfoP->Data, valueInfoP->DataLength);
        ReturnValue = 1;
    }

    if(!valueInfoP);
    ExFreePool(valueInfoP);
    ZwClose(KeyHandle);
    return ReturnValue;
}

VOID MyRemoveCreateThreadNotifyRoutine(IN PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine)
{
    //PsRemoveCreateThreadNotifyRoutine(ThreadCreateMon);
    PVOID ptr=NULL;
    if(BuildNumber==2195)                                    //Windows 2000 Sp4,2195
    // < SP4 is not tested
    {
        ptr=0x80484520;
    }
    else if(BuildNumber==2600)
    {
        if(wcscmp(Version,L"Service Pack 1")==0)            //Windows Xp Sp1,2600
        ptr=0x8054efc0;
        else if(wcscmp(Version,L"Service Pack 2")==0)        //Windows Xp Sp2,2600
        ptr=0x80561d20;
    }
    else if(BuildNumber==3790)                                //Windows 2003 server,3790
    {
        ptr=0x80570f40;
    }
    if(ptr!=NULL)
    memset(ptr, 0, sizeof(ULONG)*8);
}

VOID ThreadCreateMon (IN HANDLE PId, IN HANDLE TId, IN BOOLEAN  bCreate)
{
    PEPROCESS EProcess,PEProcess;
    NTSTATUS status;
    HANDLE dwParentPID;

    status = PsLookupProcessByProcessId( (ULONG)PId, &EProcess);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsLookupProcessByProcessId()\n");
        return ;
    }

    if ( bCreate )
    {
        dwParentPID=PsGetCurrentProcessId();
        status = PsLookupProcessByProcessId((ULONG)dwParentPID, &PEProcess);
        if (!NT_SUCCESS( status ))
        {
            DbgPrint("PsLookupProcessByProcessId()\n");
            return ;
        }
        if(PId==4)    //System进程创建的东东我们不管
        //在2000下是0,在XP后是4
        return;
        if((g_bMainThread==TRUE) && (g_dwParentId != dwParentPID) &&(dwParentPID != PId))
        {
            g_bMainThread=FALSE;
            sprintf(outBuf, "=============================="
            "Remote Thread :"
            "=============================="
            "\nT:%18s%9d%9d%25s%9d\n"
            "======================================"
            "======================================\n",
            (char *)((char *)EProcess+ProcessNameOffset),
            PId, TId,
            (char *)((char *)PEProcess+ProcessNameOffset),dwParentPID);
            if(gpEventObject!=NULL)
            KeSetEvent((PRKEVENT)gpEventObject, 0, FALSE);
        }
        if(CheckList.ONLYSHOWREMOTETHREAD)    //只显示远线程
        return;
        DbgPrint( "T:%18s%9d%9d%25s%9d\n",
        (char *)((char *)EProcess+ProcessNameOffset),
        PId, TId,
        (char *)((char *)PEProcess+ProcessNameOffset),dwParentPID);
        sprintf(outBuf, "T:%18s%9d%9d%25s%9d\n",
        (char *)((char *)EProcess+ProcessNameOffset),
        PId, TId,
        (char *)((char *)PEProcess+ProcessNameOffset),dwParentPID);
        if(gpEventObject!=NULL)
        KeSetEvent((PRKEVENT)gpEventObject, 0, FALSE);
    }
    else if(CheckList.SHOWTERMINATETHREAD)
    {
        DbgPrint( "TERMINATED == THREAD ID: %d\n", TId);
        sprintf(outBuf,"TERMINATED == THREAD ID: %d\n", TId);
        if(gpEventObject!=NULL)
        KeSetEvent((PRKEVENT)gpEventObject, 0, FALSE);
    }
}


VOID ProcessCreateMon ( HANDLE hParentId, HANDLE PId, BOOLEAN bCreate )
{

    PEPROCESS        EProcess,PProcess;
    NTSTATUS        status;
    HANDLE            TId;

    g_dwParentId = hParentId;
    status = PsLookupProcessByProcessId((ULONG)PId, &EProcess);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsLookupProcessByProcessId()\n");
        return ;
    }
    status = PsLookupProcessByProcessId((ULONG)hParentId, &PProcess);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsLookupProcessByProcessId()\n");
        return ;
    }

    if ( bCreate )
    {
        g_bMainThread = TRUE;
        DbgPrint( "P:%18s%9d%9d%25s%9d\n",
        (char *)((char *)EProcess+ProcessNameOffset),
        PId,PsGetCurrentThreadId(),
        (char *)((char *)PProcess+ProcessNameOffset),
        hParentId
        );
        sprintf(outBuf, "P:%18s%9d%9d%25s%9d\n",
        (char *)((char *)EProcess+ProcessNameOffset),
        PId,PsGetCurrentThreadId(),
        (char *)((char *)PProcess+ProcessNameOffset),
        hParentId
        );
        if(gpEventObject!=NULL)
        KeSetEvent((PRKEVENT)gpEventObject, 0, FALSE);
    }
    else if(CheckList.SHOWTERMINATEPROCESS)
    {
        DbgPrint( "TERMINATED == PROCESS ID: %d\n", PId);
        sprintf(outBuf,"TERMINATED == PROCESS ID: %d\n", PId);
        if(gpEventObject!=NULL)
        KeSetEvent((PRKEVENT)gpEventObject, 0, FALSE);
    }

}

NTSTATUS OnUnload( IN PDRIVER_OBJECT pDriverObject )
{
    NTSTATUS            status;
    DbgPrint("OnUnload called\n");
    if(gpEventObject)
    ObDereferenceObject(gpEventObject);
    PsSetCreateProcessNotifyRoutine(ProcessCreateMon, TRUE);
    MyRemoveCreateThreadNotifyRoutine(ThreadCreateMon);
    if(pDriverObject->DeviceObject != NULL)
    {
        status=IoDeleteSymbolicLink( &devLinkUnicd );
        if ( !NT_SUCCESS( status ) )
        {
            DbgPrint((  "IoDeleteSymbolicLink() failed\n" ));
            return status;
        }
        IoDeleteDevice( pDriverObject->DeviceObject );
    }
    return STATUS_SUCCESS;
}

NTSTATUS DeviceIoControlDispatch(
IN  PDEVICE_OBJECT  DeviceObject,
IN  PIRP            pIrp
)
{
    PIO_STACK_LOCATION              irpStack;
    NTSTATUS                        status;
    PVOID                           inputBuffer;
    ULONG                           inputLength;
    PVOID                           outputBuffer;
    ULONG                           outputLength;
    OBJECT_HANDLE_INFORMATION        objHandleInfo;

    status = STATUS_SUCCESS;
    //
    irpStack = IoGetCurrentIrpStackLocation(pIrp);

    switch (irpStack->MajorFunction)
    {
    case IRP_MJ_CREATE :
        DbgPrint("Call IRP_MJ_CREATE\n");
        break;
    case IRP_MJ_CLOSE:
        DbgPrint("Call IRP_MJ_CLOSE\n");
        break;
    case IRP_MJ_DEVICE_CONTROL:
        DbgPrint("IRP_MJ_DEVICE_CONTROL\n");
        inputLength=irpStack->Parameters.DeviceIoControl.InputBufferLength;
        outputLength=irpStack->Parameters.DeviceIoControl.OutputBufferLength;
        switch (irpStack->Parameters.DeviceIoControl.IoControlCode)
        {
        case IOCTL_PASSEVENT:    //用事件做通信
            inputBuffer = pIrp->AssociatedIrp.SystemBuffer;

            DbgPrint("inputBuffer:%08x\n", (HANDLE)inputBuffer);
            status = ObReferenceObjectByHandle(*(HANDLE *)inputBuffer,
            GENERIC_ALL,
            NULL,
            KernelMode,
            &gpEventObject,
            &objHandleInfo);

            if(status!=STATUS_SUCCESS)
            {
                DbgPrint("wrong\n");
                break;
            }
            break;
        case IOCTL_UNPASSEVENT:
            if(gpEventObject)
            ObDereferenceObject(gpEventObject);
            DbgPrint("UNPASSEVENT called\n");
            break;
        case IOCTL_PASSBUF:
            RtlCopyMemory(pIrp->UserBuffer, outBuf, outputLength);
            break;
        case IOCTL_PASSEVSTRUCT:
            inputBuffer = pIrp->AssociatedIrp.SystemBuffer;
            memset(&CheckList, 0, sizeof(CheckList));
            RtlCopyMemory(&CheckList, inputBuffer, sizeof(CheckList));
            DbgPrint("%d:%d\n", CheckList.ONLYSHOWREMOTETHREAD, CheckList.SHOWTHREAD);
            break;
        default:
            break;
        }
        break;
    default:
        DbgPrint("Call IRP_MJ_UNKNOWN\n");
        break;
    }

    pIrp->IoStatus.Status = status;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest (pIrp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS DDKAPI DriverEntry( IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING theRegistryPath )
{
    NTSTATUS                Status;
    PDEVICE_OBJECT            pDevice;

    DbgPrint("DriverEntry called!\n");
    g_bMainThread = FALSE;

    if(1!=GetRegValue(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion", L"CSDVersion", Version))
    {
        DbgPrint("GetRegValueDword Wrong\n");
    }
    PsGetVersion(NULL, NULL, &BuildNumber, NULL);
    DbgPrint("[[[%d]]]:[[[%ws]]]", BuildNumber, Version);

    RtlInitUnicodeString (&devNameUnicd, devName );
    RtlInitUnicodeString (&devLinkUnicd, devLink );

    Status = IoCreateDevice ( pDriverObject,
    0,
    &devNameUnicd,
    FILE_DEVICE_UNKNOWN,
    0,
    TRUE,
    &pDevice );
    if( !NT_SUCCESS(Status))
    {
        DbgPrint(("Can not create device.\n"));
        return Status;
    }

    Status = IoCreateSymbolicLink (&devLinkUnicd, &devNameUnicd);
    if( !NT_SUCCESS(Status))
    {
        DbgPrint(("Cannot create link.\n"));
        return Status;
    }

    ProcessNameOffset = GetProcessNameOffset();

    pDriverObject->DriverUnload  = OnUnload;
    pDriverObject->MajorFunction[IRP_MJ_CREATE] =
    pDriverObject->MajorFunction[IRP_MJ_CLOSE] =
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceIoControlDispatch;

    Status = PsSetCreateProcessNotifyRoutine(ProcessCreateMon, FALSE);
    if (!NT_SUCCESS( Status ))
    {
        DbgPrint("PsSetCreateProcessNotifyRoutine()\n");
        return Status;
    }

    Status = PsSetCreateThreadNotifyRoutine(ThreadCreateMon);
    if (!NT_SUCCESS( Status ))
    {
        DbgPrint("PsSetCreateThreadNotifyRoutine()\n");
        return Status;
    }

    return STATUS_SUCCESS;
}