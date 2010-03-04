#ifndef HUYS_CRC32_H
#define HUYS_CRC32_H

#include "ulib.h"

namespace huys
{

namespace crypto
{

class ULIB_API CRC32
{
public:
    unsigned long getCRC(const char *filename);
    unsigned long getCRC(FILE *fp);
private:
    unsigned long crc_reflect(unsigned long ref, unsigned char ch); 
    void crc_addText(unsigned long *crc32_table, unsigned long *pCRC, unsigned char *text, int len); 
    void init_crc32_table(unsigned long * crc32_table);
private:
    unsigned long * _crc32_table;
};

}; // namespace crypto

}; // namespace huys


#endif // HUYS_CRC32_H

