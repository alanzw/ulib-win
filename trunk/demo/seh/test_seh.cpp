#if 0

#include <stdio.h>

int main()
{
    int a(0);
    int b(0);
    int c(0);
    __try
    {
        //
        a=c/b;
    }
    __except(1)
    {
        //
        printf("except1 \n");
    }

#if 0
    __try
    {
        __asm
        {
            xor eax,eax
            mov dword ptr [eax],0

        }
    }
    __except(1)
    {
        printf("except2 \n");

    }
#endif

#if 0
    __try
    {
        __asm
        {
            mov cr0,eax;
        };
    }
    __except(1)
    {
        puts("execpt3 \n");
    }
#endif

    return 0;
}


try

{

    // try block

}

except (FilterFunction(GetExceptionInformation())

{

    // exception handler block

}

#endif


#include <windows.h>

#include <dbghelp.h>

#pragma auto_inline (off)

#pragma comment( lib, "dbghelp" )

LONG WINAPI MyUnhandledExceptionFilter( struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    HANDLE lhDumpFile = CreateFile(_T("DumpFile.dmp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);

    MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
    loExceptionInfo.ExceptionPointers = ExceptionInfo;
    loExceptionInfo.ThreadId = GetCurrentThreadId();
    loExceptionInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);

    CloseHandle(lhDumpFile);

    return EXCEPTION_EXECUTE_HANDLER;
}

void Fun2()
{
    __try
    {
       static bool b = false;
       if(!b)
       {
           b = true;
           int *p = NULL;
           *p = 0;
        }
       else
       {
           MessageBox(NULL, _T("Here"), _T(""), MB_OK);
       }
    }
    __except(MyUnhandledExceptionFilter(GetExceptionInformation()))
    {
    }
}


void Fun()
{
    Fun2();
}

int main()
{
    Fun();
    Fun();
    return 1;
}

