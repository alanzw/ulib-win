#include <cstring>
#include <cstdio>

#include "crypto/crc32.h"

namespace huys
{

namespace crypto
{


void CRC32::init_crc32_table(unsigned long * crc32_table)
{
    /* Call this function only once to initialize the CRC table. */
    /* This is the official polynomial used by CRC-32*/
    /* in PKZip, WinZip and Ethernet.*/
    unsigned long ulPolynomial = 0x04c11db7;
    int i, j;

    /* 256 values representing ASCII character codes.*/
    for( i = 0; i <= 0xFF; i++)
    {
        crc32_table[i] = crc_reflect(i, 8) << 24;
        for ( j = 0; j < 8; j++)
            crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
        crc32_table[i] = crc_reflect(crc32_table[i], 32);
    }
}

unsigned long CRC32::getCRC(FILE *fp)
{
    unsigned long crc32_table[256], globalCRC;
    char lsFileCRC[10];
    int i;
    unsigned char ch;
    int liFileLen;

    init_crc32_table(crc32_table) ;

    /*start calculation*/
    globalCRC = 0xffffffff;
    fseek(fp, 0L, SEEK_END);
    liFileLen = ftell(fp);
    rewind(fp);
    for (i=0; i < liFileLen ; i++ )
    {
        fread(&ch,1,1,fp);
        crc_addText(crc32_table,&globalCRC,(unsigned char *)&ch , sizeof ch);
    }
    /*end calculation*/
    globalCRC ^= 0xffffffff;
    return globalCRC;
}

unsigned long CRC32::getCRC(const char *filename)
{
    FILE * pf = fopen(filename, "r");
    if (0 == pf)
    {
        return 0;
    }
    unsigned long globalCRC = 0;
    globalCRC = getCRC(pf);
    fclose(pf);
    return globalCRC;
}

unsigned long CRC32::crc_reflect(unsigned long ref, unsigned char ch)
{
    /* Used only by Init_CRC32_Table().*/
    unsigned long value=0;
    int i;

    /* Swap bit 0 for bit 7*/
    /* bit 1 for bit 6, etc. */
    for( i = 1; i < (ch + 1); i++)
    {
        if(ref & 1)  value |= 1 << (ch - i);
        ref >>= 1;
    }
    return value; 
}

void CRC32::crc_addText(unsigned long *crc32_table, unsigned long *pCRC, unsigned char *text, int len)
{
    unsigned char * buffer;
    buffer = text;
    while (len-->0)
    {
        *pCRC = (*pCRC >> 8) ^ crc32_table[(*pCRC & 0xFF) ^ *buffer++];
    }
}

}; // namespace crypto

}; // namespace huys

