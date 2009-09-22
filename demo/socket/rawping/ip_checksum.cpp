/***********************************************************************
 ip_checksum.cpp - Calculates IP-style checksums on a block of data.
***********************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

USHORT ip_checksum(USHORT* buffer, int size) 
{
    unsigned long cksum = 0;
    
    // Sum all the words together, adding the final byte if size is odd
    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) {
        cksum += *(UCHAR*)buffer;
    }

    // Do a little shuffling
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    
    // Return the bitwise complement of the resulting mishmash
    return (USHORT)(~cksum);
}
