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
 * 用到了CTL_CODE宏
 */
#include <devioctl.h>

/************************************************************************
 *                                                                      *
 *                               Macro                                  *
 *                                                                      *
 ************************************************************************/

/*
 * 后面要追加DeviceNumber
 */
#define INTERNALNAME                            L"\\Device\\IpFilterHookInternal"
/*
 * DosDevices
 */
#define EXTERNALNAME                            L"\\??\\IpFilterHookExternal"
/*
 * 参ExAllocatePoolWithTag第三形参的DDK文档
 */
#define PRIVATETAG                              'OFSN'
/*
 * 0x0800是最小可用值
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
 * 置位 - Enable
 * 复位 - Disable
 */
#define FIREWALLFLAGS_DISABLEALL                0x00000000
/*
 * 所有ICMP报文，包括接收、发送
 */
#define FIREWALLFLAGS_ALLICMP                   0x00000001
/*
 * 接收到的RST报文
 */
#define FIREWALLFLAGS_INBOUNDRST                0x00000002
#define FIREWALLFLAGS_ENABLEALL                 0xFFFFFFFF
/*
 * 缺省设置是全部允许，即不丢弃任何报文
 */
#define FIREWALLFLAGS_DEFAULT                   FIREWALLFLAGS_ENABLEALL

/*
 * 设备扩展是自定义结构
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
 * 重复定义是为了方便移植
 */
#define IPTOS_LOWDELAY                          0x10
#define IPTOS_THROUGHPUT                        0x08
#define IPTOS_RELIABILITY                       0x04
#define IPTOS_MINCOST                           0x02

#define IPPROTO_ICMP                            1
#define IPPROTO_TCP                             6
#define IPPROTO_UDP                             17

/*
 * 自定义下面这些结构是为了方便移植
 */

#pragma pack( push, 1 )

/*
 * Structure of an internet header, naked of options.
 */
