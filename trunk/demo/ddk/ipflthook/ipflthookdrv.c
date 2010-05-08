/*
    sc queryex IpFilterDriver

    enumservice.exe | findstr /I ipfilter
    
    net start IpFilterDriver
    
    enumservice.exe | findstr /I ipfilter
    
    net stop IpFilterDriver

 */
 
/*
 * For x86/EWindows XP SP1 & VC 7 & Windows DDK 2600.1106
 * build -cZ -x86
 */

/************************************************************************
 *                                                                      *
 *                               Head File                              *
 *                                                                      *
 ************************************************************************/

#include <ntddk.h>
/*
 * typedef ULONG   IPAddr, IPMask;
 */
#include <ntddndis.h>
#include <pfhook.h>
/*
 * �õ���CTL_CODE��
 */
#include <devioctl.h>

/************************************************************************
 *                                                                      *
 *                               Macro                                  *
 *                                                                      *
 ************************************************************************/

/*
 * ����Ҫ׷��DeviceNumber
 */
#define INTERNALNAME                            L"\\Device\\IpFilterHookInternal"
/*
 * DosDevices
 */
#define EXTERNALNAME                            L"\\??\\IpFilterHookExternal"
/*
 * ��ExAllocatePoolWithTag�����βε�DDK�ĵ�
 */
#define PRIVATETAG                              'OFSN'
/*
 * 0x0800����С����ֵ
 */
#define IPFILTERHOOK_INDEX                      0x0800
#define IOCTL_IPFILTERHOOK_GET_FIREWALLFLAGS    CTL_CODE    \
(                                                           \
    FILE_DEVICE_NETWORK,                                    \
    IPFILTERHOOK_INDEX + 0,                                 \
    METHOD_BUFFERED,                                        \
    FILE_READ_ACCESS                                        \
)
#define IOCTL_IPFILTERHOOK_SET_FIREWALLFLAGS    CTL_CODE    \
(                                                           \
    FILE_DEVICE_NETWORK,                                    \
    IPFILTERHOOK_INDEX + 1,                                 \
    METHOD_BUFFERED,                                        \
    FILE_WRITE_ACCESS                                       \
)
/*
 * ��λ - Enable
 * ��λ - Disable
 */
#define FIREWALLFLAGS_DISABLEALL                0x00000000
/*
 * ����ICMP���ģ��������ա�����
 */
#define FIREWALLFLAGS_ALLICMP                   0x00000001
/*
 * ���յ���RST����
 */
#define FIREWALLFLAGS_INBOUNDRST                0x00000002
#define FIREWALLFLAGS_ENABLEALL                 0xFFFFFFFF
/*
 * ȱʡ������ȫ���������������κα���
 */
#define FIREWALLFLAGS_DEFAULT                   FIREWALLFLAGS_ENABLEALL

/*
 * �豸��չ���Զ���ṹ
 */
