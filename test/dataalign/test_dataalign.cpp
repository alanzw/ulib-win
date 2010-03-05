#include <stdio.h>

#include <windows.h>

int mswindows_handle_hardware_exceptions (DWORD code)
{
   printf("Handling exception\n");
    if (code == STATUS_DATATYPE_MISALIGNMENT)
   {
      printf("misalignment fault!\n");
         return EXCEPTION_EXECUTE_HANDLER;
   }
    else
      return EXCEPTION_CONTINUE_SEARCH;
}

void test()
{
    __try {
        char temp[10];
        memset(temp, 0, 10);
        double *val;
        val = (double *)(&temp[3]);
        printf("%lf\n", *val);
    }
    __except(mswindows_handle_hardware_exceptions (GetExceptionCode ()))
    {}

}


int main()
{
    char a;
    char b;
    class S1
    {
    public:
        char m_1;         // 1-byte element
                          // 3-bytes of padding are placed here
        int m_2;          // 4-byte element
        double m_3, m_4;  // 8-byte elements
    };
    S1 x;
    long y;
    S1 z[5];

    printf("sizeof S1 : %d\n\n", sizeof(S1));

    printf("a     = %p\n", &a);
    printf("b     = %p\n", &b);
    printf("x     = %p\n", &x);
    printf("x.m_1 = %p\n", &x.m_1);
    printf("x.m_2 = %p\n", &x.m_2);
    printf("x.m_3 = %p\n", &x.m_3);
    printf("x.m_4 = %p\n", &x.m_4);
    printf("y     = %p\n", &y);
    printf("z[0]  = %p\n", z);
    printf("z[1]  = %p\n", &z[1]);
    
    test();

    return 0;
}
