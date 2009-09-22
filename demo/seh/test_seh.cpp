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

