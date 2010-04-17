#include <windows.h>
#include <tchar.h>

#include "ufile.h"
#include "adt/ustring.h"
#include "adt/uautoptr.h"
#include "ufilesplit.h"

typedef huys::ADT::UStringAnsi TString;

UFileSplit::UFileSplit()
{
}

UFileSplit::~UFileSplit()
{}

BOOL UFileSplit::split(LPCTSTR sFilename, DWORD dwEachSize, DWORD dwBufferSize, LPCTSTR sOutFile)
{
    DWORD dwFileSize = 0;
    DWORD dwCount = 0;
    UFile fin;
    UFile fout;

    int nIndex = 1;
    TString outfilename;

    if (!fin.open(sFilename))
    {
        return FALSE;
    }

    dwFileSize = fin.size();

    if (dwFileSize < dwEachSize)
    {
        return FALSE;
    }

    int num = dwFileSize / dwEachSize;

    if (dwFileSize%dwEachSize)
    {
        ++num;
    }

    if (!fout.create(outfilename.format("%s.%02d", sOutFile, nIndex)))
    {
        return FALSE;
    }

    LPBYTE pbuf = new BYTE[dwBufferSize];

    DWORD dwRead;
    while( fin.read(pbuf, dwBufferSize, &dwRead)  && nIndex <= num)
    {
        dwCount += dwRead;
        fout.write(pbuf, dwRead);

        if (dwEachSize <= dwCount+dwBufferSize || dwFileSize <= (nIndex-1) * dwEachSize + dwCount+dwBufferSize)
        {
            fin.read(pbuf, dwCount+dwBufferSize-dwEachSize, &dwRead);
            dwCount += dwRead;
            fout.write(pbuf, dwRead);
            ++nIndex;
            fout.close();

            if (nIndex <= num)
            {
                if (!fout.create(outfilename.format("%s.%02d", sOutFile, nIndex)))
                {
                    return FALSE;
                }
                dwCount = 0;
            }
        }

    }

    delete pbuf;

    return TRUE;
}

BOOL UFileSplit::merge(LPCTSTR *sFilenames, int nNum, DWORD dwBufferSize, LPCTSTR sOutFile)
{
    return TRUE;
}

