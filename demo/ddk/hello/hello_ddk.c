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

NTSTATUS DriverEntry(PDRIVER_OBJECT driver,
PUNICODE_STRING reg_path)
{
#if HU_DEBUG

#ifdef _MSC_VER
    _asm int 3
#endif // _MSC_VER

#ifdef __GNUC__
    __asm__ ("int 3");
#endif // __GNUC__

#endif

    DbgPrint("first, hello my first driver\r\n");
    driver->DriverUnload = DriverUnload;

    return STATUS_SUCCESS;
}

