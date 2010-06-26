#include <cstring>
#include <cstdio>
#include "crypto/base64.h"

namespace huys
{

namespace crypto
{

const char base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void BASE64::encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
    out[0] = base64_alphabet[ in[0] >> 2 ];
    out[1] = base64_alphabet[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? base64_alphabet[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? base64_alphabet[ in[2] & 0x3f ] : '=');
}

static unsigned char base64_table_pos(unsigned char c)
{
    char * p = strrchr(base64_alphabet, c);
    return (p - base64_alphabet);
}

/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
void BASE64::decodeblock( unsigned char in[4], unsigned char out[3] )
{
    out[ 0 ] = (unsigned char ) ( base64_table_pos(in[0]) << 2 | base64_table_pos(in[1]) >> 4);
    out[ 1 ] = (unsigned char ) ( base64_table_pos(in[1]) << 4 | (in[2] == '=' ? '\0' : base64_table_pos(in[2]) >> 2));
    out[ 2 ] = (unsigned char ) ( (in[2] == '=' ? '\0' : (base64_table_pos(in[2]) << 6)) | (in[3] == '=' ? '\0' : base64_table_pos(in[3])));

    //printf("%d %d %d %d\n", in[0], in[1], in[2], in[3]);
    //printf("%d %d %d\n", out[0], out[1], out[2]);
}



char* BASE64::encode(const char* data, int len, char* dest)
{
    if (!data || !dest)
    {
        return 0;
    }
    int dv = len/3;
    int md = len%3;
    //int dest_len = (md == 0 ? dv*4 : dv*4+4);

    unsigned char tmp[3];
    unsigned char * pSrc = (unsigned char *)data;
    unsigned char * pDest = (unsigned char *)dest;

    int i = 0;
    for (i=0; i<dv; ++i)
    {
        memcpy(tmp, pSrc, 3);
        encodeblock(tmp, pDest, 3);
        pDest += 4;
        pSrc += 3;
    }

    if (md != 0)
    {
        memset(tmp, 0, 3);
        memcpy(tmp, pSrc, md);
        encodeblock(tmp, pDest, md);
        pDest += 4;
    }

    *pDest = '\0';

    return dest;
}

char* BASE64::decode(const char* data, int len, char* dest)
{
    if (!data || !dest)
    {
        return 0;
    }

    int dv = len/4;
    int md = len%4;
    int dest_len = (md == 0 ? dv*3 : dv*3+3);

    unsigned char tmp[4];
    const unsigned char * pSrc = (unsigned char *)data;
    unsigned char * pDest = (unsigned char *)dest;

    int i = 0;
    for (i=0; i<dv; ++i)
    {
        memcpy(tmp, pSrc, 4);
        decodeblock(tmp, pDest);
        pSrc += 4;
        pDest += 3;
    }

    if (md != 0)
    {
        memset(tmp, 0, 4);
        memcpy(tmp, pSrc, md);
        decodeblock(tmp, pDest);
        //pSrc += md;
        //pDest += 4;
    }

    //*pDest = '\0';

    dest[dest_len] = '\0';
    return dest;
}

}; // namespace crypto

}; // namespace huys

