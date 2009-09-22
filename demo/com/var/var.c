#include <windows.h>

#include <stdio.h>

int main()
{
    long x = -1000;

    VARIANT v;

    VARIANT e;

    VARIANT t;

    v.vt = VT_I4;
    v.lVal = x;

    VarAbs(&v, &e);
    printf("%d --> %d\n", v.lVal, e.lVal);



    VarAdd(&v, &e, &t);
    printf("%d + %d --> %d\n", v.lVal, e.lVal, t.lVal);

    VarMul(&v, &e, &t);
    printf("%d * %d --> %d\n", v.lVal, e.lVal, t.lVal);

    return 0;
}

