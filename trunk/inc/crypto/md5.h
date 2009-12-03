#ifndef HUYS_MD5_H
#define HUYS_MD5_H

#include "ulib.h"

namespace huys
{

namespace crypto
{

class ULIB_API MD5
{
    typedef unsigned long uint32;
    typedef unsigned char uchar;
    typedef struct MD5Context {
        uchar in[64];
        uint32 buf[4];
        uint32 bits[2];
        int doByteReverse;
    } MD5_CTX;
public:
    void init(int brokenEndian = 0);
    void update(uchar const * buf, unsigned len);
    void final(unsigned char digest[16]);
    void transform(uint32 buf[4], uint32 const in[16]);
private:
    MD5_CTX _ctx;
private:
    void byte_swap(uchar &a, uchar &b)
    {
        a ^= b;
        b ^= a; 
        a ^= b;
    }
    void byte_reverse(uchar *buf, unsigned longs)
    {
        /*uint32 t;*/
        do {
        /*
            t = (uint32) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
                ((unsigned) buf[1] << 8 | buf[0]);
            *(uint32 *) buf = t;
        */
            byte_swap(buf[0], buf[3]);
            byte_swap(buf[1], buf[2]);
            buf += 4;
        } while (--longs);
    }
};

}; // namespace crypto

}; // namespace huys

#endif // HUYS_MD5_H