typedef struct _DEVICE_EXTENSION
{
    PDEVICE_OBJECT  DeviceObject;
    ULONG           DeviceNumber;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define OFFSETOF(TYPE, MEMBER)                  ((size_t)&((TYPE)0)->MEMBER)

/*
 * Definitions for IP type of service (ip_tos)
 *
 * �ظ�������Ϊ�˷�����ֲ
 */
#define IPTOS_LOWDELAY                          0x10
#define IPTOS_THROUGHPUT                        0x08
#define IPTOS_RELIABILITY                       0x04
#define IPTOS_MINCOST                           0x02

#define IPPROTO_ICMP                            1
#define IPPROTO_TCP                             6
#define IPPROTO_UDP                             17

/*
 * �Զ���������Щ�ṹ��Ϊ�˷�����ֲ
 */

#pragma pack( push, 1 )

/*
 * Structure of an internet header, naked of options.
 */
struct ipheader
{
#ifdef LITTLE_ENDIAN
    /*
     * 0x45��LITTLE_ENDIAN���λ�ں�ip_hl��4�ֽ�Ϊ��λ��
     */
    unsigned char       ip_hl:4;    /* header length         */
    unsigned char       ip_v:4;     /* version               */
#else
    /*
     * 0x45��BIG_ENDIAN���λ��ǰ
     */
    unsigned char       ip_v:4;     /* version               */
    unsigned char       ip_hl:4;    /* header length         */
#endif
    unsigned char       ip_tos;     /* type of service       */
    /*
     * ip_len���ֽ�Ϊ��λ
     */
    unsigned short int  ip_len;     /* total length          */
    unsigned short int  ip_id;      /* identification        */
    /*
     * ip_off��8�ֽ�Ϊ��λ
     */
    unsigned short int  ip_off;     /* fragment offset field */
    /*
     * ip_offֻռ�˺�13-bits��ǰ3-bits�ĺ���λ�ֱ���DF��MF
     */
#define IP_DF 0x4000                /* dont fragment flag    */
#define IP_MF 0x2000                /* more fragments flag   */
    unsigned char       ip_ttl;     /* time to live          */
    unsigned char       ip_p;       /* protocol              */
    unsigned short int  ip_sum;     /* checksum              */
#if 0
    /*
     * �����ò�����������Ա��Ϊ���ٱ����鷳��ע�͵�����
     */
    struct in_addr      ip_src;     /* source address        */
    struct in_addr      ip_dst;     /* dest address          */
#endif
};

/*
 * TCP header. (with some options)
 * Per RFC 793, September, 1981.
 */
struct tcpheader
{
    unsigned short int  th_sport;   /* source port            */
    unsigned short int  th_dport;   /* destination port       */
    unsigned int        th_seq;     /* sequence number        */
    unsigned int        th_ack;     /* acknowledgement number */
#ifdef LITTLEENDIAN
    /*
     * 0x50��LITTLEENDIAN���λ�ں�
     */
    unsigned char       th_x2:4,    /* (unused)               */
                        th_off:4;   /* data offset            */
#else
    /*
     * 0x50��BIGENDIAN���λ��ǰ��th_off��4�ֽ�Ϊ��λ��
     */
    unsigned char       th_off:4,   /* data offset            */
                        th_x2:4;    /* (unused)               */
#endif
    unsigned char       th_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PSH  0x08
#define TH_ACK  0x10
#define TH_URG  0x20
    unsigned short int  th_win;     /* window                 */
    unsigned short int  th_sum;     /* checksum               */
    unsigned short int  th_urp;     /* urgent pointer         */
    /*
     * �ο�W.Richard Stevens��Tcp/Ip Illustrated Volume I 18.10С��
     *
     * kind=2 len=4 mss(������IP�ײ���TCP�ײ�)
     *
     */
    unsigned char       th_optmsskind;
    unsigned char       th_optmsslen;
    unsigned short int  th_optmss;
    /*
     * kind=1
     */
    unsigned char       th_optnopa;
    unsigned char       th_optnopb;
    /*
     * kind=4 len=2
     */
    unsigned char       th_optsackpermittedkind;
    unsigned char       th_optsackpermittedlen;
};

#pragma pack( pop )

/************************************************************************
 *                                                                      *
 *                            Function Prototype                        *
 *                                                                      *
 ************************************************************************/

/*
 * ��Dispatch����
 */
static NTSTATUS             IfhCreateDevice
(
    IN  PDRIVER_OBJECT          DriverObject,
    IN  ULONG                   DeviceNumber
);
/*
 * ��Dispatch����
 */
static VOID                 IfhDeleteDevice
(
    IN  PDEVICE_OBJECT          DeviceObject
);
/*
 * ��DDK�ĵ�������DispatchClose
 */
static NTSTATUS             IfhDispatchClose
(
    IN  PDEVICE_OBJECT          DeviceObject,
    IN  PIRP                    Irp
);
/*
 * ��DDK�ĵ�������DispatchCreate
 */
static NTSTATUS             IfhDispatchCreate
(
    IN  PDEVICE_OBJECT          DeviceObject,
    IN  PIRP                    Irp
);
/*
 * ��DDK�ĵ�������DispatchDeviceControl
 */
static NTSTATUS             IfhDispatchDeviceControl
(
    IN  PDEVICE_OBJECT          DeviceObject,
    IN  PIRP                    Irp
);
static NTSTATUS             IfhDoHook
(
    PacketFilterExtensionPtr    fwfunc
);
/*
 * ��Dispatch��������DDK�ĵ�������Unload
 */
static VOID                 IfhDriverUnload
(
    IN  PDRIVER_OBJECT  DriverObject
);
/*
 * �ص���������DDK�ĵ�������PacketFilterExtensionPtr
 */
static PF_FORWARD_ACTION    IfhFirewall
(
    IN  unsigned char          *PacketHeader,
    IN  unsigned char          *Packet,
    IN  unsigned int            PacketLength,
    IN  unsigned int            RecvInterfaceIndex,
    IN  unsigned int            SendInterfaceIndex,
    IN  IPAddr                  RecvLinkNextHop,
    IN  IPAddr                  SendLinkNextHop
);
/*
 * Ωһ����������
 */
       NTSTATUS             DriverEntry
(
    IN  PDRIVER_OBJECT          DriverObject,
    IN  PUNICODE_STRING         RegistryPath
);

/*
 * �ο�<<The Windows 2000 Device Driver Book, Second Edition>>��5.2.6��
 * 5.2.7С�ڡ�INIT��PAGEӦ���Ǵ�Сд���еģ����Ҿ�Ȼ������init��page������
 * ����ô���£��������������INIT��PAGE���ˡ�
 *
 * IfhFirewall������ʱ����ʲôIRQL�ϣ���Ҳ���������ȫ��������������ˡ�
 */
#ifdef ALLOC_PRAGMA

#pragma alloc_text( INIT, IfhCreateDevice           )
#pragma alloc_text( PAGE, IfhDeleteDevice           )
#pragma alloc_text( PAGE, IfhDispatchClose          )
#pragma alloc_text( PAGE, IfhDispatchCreate         )
#pragma alloc_text( PAGE, IfhDispatchDeviceControl  )
#pragma alloc_text( PAGE, IfhDoHook                 )
#pragma alloc_text( PAGE, IfhDriverUnload           )
#pragma alloc_text( INIT, DriverEntry               )

#endif

/************************************************************************
 *                                                                      *
 *                            Static Global Var                         *
 *                                                                      *
 ************************************************************************/

/*
 * ���ڻص�����IfhFirewallҪʹ�øñ�־�������Ϊ��̬ȫ�ֱ�����
 */
static ULONG    FirewallFlags   = FIREWALLFLAGS_DEFAULT;

/************************************************************************/

/*
 * ��Dispatch�����������������������������ܶ࣬ʵ���Ͼ��ǵ���������
 * ����IoCreateDevice��IoCreateSymbolicLink���ֱ����ڲ��豸�����ⲿ�豸
 * ����
 */
static NTSTATUS IfhCreateDevice
(
    IN  PDRIVER_OBJECT  DriverObject,
    IN  ULONG           DeviceNumber
)
{
    NTSTATUS            status;
    PDEVICE_OBJECT      DeviceObject;
    PDEVICE_EXTENSION   DeviceExtension;
    UNICODE_STRING      DeviceName;
    UNICODE_STRING      SymbolicLinkName;
    UNICODE_STRING      NumberUnicodeString;
    UNICODE_STRING      InternalNameUnicodeString;
    UNICODE_STRING      ExternalNameUnicodeString;

    KdPrint((  "Entering IfhCreateDevice()\n" ));
    /*
     * If the string is NULL-terminated, Length does not include the
     * trailing NULL.
     */
    NumberUnicodeString.Length              = 0;
    /*
     * 0xFFFFFFFFת����10������4294967295���10���ַ������Ͻ�β��NUL����
     * ����11��
     */
    NumberUnicodeString.MaximumLength       = 16;
    /*
     * PVOID ExAllocatePoolWithTag
     * (
     *     IN  POOL_TYPE   PoolType,
     *     IN  SIZE_T      NumberOfBytes,
     *     IN  ULONG       Tag
     * );
     *
     * ����������ڴ棬�ǵ��ں����ͷ�����
     */
    NumberUnicodeString.Buffer              = ( PWSTR )ExAllocatePoolWithTag
    (
        PagedPool,
        NumberUnicodeString.MaximumLength,
        PRIVATETAG
    );
    if ( NULL == NumberUnicodeString.Buffer )
    {
        /*
         * DDK�ĵ���ָ�������ExAllocatePool����NULL��������Ӧ�÷���
         * STATUS_INSUFFICIENT_RESOURCES������DDK�ĵ���û��ָ��
         * ExAllocatePoolWithTag����NULLʱ����Σ���ֻ�����һ�£�Ҳ����
         * STATUS_INSUFFICIENT_RESOURCES����֪�����Ƿ���Է���
         * STATUS_NO_MEMORY��
         */
        return( STATUS_INSUFFICIENT_RESOURCES );
    }
    /*
     * NTSTATUS RtlIntegerToUnicodeString
     * (
     *     IN      ULONG           Value,
     *     IN      ULONG           Base    OPTIONAL,
     *     IN OUT  PUNICODE_STRING String
     * );
     *
     * converts an unsigned integer value to a NULL-terminated string of one
     * or more Unicode characters in the specified base.
     */
    status                                  = RtlIntegerToUnicodeString
    (
        DeviceNumber,
        10,
        &NumberUnicodeString
    );
    if ( !NT_SUCCESS( status ) )
    {
        ExFreePoolWithTag
        (
            NumberUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &NumberUnicodeString,
            sizeof( NumberUnicodeString )
        );
        return( status );
    }
    /*
     * VOID RtlInitUnicodeString
     * (
     *     IN OUT  PUNICODE_STRING DestinationString,
     *     IN      PCWSTR          SourceString
     * );
     *
     * �������û�ж�̬�����ڴ�
     */
    RtlInitUnicodeString( &DeviceName, INTERNALNAME );
    /*
     * �ں���׷��DeviceNumber��û��wsprintf()���ã������ѣ�ֻ��������̬��
     * ����
     */
    InternalNameUnicodeString.Length        = DeviceName.Length + NumberUnicodeString.Length;
    InternalNameUnicodeString.MaximumLength = InternalNameUnicodeString.Length + 2;
    InternalNameUnicodeString.Buffer        = ( PWSTR )ExAllocatePoolWithTag
    (
        PagedPool,
        InternalNameUnicodeString.MaximumLength,
        PRIVATETAG
    );
    if ( NULL == InternalNameUnicodeString.Buffer )
    {
        /*
         * NTKERNELAPI VOID ExFreePoolWithTag
         * (
         *     IN  PVOID   P,
         *     IN  ULONG   Tag
         * );
         *
         * ��Ҫ�ͷ�NumberUnicodeString.Buffer
         */
        ExFreePoolWithTag
        (
            NumberUnicodeString.Buffer,
            PRIVATETAG
        );
        /*
         * VOID RtlZeroMemory
         * (
         *     IN  VOID UNALIGNED *Destination,
         *     IN  SIZE_T          Length
         * );
         */
        RtlZeroMemory
        (
            &NumberUnicodeString,
            sizeof( NumberUnicodeString )
        );
        return( STATUS_INSUFFICIENT_RESOURCES );
    }
    /*
     * VOID RtlCopyUnicodeString
     * (
     *     IN OUT  PUNICODE_STRING DestinationString,
     *     IN      PUNICODE_STRING SourceString
     * );
     */
    RtlCopyUnicodeString
    (
        &InternalNameUnicodeString,
        &DeviceName
    );
    /*
     * NTSTATUS RtlAppendUnicodeStringToString
     * (
     *     IN OUT  PUNICODE_STRING Destination,
     *     IN      PUNICODE_STRING Source
     * );
     */
    status                                  = RtlAppendUnicodeStringToString
    (
        &InternalNameUnicodeString,
        &NumberUnicodeString
    );
    /*
     * �Ѿ�����ҪNumberUnicodeString.Buffer�������ͷ�����
     */
    ExFreePoolWithTag
    (
        NumberUnicodeString.Buffer,
        PRIVATETAG
    );
    RtlZeroMemory
    (
        &NumberUnicodeString,
        sizeof( NumberUnicodeString )
    );
    if ( !NT_SUCCESS( status ) )
    {
        ExFreePoolWithTag
        (
            InternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &InternalNameUnicodeString,
            sizeof( InternalNameUnicodeString )
        );
        return( status );
    }
    InternalNameUnicodeString.Buffer[ InternalNameUnicodeString.Length / 2 ]
                                            = UNICODE_NULL;
    /*
     * NTSTATUS IoCreateDevice
     * (
     *     IN  PDRIVER_OBJECT      DriverObject,
     *     IN  ULONG               DeviceExtensionSize,
     *     IN  PUNICODE_STRING     DeviceName  OPTIONAL,
     *     IN  DEVICE_TYPE         DeviceType,
     *     IN  ULONG               DeviceCharacteristics,
     *     IN  BOOLEAN             Exclusive,
     *     OUT PDEVICE_OBJECT     *DeviceObject
     * );
     */
    status                                  = IoCreateDevice
    (
        DriverObject,
        sizeof( DEVICE_EXTENSION ),
        &InternalNameUnicodeString,
        FILE_DEVICE_NETWORK,
        0,
        FALSE,
        &DeviceObject
    );
    if ( !NT_SUCCESS( status ) )
    {
        ExFreePoolWithTag
        (
            InternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &InternalNameUnicodeString,
            sizeof( InternalNameUnicodeString )
        );
        return( status );
    }
    DeviceObject->Flags                    |= DO_BUFFERED_IO;
    /*
     * Initialize the Device Extension
     *
     * �豸��չ���ڴ�ռ�����IoCreateDevice�����
     */
    DeviceExtension                         = ( PDEVICE_EXTENSION )DeviceObject->DeviceExtension;
    DeviceExtension->DeviceObject           = DeviceObject;
    DeviceExtension->DeviceNumber           = DeviceNumber;
    /*
     * ���濪ʼ����SymbolicLink
     */
    NumberUnicodeString.Length              = 0;
    NumberUnicodeString.MaximumLength       = 16;
    /*
     * ����������ڴ棬�ǵ��ں����ͷ�����
     */
    NumberUnicodeString.Buffer              = ( PWSTR )ExAllocatePoolWithTag
    (
        PagedPool,
        NumberUnicodeString.MaximumLength,
        PRIVATETAG
    );
    if ( NULL == NumberUnicodeString.Buffer )
    {
        IoDeleteDevice( DeviceObject );
        DeviceObject    = NULL;
        ExFreePoolWithTag
        (
            InternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &InternalNameUnicodeString,
            sizeof( InternalNameUnicodeString )
        );
        return( STATUS_INSUFFICIENT_RESOURCES );
    }
    /*
     * һ���ڲ��豸�Ŵ�0�ƣ��ⲿ�豸�Ŵ�1��
     */
    status                                  = RtlIntegerToUnicodeString
    (
        DeviceNumber + 1,
        10,
        &NumberUnicodeString
    );
    if ( !NT_SUCCESS( status ) )
    {
        IoDeleteDevice( DeviceObject );
        DeviceObject    = NULL;
        ExFreePoolWithTag
        (
            InternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &InternalNameUnicodeString,
            sizeof( InternalNameUnicodeString )
        );
        ExFreePoolWithTag
        (
            NumberUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &NumberUnicodeString,
            sizeof( NumberUnicodeString )
        );
        return( status );
    }
    RtlInitUnicodeString( &SymbolicLinkName, EXTERNALNAME );
    ExternalNameUnicodeString.Length        = SymbolicLinkName.Length + NumberUnicodeString.Length;
    ExternalNameUnicodeString.MaximumLength = ExternalNameUnicodeString.Length + 2;
    ExternalNameUnicodeString.Buffer        = ( PWSTR )ExAllocatePoolWithTag
    (
        PagedPool,
        ExternalNameUnicodeString.MaximumLength,
        PRIVATETAG
    );
    if ( NULL == ExternalNameUnicodeString.Buffer )
    {
        /*
         * VOID IoDeleteDevice
         * (
         *     IN  PDEVICE_OBJECT  DeviceObject
         * );
         *
         * ��Ҫ����IoCreateDevice
         */
        IoDeleteDevice( DeviceObject );
        DeviceObject    = NULL;
        ExFreePoolWithTag
        (
            InternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &InternalNameUnicodeString,
            sizeof( InternalNameUnicodeString )
        );
        ExFreePoolWithTag
        (
            NumberUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &NumberUnicodeString,
            sizeof( NumberUnicodeString )
        );
        return( STATUS_INSUFFICIENT_RESOURCES );
    }
    /*
     * VOID RtlCopyUnicodeString
     * (
     *     IN OUT  PUNICODE_STRING DestinationString,
     *     IN      PUNICODE_STRING SourceString
     * );
     */
    RtlCopyUnicodeString
    (
        &ExternalNameUnicodeString,
        &SymbolicLinkName
    );
    status                                  = RtlAppendUnicodeStringToString
    (
        &ExternalNameUnicodeString,
        &NumberUnicodeString
    );
    /*
     * �Ѿ�����ҪNumberUnicodeString.Buffer�������ͷ�����
     */
    ExFreePoolWithTag
    (
        NumberUnicodeString.Buffer,
        PRIVATETAG
    );
    RtlZeroMemory
    (
        &NumberUnicodeString,
        sizeof( NumberUnicodeString )
    );
    if ( !NT_SUCCESS( status ) )
    {
        ExFreePoolWithTag
        (
            ExternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &ExternalNameUnicodeString,
            sizeof( ExternalNameUnicodeString )
        );
        IoDeleteDevice( DeviceObject );
        DeviceObject    = NULL;
        ExFreePoolWithTag
        (
            InternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &InternalNameUnicodeString,
            sizeof( InternalNameUnicodeString )
        );
        return( status );
    }
    ExternalNameUnicodeString.Buffer[ ExternalNameUnicodeString.Length / 2 ]
                                            = UNICODE_NULL;
    /*
     * NTSTATUS IoCreateSymbolicLink
     * (
     *     IN  PUNICODE_STRING SymbolicLinkName,
     *     IN  PUNICODE_STRING DeviceName
     * );
     */
    status                                  = IoCreateSymbolicLink
    (
        &ExternalNameUnicodeString,
        &InternalNameUnicodeString
    );
    /*
     * �Ѿ�����ҪInternalNameUnicodeString.Buffer��ExternalNameUnicodeString.Buffer
     */
    ExFreePoolWithTag
    (
        ExternalNameUnicodeString.Buffer,
        PRIVATETAG
    );
    RtlZeroMemory
    (
        &ExternalNameUnicodeString,
        sizeof( ExternalNameUnicodeString )
    );
    ExFreePoolWithTag
    (
        InternalNameUnicodeString.Buffer,
        PRIVATETAG
    );
    RtlZeroMemory
    (
        &InternalNameUnicodeString,
        sizeof( InternalNameUnicodeString )
    );
    if ( !NT_SUCCESS( status ) )
    {
        IoDeleteDevice( DeviceObject );
        DeviceObject    = NULL;
    }
    return( STATUS_SUCCESS );
}  /* end of IfhCreateDevice */

/*
 * ��Dispatch������ʵ���Ͼ��ǵ�������������IoDeleteSymbolicLink��
 * IoDeleteDevice��
 */
static VOID IfhDeleteDevice
(
    IN  PDEVICE_OBJECT  DeviceObject
)
{
    NTSTATUS            status;
    PDEVICE_EXTENSION   DeviceExtension;
    UNICODE_STRING      SymbolicLinkName;
    UNICODE_STRING      NumberUnicodeString;
    UNICODE_STRING      ExternalNameUnicodeString;

    KdPrint((  "Entering IfhDeleteDevice()\n" ));
    DeviceExtension                         = ( PDEVICE_EXTENSION )DeviceObject->DeviceExtension;
    NumberUnicodeString.Length              = 0;
    NumberUnicodeString.MaximumLength       = 16;
    /*
     * ����������ڴ棬�ǵ��ں����ͷ�����
     */
    NumberUnicodeString.Buffer              = ( PWSTR )ExAllocatePoolWithTag
    (
        PagedPool,
        NumberUnicodeString.MaximumLength,
        PRIVATETAG
    );
    if ( NULL == NumberUnicodeString.Buffer )
    {
        /*
         * �������һЩ������Ϣ
         *
         * This routine is defined in ntddk.h, wdm.h, and ndis.h.
         * A call to this macro requires double parentheses.
         */
        KdPrint((  "ExAllocatePoolWithTag() for NumberUnicodeString.Buffer failed\n" ));
        return;
    }
    /*
     * һ���ڲ��豸�Ŵ�0�ƣ��ⲿ�豸�Ŵ�1��
     */
    status                                  = RtlIntegerToUnicodeString
    (
        DeviceExtension->DeviceNumber + 1,
        10,
        &NumberUnicodeString
    );
    if ( !NT_SUCCESS( status ) )
    {
        ExFreePoolWithTag
        (
            NumberUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &NumberUnicodeString,
            sizeof( NumberUnicodeString )
        );
        KdPrint((  "RtlIntegerToUnicodeString() failed\n" ));
        return;
    }
    RtlInitUnicodeString( &SymbolicLinkName, EXTERNALNAME );
    ExternalNameUnicodeString.Length        = SymbolicLinkName.Length + NumberUnicodeString.Length;
    ExternalNameUnicodeString.MaximumLength = ExternalNameUnicodeString.Length + 2;
    ExternalNameUnicodeString.Buffer        = ( PWSTR )ExAllocatePoolWithTag
    (
        PagedPool,
        ExternalNameUnicodeString.MaximumLength,
        PRIVATETAG
    );
    if ( NULL == ExternalNameUnicodeString.Buffer )
    {
        ExFreePoolWithTag
        (
            NumberUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &NumberUnicodeString,
            sizeof( NumberUnicodeString )
        );
        KdPrint((  "ExAllocatePoolWithTag() for ExternalNameUnicodeString.Buffer failed\n" ));
        return;
    }
    /*
     * VOID RtlCopyUnicodeString
     * (
     *     IN OUT  PUNICODE_STRING DestinationString,
     *     IN      PUNICODE_STRING SourceString
     * );
     */
    RtlCopyUnicodeString
    (
        &ExternalNameUnicodeString,
        &SymbolicLinkName
    );
    status                                  = RtlAppendUnicodeStringToString
    (
        &ExternalNameUnicodeString,
        &NumberUnicodeString
    );
    /*
     * �Ѿ�����ҪNumberUnicodeString.Buffer�������ͷ�����
     */
    ExFreePoolWithTag
    (
        NumberUnicodeString.Buffer,
        PRIVATETAG
    );
    RtlZeroMemory
    (
        &NumberUnicodeString,
        sizeof( NumberUnicodeString )
    );
    if ( !NT_SUCCESS( status ) )
    {
        ExFreePoolWithTag
        (
            ExternalNameUnicodeString.Buffer,
            PRIVATETAG
        );
        RtlZeroMemory
        (
            &ExternalNameUnicodeString,
            sizeof( ExternalNameUnicodeString )
        );
        KdPrint((  "RtlAppendUnicodeStringToString() failed\n" ));
        return;
    }
    ExternalNameUnicodeString.Buffer[ ExternalNameUnicodeString.Length / 2 ]
                                            = UNICODE_NULL;
    /*
     * NTSTATUS IoDeleteSymbolicLink
     * (
     *     IN  PUNICODE_STRING SymbolicLinkName
     * );
     */
    status                                  = IoDeleteSymbolicLink
    (
        &ExternalNameUnicodeString
    );
    /*
     * �Ѿ�����ҪExternalNameUnicodeString.Buffer
     */
    ExFreePoolWithTag
    (
        ExternalNameUnicodeString.Buffer,
        PRIVATETAG
    );
    RtlZeroMemory
    (
        &ExternalNameUnicodeString,
        sizeof( ExternalNameUnicodeString )
    );
    if ( !NT_SUCCESS( status ) )
    {
        KdPrint((  "IoDeleteSymbolicLink() failed\n" ));
        return;
    }
    /*
     * VOID IoDeleteDevice
     * (
     *     IN  PDEVICE_OBJECT  DeviceObject
     * );
     */
    IoDeleteDevice( DeviceObject );
    return;
}  /* end of IfhDeleteDevice */

/*
 * Handles call from Win32 CloseHandle request. For IpFilterHook driver, frees
 * any buffer.
 */
static NTSTATUS IfhDispatchClose
(
    IN  PDEVICE_OBJECT  DeviceObject,
    IN  PIRP            Irp
)
{
    KdPrint((  "Entering IfhDispatchClose()\n" ));
    Irp->IoStatus.Status        = STATUS_SUCCESS;
    Irp->IoStatus.Information   = 0;
    IoCompleteRequest( Irp, IO_NO_INCREMENT );
    return( STATUS_SUCCESS );
}  /* end of IfhDispatchClose */

/*
 * Handles call from Win32 CreateFile request. For IpFilterHook driver, does
 * nothing.
 */
static NTSTATUS IfhDispatchCreate
(
    IN  PDEVICE_OBJECT  DeviceObject,
    IN  PIRP            Irp
)
{
    KdPrint((  "Entering IfhDispatchCreate()\n" ));
    /*
     * ����IRP���������Ǵ��д���ģ�����I/O�������Ტ��ʹ��IRP��һ����ԣ�
     * Dispatch������Ҫ�޸�IRP�г�Աʱ��Ӧ����ջ�ϻ��豸��չ�н���һ��������
     */
    Irp->IoStatus.Status        = STATUS_SUCCESS;
    /*
     * report that no bytes were transfered
     */
    Irp->IoStatus.Information   = 0;
    /*
     * VOID IoCompleteRequest
     * (
     *     IN  PIRP    Irp,
     *     IN  CCHAR   PriorityBoost
     * );
     *
     * IoCompleteRequest indicates the caller has completed all processing
     * for a given I/O request and is returning the given IRP to the I/O
     * Manager.
     *
     * PriorityBoost is IO_NO_INCREMENT if the original thread requested
     * an operation the driver could complete quickly or if the IRP is
     * completed with an error.
     *
     * Mark the IRP as "complete" - no further processing, no priority
     * increment.
     */
    IoCompleteRequest( Irp, IO_NO_INCREMENT );
    /*
     * ����IoCompleteRequest()֮��I/O���������ԴӷǷ�ҳ�����ͷ�IRP�����
     * ���ܳ���return( Irp->IoStatus.Status )�����Ĵ��롣
     */
    return( STATUS_SUCCESS );
}  /* end of IfhDispatchCreate */

/*
 * Handles call from Win32 DeviceIoControl request.
 */
static NTSTATUS IfhDispatchDeviceControl
(
    IN  PDEVICE_OBJECT  DeviceObject,
    IN  PIRP            Irp
)
{
    NTSTATUS            status;
    PIO_STACK_LOCATION  IrpStackLocation;
    ULONG               IoControlCode;
    ULONG               InputBufferLength;
    ULONG               OutputBufferLength;
    ULONG               TransferSize;
    PVOID               TransferBuffer;

    KdPrint((  "Entering IfhDispatchDeviceControl()\n" ));
    status              = STATUS_SUCCESS;
    TransferSize        = 0;
    TransferBuffer      = Irp->AssociatedIrp.SystemBuffer;
    IrpStackLocation    = IoGetCurrentIrpStackLocation( Irp );
    IoControlCode       = IrpStackLocation->Parameters.DeviceIoControl.IoControlCode;
    InputBufferLength   = IrpStackLocation->Parameters.DeviceIoControl.InputBufferLength;
    OutputBufferLength  = IrpStackLocation->Parameters.DeviceIoControl.OutputBufferLength;
    switch ( IoControlCode )
    {
    case IOCTL_IPFILTERHOOK_GET_FIREWALLFLAGS:
        if ( OutputBufferLength != sizeof( FirewallFlags ) )
        {
            status  = STATUS_INVALID_BUFFER_SIZE;
            break;
        }
        TransferSize    = OutputBufferLength;
        /*
         * ����FirewallFlags
         */
        RtlCopyMemory
        (
            TransferBuffer,
            &FirewallFlags,
            TransferSize
        );
        break;
    case IOCTL_IPFILTERHOOK_SET_FIREWALLFLAGS:
        if ( InputBufferLength != sizeof( FirewallFlags ) )
        {
            status  = STATUS_INVALID_BUFFER_SIZE;
            break;
        }
        TransferSize    = InputBufferLength;
        /*
         * ����FirewallFlags
         */
        RtlCopyMemory
        (
            &FirewallFlags,
            TransferBuffer,
            TransferSize
        );
        break;
    default:
        status          = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }
    /*
     * Now complete the IRP
     */
    Irp->IoStatus.Status                = status;
    Irp->IoStatus.Information           = TransferSize;
    IoCompleteRequest( Irp, IO_NO_INCREMENT );
    return( status );
}  /* end of IfhDispatchDeviceControl */

static NTSTATUS IfhDoHook ( PacketFilterExtensionPtr fwfunc )
{
    NTSTATUS                    status;
    UNICODE_STRING              DeviceName;
    PDEVICE_OBJECT              DeviceObject;
    PFILE_OBJECT                FileObject  = NULL;
    PIRP                        Irp         = NULL;
    PF_SET_EXTENSION_HOOK_INFO  hookinfo;
    IO_STATUS_BLOCK             IoStatusBlock;

    RtlInitUnicodeString( &DeviceName, DD_IPFLTRDRVR_DEVICE_NAME );
    /*
     * NTSTATUS IoGetDeviceObjectPointer
     * (
     *     IN  PUNICODE_STRING     ObjectName,
     *     IN  ACCESS_MASK         DesiredAccess,
     *     OUT PFILE_OBJECT       *FileObject,
     *     OUT PDEVICE_OBJECT     *DeviceObject
     * );
     */
    status                      = IoGetDeviceObjectPointer
    (
        &DeviceName,
        SYNCHRONIZE | GENERIC_READ | GENERIC_WRITE,
        &FileObject,
        &DeviceObject
    );
    if ( !NT_SUCCESS( status ) )
    {
        KdPrint((  "IoGetDeviceObjectPointer() failed\n" ));
        goto IfhDoHook_exit;
    }
    hookinfo.ExtensionPointer   = fwfunc;
    /*
     * PIRP IoBuildDeviceIoControlRequest
     * (
     *     IN  ULONG               IoControlCode,
     *     IN  PDEVICE_OBJECT      DeviceObject,
     *     IN  PVOID               InputBuffer     OPTIONAL,
     *     IN  ULONG               InputBufferLength,
     *     OUT PVOID               OutputBuffer    OPTIONAL,
     *     IN  ULONG               OutputBufferLength,
     *     IN  BOOLEAN             InternalDeviceIoControl,
     *     IN  PKEVENT             Event,
     *     OUT PIO_STATUS_BLOCK    IoStatusBlock
     * );
     */
    Irp                         = IoBuildDeviceIoControlRequest
    (
        IOCTL_PF_SET_EXTENSION_POINTER,
        DeviceObject,
        &hookinfo,
        sizeof( hookinfo ),
        NULL,
        0,
        FALSE,
        NULL,
        &IoStatusBlock
    );
    if ( NULL == Irp )
    {
        status  = IoStatusBlock.Status;
        KdPrint((  "IoBuildDeviceIoControlRequest() failed\n" ));
        goto IfhDoHook_exit;
    }
    /*
     * NTSTATUS IoCallDriver
     * (
     *     IN      PDEVICE_OBJECT  DeviceObject,
     *     IN OUT  PIRP            Irp
     * );
     */
    status                      = IoCallDriver
    (
        DeviceObject,
        Irp
    );
    if ( !NT_SUCCESS( status ) )
    {
        KdPrint((  "IoCallDriver() failed\n" ));
    }
    /*
     * IRPs created using IoBuildDeviceIoControlRequest must be completed
     * by calling IoCompleteRequest and not by merely deallocating the IRP
     * with IoFreeIrp.
     */
    Irp                         = NULL;

IfhDoHook_exit:

    if ( NULL != FileObject )
    {
        ObDereferenceObject( FileObject );
        FileObject  = NULL;
    }
    return( status );
}  /* end of IfhDoHook */

static VOID IfhDriverUnload
(
    IN  PDRIVER_OBJECT  DriverObject
)
{
    PDEVICE_OBJECT      NextDeviceObject;
    PDEVICE_EXTENSION   DeviceExtension;

    KdPrint((  "Entering IfhDriverUnload()\n" ));
    /*
     * Loop through each device controlled by driver
     */
    NextDeviceObject    = DriverObject->DeviceObject;
    /*
     * ���Ǹ������ѭ������
     */
    while ( NextDeviceObject )
    {
        /*
         * Dig out the Device Extension from the Device Object
         */
        DeviceExtension     = ( PDEVICE_EXTENSION )NextDeviceObject->DeviceExtension;
        NextDeviceObject    = NextDeviceObject->NextDevice;
        IfhDeleteDevice( DeviceExtension->DeviceObject );
    }  /* end of while */
    /*
     * ����ص�����
     */
    if ( !NT_SUCCESS( IfhDoHook( NULL ) ) )
    {
        KdPrint((  "IfhDoHook( NULL ) failed\n" ));
    }
    return;
}  /* end of IfhDriverUnload */

/*
 * ��������̵�������ص�������ʱ��ǰ���Ƿ��Ѿ�����һЩ��飬��ȫ�������
 * ����һЩ���Ϊ�
 */
static PF_FORWARD_ACTION IfhFirewall
(
    IN  unsigned char  *PacketHeader,
    IN  unsigned char  *Packet,
    /*
     * This size does not include the size of the IP header
     */
    IN  unsigned int    PacketLength,
    IN  unsigned int    RecvInterfaceIndex,
    IN  unsigned int    SendInterfaceIndex,
    IN  IPAddr          RecvLinkNextHop,
    IN  IPAddr          SendLinkNextHop
)
{
    /*
     * ԭ���ǣ���δ��ȷ��ֹ���������Ҹ��û�̬Packet Filtering APIһ����
     * �ᡣ
     */
    PF_FORWARD_ACTION   action  = PF_PASS;
    /*
     * IP�ײ����ݿ϶���λ��
     */
    struct ipheader    *iph     = ( struct ipheader * )PacketHeader;
    struct tcpheader   *tcph    = ( struct tcpheader * )Packet;

    if
    (
        ( IPPROTO_ICMP == iph->ip_p ) &&
        ( !( FirewallFlags & FIREWALLFLAGS_ALLICMP ) )
    )
    {
        /*
         * ��������ICMP����
         */
        action  = PF_DROP;
        goto IfhFirewall_exit;
    }
    /*
     * For received packets, SendInterfaceIndex is set to INVALID_PF_IF_INDEX
     */
    if
    (
        ( IPPROTO_TCP == iph->ip_p ) &&
        ( INVALID_PF_IF_INDEX == SendInterfaceIndex ) &&
        ( !( FirewallFlags & FIREWALLFLAGS_INBOUNDRST ) )
    )
    {
        /*
         * �������н��յ���RST���ġ�
         *
         * ����Ƿ�����Ч��TCP���ģ����Ƕ���raw socket���͵ı��ġ�
         */
        if ( PacketLength >= OFFSETOF( struct tcpheader *, th_win ) )
        {
            if ( tcph->th_flags & TH_RST )
            {
                action  = PF_DROP;
            }
        }
    }

IfhFirewall_exit:

    return( action );
}  /* end of IfhFirewall */

/*
 * DriverEntry is the first routine called after a driver is loaded, and
 * is responsible for initializing the driver.
 *
 * you'll find a list of NTSTATUS status codes in the DDK header
 * ntstatus.h (\WINDDK\2600.1106\inc\ddk\wxp\)
 */
NTSTATUS DriverEntry
(
    IN  PDRIVER_OBJECT  DriverObject,
    IN  PUNICODE_STRING RegistryPath
)
{
    NTSTATUS    status;

    /*
     * kernel-mode functions and the functions in your driver use the
     * __stdcall calling convention when compiled for an x86 computer.
     * This shouldn't affect any of your programming, but it's something
     * to bear in mind when you're debugging
     *
     * This routine has no effect if compiled in a free build environment.
     * You should compiled in a checked build environment.
     */
    KdPrint((  "Entering DriverEntry()\n" ));

    /*
     * If this driver controlled real hardware, code would be placed here
     * to locate it. Using IoReportDetectedDevice, the ports, IRQs, and
     * DMA channels would be "marked" as "in use" and under the control of
     * this driver. This IpFilterHook driver has no HW, so...
     */

    /*
     * Announce other driver entry points
     */
    DriverObject->DriverUnload                              = IfhDriverUnload;
    /*
     * This includes Dispatch routines for Create, Write & Read
     */
    DriverObject->MajorFunction[IRP_MJ_CREATE           ]   = IfhDispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL   ]   = IfhDispatchDeviceControl;
    DriverObject->MajorFunction[IRP_MJ_CLOSE            ]   = IfhDispatchClose;
    /*
     * ��<<Programming the Microsoft Windows Driver Model, 2nd Ed>>������
     * ��"Error Handling"С�ڡ�
     *
     * Which Exceptions Can Be Trapped
     *
     * Gary Nebbett researched the question of which exceptions can be
     * trapped with the structured exception mechanism and reported his
     * results in a newsgroup post several years ago. In summary, the
     * following exceptions will be caught when they occur at IRQL less
     * than or equal to DISPATCH_LEVEL (note that some of these are
     * specific to the Intel x86 processor):
     *
     * a. Anything signaled by ExRaiseStatus and related functions
     * b. Attempt to dereference invalid pointer to user-mode memory
     * c. Debug or breakpoint exception
     * d. Integer overflow (INTO instruction)
     * e. Invalid opcode
     *
     * Note that a reference to an invalid kernel-mode pointer leads
     * directly to a bug check and can��t be trapped. Likewise, a
     * divide-by-zero exception or a BOUND instruction exception leads to
     * a bug check.
     *
     * �������������ʹ��SEH���ưɣ������������ʱ������
     */
    __try
    {
        KdPrint((  "You should see this message [0]\n" ));
        /*
         * ���ûص�����
         */
        status  = IfhDoHook( IfhFirewall );
        if ( !NT_SUCCESS( status ) )
        {
            KdPrint((  "IfhDoHook( IfhFirewall ) failed\n" ));
        }
        else
        {
            /*
             * For each physical or logical device detected that will be under
             * this Driver's control, a new Device Object must be created.
             *
             * This call would be repeated until all devices are created.
             *
             * ��������ֻ������һ���豸����
             */
            status  = IfhCreateDevice
            (
                DriverObject,
                0
            );
        }
        KdPrint((  "You should see this message [1]\n" ));
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        KdPrint((  "__except{}\n" ));
        status  = STATUS_UNSUCCESSFUL;
    }
    KdPrint((  "Exiting DriverEntry()\n" ));
    return( status );
}  /* end of DriverEntry */

/************************************************************************/