#include <cstdio>

#include "crypto/base64.h"

/*
  "1" 0x31

  origin : 00110001
  encode : 00001100 00010000 00111101 00111101
  decode : 00110001 00000000 00000000
 */

int main()
{
    static const char *const test = "0LvQu8T6xM3XxdDU19O/tM3qztK1xEJhc2U2NL3Ms8yjoSCjuqOp";
    //char test[] = "1";
    char dest[1024] = {0};
    char str[1024] = {0};
    int i;

    typedef huys::crypto::BASE64 base64;
    typedef unsigned char uchar;

    base64 engine;

    printf("src %d: %s \n", strlen(test), test);
    engine.decode(test, strlen(test), dest);
    printf("out %d: %s \n", strlen(dest), dest);
    engine.encode(dest, strlen(dest), str);
    printf("str %d: %s \n", strlen(str), str);

    return 0;
}