struct ipheader
{
#ifdef LITTLE_ENDIAN
    /*
     * 0x45，LITTLE_ENDIAN序高位在后。ip_hl以4字节为单位。
     */
    unsigned char       ip_hl:4;    /* header length         */
    unsigned char       ip_v:4;     /* version               */
#else
    /*
     * 0x45，BIG_ENDIAN序高位在前
     */
    unsigned char       ip_v:4;     /* version               */
    unsigned char       ip_hl:4;    /* header length         */
#endif
    unsigned char       ip_tos;     /* type of service       */
    /*
     * ip_len以字节为单位
     */
    unsigned short int  ip_len;     /* total length          */
    unsigned short int  ip_id;      /* identification        */
    /*
     * ip_off以8字节为单位
     */
    unsigned short int  ip_off;     /* fragment offset field */
    /*
     * ip_off只占了后13-bits，前3-bits的后两位分别是DF、MF
     */
#define IP_DF 0x4000                /* dont fragment flag    */
#define IP_MF 0x2000                /* more fragments flag   */
    unsigned char       ip_ttl;     /* time to live          */
    unsigned char       ip_p;       /* protocol              */
    unsigned short int  ip_sum;     /* checksum              */
#if 0
    /*
     * 我们用不着这两个成员，为减少编译麻烦，注释掉它们
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
     * 0x50，LITTLEENDIAN序高位在后
     */
    unsigned char       th_x2:4,    /* (unused)               */
                        th_off:4;   /* data offset            */
#else
    /*
     * 0x50，BIGENDIAN序高位在前。th_off以4字节为单位。
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
     * 参看W.Richard Stevens的Tcp/Ip Illustrated Volume I 18.10小节
     *
     * kind=2 len=4 mss(不包括IP首部、TCP首部)
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
 * 非Dispatch函数
 */
static NTSTATUS             IfhCreateDevice
(
    IN  PDRIVER_OBJECT          DriverObject,
    IN  ULONG                   DeviceNumber
);
/*
 * 非Dispatch函数
 */
static VOID                 IfhDeleteDevice
(
    IN  PDEVICE_OBJECT          DeviceObject
);
/*
 * 在DDK文档中搜索DispatchClose
 */
static NTSTATUS             IfhDispatchClose
(
    IN  PDEVICE_OBJECT          DeviceObject,
    IN  PIRP                    Irp
);
/*
 * 在DDK文档中搜索DispatchCreate
 */
static NTSTATUS             IfhDispatchCreate
(
    IN  PDEVICE_OBJECT          DeviceObject,
    IN  PIRP                    Irp
);
/*
 * 在DDK文档中搜索DispatchDeviceControl
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
 * 非Dispatch函数，在DDK文档中搜索Unload
 */
static VOID                 IfhDriverUnload
(
    IN  PDRIVER_OBJECT  DriverObject
);
/*
 * 回调函数，在DDK文档中搜索PacketFilterExtensionPtr
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
 * 惟一的引出函数
 */
       NTSTATUS             DriverEntry
(
    IN  PDRIVER_OBJECT          DriverObject,
    IN  PUNICODE_STRING         RegistryPath
);

/*
 * 参看<<The Windows 2000 Device Driver Book, Second Edition>>中5.2.6、
 * 5.2.7小节。INIT、PAGE应该是大小写敏感的，可我居然看到过init、page，不清
 * 楚怎么回事，稳妥起见还是用INIT、PAGE算了。
 *
 * IfhFirewall被调用时处在什么IRQL上，我也不清楚，安全起见，不放这里了。
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
 * 由于回调函数IfhFirewall要使用该标志，因此设为静态全局变量。
 */
static ULONG    FirewallFlags   = FIREWALLFLAGS_DEFAULT;

/************************************************************************/

/*
 * 非Dispatch函数。如果不做错误处理，函数将精简很多，实际上就是调用两个函
 * 数，IoCreateDevice与IoCreateSymbolicLink，分别建立内部设备名、外部设备
 * 名。
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
     * 0xFFFFFFFF转换成10进制是4294967295，最长10个字符，加上结尾的NUL，不
     * 超过11。
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
     * 这里分配了内存，记得在后面释放它。
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
         * DDK文档中指出，如果ExAllocatePool返回NULL，主调者应该返回
         * STATUS_INSUFFICIENT_RESOURCES。但是DDK文档中没有指出
         * ExAllocatePoolWithTag返回NULL时该如何，我只好类比一下，也返回
         * STATUS_INSUFFICIENT_RESOURCES。不知这里是否可以返回
         * STATUS_NO_MEMORY。
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
     * 这个函数没有动态分配内存
     */
    RtlInitUnicodeString( &DeviceName, INTERNALNAME );
    /*
     * 在后面追加DeviceNumber。没有wsprintf()可用，不得已，只好这样变态地
     * 处理。
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
         * 需要释放NumberUnicodeString.Buffer
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
     * 已经不需要NumberUnicodeString.Buffer，趁早释放它。
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
     * 设备扩展的内存空间是由IoCreateDevice给予的
     */
    DeviceExtension                         = ( PDEVICE_EXTENSION )DeviceObject->DeviceExtension;
    DeviceExtension->DeviceObject           = DeviceObject;
    DeviceExtension->DeviceNumber           = DeviceNumber;
    /*
     * 下面开始处理SymbolicLink
     */
    NumberUnicodeString.Length              = 0;
    NumberUnicodeString.MaximumLength       = 16;
    /*
     * 这里分配了内存，记得在后面释放它。
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
     * 一般内部设备号从0计，外部设备号从1计
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
         * 需要抵消IoCreateDevice
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
     * 已经不需要NumberUnicodeString.Buffer，趁早释放它。
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
     * 已经不需要InternalNameUnicodeString.Buffer、ExternalNameUnicodeString.Buffer
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
 * 非Dispatch函数。实际上就是调用两个函数，IoDeleteSymbolicLink与
 * IoDeleteDevice。
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
     * 这里分配了内存，记得在后面释放它。
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
         * 考虑输出一些调试信息
         *
         * This routine is defined in ntddk.h, wdm.h, and ndis.h.
         * A call to this macro requires double parentheses.
         */
        KdPrint((  "ExAllocatePoolWithTag() for NumberUnicodeString.Buffer failed\n" ));
        return;
    }
    /*
     * 一般内部设备号从0计，外部设备号从1计
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
     * 已经不需要NumberUnicodeString.Buffer，趁早释放它。
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
     * 已经不需要ExternalNameUnicodeString.Buffer
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
     * 尽管IRP对于驱动是串行传输的，但是I/O管理器会并行使用IRP。一般而言，
     * Dispatch例程需要修改IRP中成员时，应该在栈上或设备扩展中建立一个副本。
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
     * 调用IoCompleteRequest()之后，I/O管理器可以从非分页池中释放IRP，因此
     * 不能出现return( Irp->IoStatus.Status )这样的代码。
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
         * 返回FirewallFlags
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
         * 设置FirewallFlags
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
     * 这是个单向非循环链表
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
     * 清除回调函数
     */
    if ( !NT_SUCCESS( IfhDoHook( NULL ) ) )
    {
        KdPrint((  "IfhDoHook( NULL ) failed\n" ));
    }
    return;
}  /* end of IfhDriverUnload */

/*
 * 不清楚流程到达这个回调函数的时候前面是否已经做过一些检查，安全起见还是
 * 多做一些检查为妙。
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
     * 原则是，如未明确禁止就允许，并且给用户态Packet Filtering API一个机
     * 会。
     */
    PF_FORWARD_ACTION   action  = PF_PASS;
    /*
     * IP首部数据肯定到位了
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
         * 丢弃所有ICMP报文
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
         * 丢弃所有接收到的RST报文。
         *
         * 检查是否是有效的TCP报文，考虑动用raw socket发送的报文。
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
     * 参<<Programming the Microsoft Windows Driver Model, 2nd Ed>>第三章
     * 中"Error Handling"小节。
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
     * directly to a bug check and can’t be trapped. Likewise, a
     * divide-by-zero exception or a BOUND instruction exception leads to
     * a bug check.
     *
     * 稳妥起见，还是使用SEH机制吧，尽量避免调试时重启。
     */
    __try
    {
        KdPrint((  "You should see this message [0]\n" ));
        /*
         * 设置回调函数
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
             * 我们这里只创建了一个设备对象
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