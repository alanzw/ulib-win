#ifndef HU_BITS_H
#define HU_BITS_H

#define BITS_PER_LONG 32

typedef unsigned long ulong;

inline long max0(long x)
{
    return x & ~(x >> (BITS_PER_LONG-1));
}

inline ulong average(ulong x, ulong y)
// Use the fact that x+y == ((x&y)<<1) + (x^y)
// that is: sum == carries + sum_without_carries
{
    return (x & y) + ((x ^ y) >> 1);
}

inline ulong bit_count(ulong x)
// return number of bits set
{
    x = (0x55555555 & x) + (0x55555555 & (x>> 1)); // 0-2 in 2 bits
    x = (0x33333333 & x) + (0x33333333 & (x>> 2)); // 0-4 in 4 bits
    x = (0x0f0f0f0f & x) + (0x0f0f0f0f & (x>> 4)); // 0-8 in 8 bits
    x = (0x00ff00ff & x) + (0x00ff00ff & (x>> 8)); // 0-16 in 16 bits
    x = (0x0000ffff & x) + (0x0000ffff & (x>>16)); // 0-31 in 32 bits
    return x;
}

inline ulong bit_count1(ulong x)
// return number of bits set
{
    x = ((x>>1) & 0x55555555) + (x & 0x55555555); // 0-2 in 2 bits
    x = ((x>>2) & 0x33333333) + (x & 0x33333333); // 0-4 in 4 bits
    x = ((x>>4) + x) & 0x0f0f0f0f; // 0-8 in 4 bits
    x += x>> 8; // 0-16 in 8 bits
    x += x>>16; // 0-32 in 8 bits
    return x & 0xff;
}

inline ulong bit_count2(ulong x)
// return number of bits set
{
    x -= (x>>1) & 0x55555555;
    x = ((x>>2) & 0x33333333) + (x & 0x33333333);
    x = ((x>>4) + x) & 0x0f0f0f0f;
    x *= 0x01010101;
    return x>>24;
}

#endif // HU_BITS_H

