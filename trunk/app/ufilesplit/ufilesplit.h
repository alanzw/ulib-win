#ifndef U_FILESPLIT_H
#define U_FILESPLIT_H

#include "adt/ustring.h"

typedef huys::ADT::UStringAnsi TString;

class UFileSplit
{
public:
    UFileSplit();
    ~UFileSplit();

    static BOOL split(LPCTSTR sFilename, DWORD dwEachSize, DWORD dwBufferSize,
               LPCTSTR sOutFile);

    static BOOL merge(LPCTSTR *sFilenames, int nNum, DWORD dwBufferSize,
               LPCTSTR sOutFile);

private:

};

#endif // U_FILESPLIT_H
