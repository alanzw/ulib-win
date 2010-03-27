#ifndef U_DX_TECPLOT_H
#define U_DX_TECPLOT_H

/***********************************************/
/* Primary Component of ASCII data file        */
/***********************************************/
/*==============================================
File  Header
Zone Record
Text Record
Geometry Record
Custom Labels Record
Data Set Auxiliary Data Record
Variable Auxiliary Data Record
===============================================*/
enum UTecFileType {
    UTFT_FULL,
    UTFL_GRID,
    UTFL_SOLUTION
};
struct UTecFileHeader {
    char title[512];
    UTecFileType type;
    char **variables;
    int number;
};

struct UZoneHeader {
};

struct UZoneRecord {
    UZoneHeader header;
    union {
        double * pDataDouble;
        float * pDataFloat;
        long * pDataLong;
        short * pDataShort;
        unsigned char * pDataByte;
        bool *pDataBit;
    };
    int type;
};

/***********************************************/
/*          a sample file:                     */
/***********************************************/
/*==============================================
TITLE="Simple Data File"
VARIABLES="X" "Y"
ZONE I=4 DATAPACKING=POINT
1 1
2 1
2 2
1 2
TEXT X=10 Y=90 T="Simple Text"
================================================*/

class UTecFile
{
    enum Limit {
        MAX_ZONE_RECORD = 32700,
        MAX_CUSTOM_LABEL_RECORD = 10,
        MAX_CHARS_PER_LINE = 32000
    };
public:
    UTecFile()
    {}

    ~UTecFile()
    {}


    bool write(const char * filename)
    {


        return true;
    }
private:
    UTecFileHeader _header;
};


#endif // U_DX_TECPLOT_H
