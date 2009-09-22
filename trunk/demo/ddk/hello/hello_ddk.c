/**
 * Add a serial port COM1 to vmware XP
 *
 *
 * Cmd:
 *   attrib -h -s -a -r boot.ini
 *
 * Add boot option:
 *   /debug /debugport=com1 /baudrate=115200
 *
 * Cmd:
 *   attrib +h +s +a +r boot.ini
 *
 * Install driver with SRVINSTW.EXE
 *
 * Host:
 *   windbg.exe -k com:port=\\.pipe\com_1,baud=115200,pipe
 *
 * Cmd:
 *   net start first
 *
 * Cmd:
 *   net stop first
 */

// first.c
#ifdef __GNUC__
  #include <ddk/ntddk.h>
#else
  #include <ntddk.h>
#endif

VOID DDKAPI DriverUnload(PDRIVER_OBJECT driver)
{
    DbgPrint("first: Our driver is unloading...\r\n");
}

NTSTATUS DDKAPI DriverEntry(PDRIVER_OBJECT driver,
PUNICODE_STRING reg_path)
{
#ifdef HU_DEBUG

#ifdef _MSC_VER
    _asm int 3
#endif // _MSC_VER

#ifdef __GNUC__
    __asm__ ("int $3");
#endif // __GNUC__

#endif

    DbgPrint("first, hello my first driver\r\n");
    driver->DriverUnload = DriverUnload;

    return STATUS_SUCCESS;
}

