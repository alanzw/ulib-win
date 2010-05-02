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

    if (dwFileSize%dwEachSize != 0)
    {
        ++num;
    }

    if (!fout.create(outfilename.format("%s.%02d", sOutFile, nIndex)))
    {
        return FALSE;
    }

    LPBYTE pbuf = new BYTE[dwBufferSize];

    DWORD dwRead;
    while( fin.read(pbuf, dwBufferSize, &dwRead)  && dwRead > 0 && nIndex <= num)
    {
        dwCount += dwRead;
        fout.write(pbuf, dwRead);

        //
        if (dwEachSize <= dwCount+dwBufferSize)
        {
            fin.read(pbuf, dwEachSize-dwCount, &dwRead);
            dwCount += dwRead;
            fout.write(pbuf, dwRead);
            fout.close();

            ++nIndex;
            if (nIndex <= num)
            {
                if (!fout.create(outfilename.format("%s.%02d", sOutFile, nIndex)))
                {
                    return FALSE;
                }
                dwCount = 0;
            }
        }

        //
        if (dwFileSize <= (nIndex-1) * dwEachSize + dwCount+dwBufferSize)
        {
            fin.read(pbuf, dwFileSize - dwCount - (nIndex-1) *dwEachSize, &dwRead);
            dwCount += dwRead;
            fout.write(pbuf, dwRead);
            fout.close();
            break;
        }

    }

    fin.close();

    delete [] pbuf;

    return TRUE;
}

BOOL UFileSplit::merge(LPCTSTR *sFilenames, int nNum, DWORD dwBufferSize, LPCTSTR sOutFile)
{
    UFile fin;
    UFile fout;
    DWORD dwCount = 0;

    if (!fout.create(sOutFile))
    {
        return FALSE;
    }

    LPBYTE pbuf = new BYTE[dwBufferSize];

    DWORD dwRead;

    int i = 0;
    for (i=0; i<nNum; ++i)
    {
        if (!fin.open(sFilenames[i]))
        {
            break;
        }

        while (fin.read(pbuf, dwBufferSize, &dwRead) && dwRead > 0)
        {
            dwCount += dwRead;
            fout.write(pbuf, dwRead);
        }
        fin.close();
    }

    fout.close();

    delete [] pbuf;

    return TRUE;
}
