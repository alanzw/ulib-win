#ifndef U_PICTURE_H
#define U_PICTURE_H

#define HIMETRIC_INCH 2540
#define ERROR_TITLE "UPicture Error"

#include <ocidl.h>
#include <olectl.h>
#include <shlwapi.h>

#include "ulib.h"

class ULIB_API UPicture
{
public:
    UPicture();

    ~UPicture();

    BOOL load(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCSTR ResourceType);

    BOOL load(const TCHAR * sFilePathName);

    BOOL loadPictureData(BYTE *pBuffer, int nSize);


    BOOL show(HDC hDC, LPRECT pDrawRect, LPRECT pSrcRect = NULL);

    BOOL saveAsBitmap(const TCHAR *sFilePath);

    BOOL updateSizeOnDC(HDC hDC);

    BOOL createFromIcon(HICON hIcon);

    BOOL createFromBitmap(HBITMAP hBitmap);

    void unload();

    void getImageSize();

private:
    void freePictureData();

private:
    IPicture *m_pIPicture;

    OLE_XSIZE_HIMETRIC m_nHeight;
    OLE_XSIZE_HIMETRIC m_nWidth;
    int m_nWeight;
};

#endif // U_PICTURE_H
