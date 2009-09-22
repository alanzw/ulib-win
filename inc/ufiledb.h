#ifndef U_FILEDB_H
#define U_FILEDB_H

#include "ulib.h"

class ULIB_API UFileDB
{
public:
    typedef struct tagData {
        /// data name
        char name[10];
        /// data description
        char description[20];
    } DBData;

    typedef struct tagRecord {
        /// data index
        int index;
        DBData data;
        tagRecord *prior;
        tagRecord *next;
    } DBRecord;
public:
    UFileDB(const char *sFilename = "default.bin");
    ~UFileDB();

    bool read();
    bool add(const DBData &aData);
    bool change(const DBData &aData);
    bool save();
    bool saveAs(const char *sNewFilename);
protected:
private:
    TCHAR m_sFilename[MAX_PATH];

    DBRecord *m_pHead;
    DBRecord *m_pEnd;
    DBRecord *m_pRecord;
};

#endif // U_FILEDB_H

