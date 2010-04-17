#ifndef U_FILESPLIT_H
#define U_FILESPLIT_H

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
