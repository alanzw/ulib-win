#ifndef HUYS_BASE64_H
#define HUYS_BASE64_H

#include "ulib.h"

namespace huys
{

namespace crypto
{

class ULIB_API BASE64
{
public:
    char* encode(const char* data, int len, char* dest);
    char* decode(const char* data, int len, char* dest);
private:
    void encodeblock( unsigned char in[3], unsigned char out[4], int len );
    void decodeblock( unsigned char in[4], unsigned char out[3] );
};

}; // namespace crypto

}; // namespace huys

#endif // HUYS_BASE64_H
