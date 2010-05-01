#include <windows.h>
#include <tchar.h>

#include "upicture.h"
#include "ufile.h"

UPicture::UPicture()
: m_pIPicture(NULL),
m_nHeight(0),
m_nWidth(0),
m_nWeight(0)
{}

UPicture::~UPicture()
{
    freePictureData();
}

BOOL UPicture::load(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCSTR ResourceType)
{
    HGLOBAL hGlobal = NULL;
    HRSRC hSource = NULL;
    LPVOID lpVoid  = NULL;
    int nSize   = 0;
    BOOL bResult=FALSE;

    if(m_pIPicture != NULL)
    {
        freePictureData(); // Important - Avoid Leaks...
    }

    hSource = ::FindResource(hInstance, lpszResourceName, ResourceType);

    if(hSource == NULL)
    {
        MessageBoxEx(NULL, "FindResource() Failed\t", ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
        return FALSE;
    }

    hGlobal = ::LoadResource(hInstance, hSource);
    if(hGlobal == NULL)
    {
        MessageBoxEx(NULL, "LoadResource() Failed\t", ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
        return FALSE;
    }

    lpVoid = ::LockResource(hGlobal);
    if(lpVoid == NULL)
    {
        MessageBoxEx(NULL, "LockResource() Failed\t", ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
        return FALSE;
    }

    nSize = (UINT)SizeofResource(hInstance, hSource);
    if(loadPictureData((BYTE*)hGlobal, nSize))
    {
        bResult = TRUE;
    }

    //UnlockResource(hGlobal); // 16Bit Windows Needs This
    //::FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)

    m_nWeight = nSize; // Update Picture Size Info...

    getImageSize();

    return bResult;
}

BOOL UPicture::load(const TCHAR * sFilePathName)
{
    if(!PathFileExists(sFilePathName))
    {
        return FALSE;
    }
    BOOL bResult = FALSE;
    UFile PictureFile;
    //CFileException e;
    int nSize = 0;

    if(m_pIPicture != NULL) freePictureData(); // Important - Avoid Leaks...

    if(PictureFile.open(sFilePathName))
    {
        nSize = PictureFile.size();
        BYTE* pBuffer = new BYTE[nSize];
        DWORD dwRead;
        if(PictureFile.read(pBuffer, nSize, &dwRead))
        {
            if(loadPictureData(pBuffer, nSize)) bResult = TRUE;
        }

        PictureFile.close();
        delete [] pBuffer;
    }
    else // Open Failed...
    {
        //TCHAR szCause[255];
        //e.GetErrorMessage(szCause, 255, NULL);
        //MessageBoxEx(NULL, szCause, ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
        bResult = FALSE;
    }

    m_nWeight = nSize; // Update Picture Size Info...
    getImageSize();
    return bResult;
}

BOOL UPicture::loadPictureData(BYTE *pBuffer, int nSize)
{
    BOOL bResult = FALSE;
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);

    if(hGlobal == NULL)
    {
        MessageBoxEx(NULL, "Can not allocate enough memory\t", ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
        return FALSE;
    }

    void* pData = GlobalLock(hGlobal);
    memcpy(pData, pBuffer, nSize);
    GlobalUnlock(hGlobal);

    IStream* pStream = NULL;

    if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
    {
        HRESULT hr;
        if((hr = ::OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pIPicture)) == E_NOINTERFACE)
        {
            MessageBoxEx(NULL, "IPicture interface is not supported\t", ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
            return FALSE;
        }
        else // S_OK
        {
            pStream->Release();
            pStream = NULL;
            bResult = TRUE;
        }
    }

    FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)

    return bResult;
}


BOOL UPicture::show(HDC hDC, LPRECT pDrawRect, LPRECT pSrcRect)
{
    if (hDC == NULL || m_pIPicture == NULL) return FALSE;

    //RECT recrDest = *pDrawRect;

    long Width  = 0;
    long Height = 0;
    m_pIPicture->get_Width(&Width);
    m_pIPicture->get_Height(&Height);

    RECT SrcRect = { 0,0,Width,Height };

    if(pSrcRect)
    {
        SrcRect=*pSrcRect;
        // convert pixels to himetric
        SrcRect.bottom =MulDiv(SrcRect.bottom, HIMETRIC_INCH,GetDeviceCaps(hDC, LOGPIXELSX));
        SrcRect.left =MulDiv(SrcRect.left, HIMETRIC_INCH,GetDeviceCaps(hDC, LOGPIXELSX));
        SrcRect.right =MulDiv(SrcRect.right, HIMETRIC_INCH,GetDeviceCaps(hDC, LOGPIXELSX));
        SrcRect.top =MulDiv(SrcRect.top, HIMETRIC_INCH,GetDeviceCaps(hDC, LOGPIXELSX));
    }
    RECT DrawRect = *pDrawRect;
    //DrawRect.NormalizeRect();
    //SrcRect.NormalizeRect();
    HRESULT hrP = 0;

    hrP = m_pIPicture->Render(hDC,
    DrawRect.left,                  // Left
    DrawRect.top,                   // Top
    DrawRect.right - DrawRect.left, // Right
    DrawRect.bottom - DrawRect.top, // Bottom
    SrcRect.left,
    SrcRect.bottom,
    SrcRect.right - SrcRect.left,
    SrcRect.top - SrcRect.bottom,
    &DrawRect);

    if (SUCCEEDED(hrP)) return TRUE;
    //AfxThrowMemoryException();
    return FALSE;
}

BOOL UPicture::saveAsBitmap(const TCHAR *sFilePath)
{
    BOOL bResult = FALSE;
    ILockBytes *Buffer = 0;
    IStorage *pStorage = 0;
    IStream *FileStream = 0;
    BYTE *BufferBytes;
    STATSTG BytesStatistics;
    DWORD OutData;
    long OutStream;
    UFile BitmapFile;
    //CFileException e;
    double SkipFloat = 0;
    DWORD ByteSkip = 0;
    _ULARGE_INTEGER RealData;

    CreateILockBytesOnHGlobal(NULL, TRUE, &Buffer); // Create ILockBytes Buffer

    HRESULT hr = ::StgCreateDocfileOnILockBytes(Buffer,
    STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);

    hr = pStorage->CreateStream(L"PICTURE",
    STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &FileStream);

    m_pIPicture->SaveAsFile(FileStream, TRUE, &OutStream); // Copy Data Stream
    FileStream->Release();
    pStorage->Release();
    Buffer->Flush();

    // Get Statistics For Final Size Of Byte Array
    Buffer->Stat(&BytesStatistics, STATFLAG_NONAME);

    // Cut UnNeeded Data Coming From SaveAsFile() (Leave Only "Pure" Picture Data)
    SkipFloat = (double(OutStream) / 512); // Must Be In a 512 Blocks...
    if(SkipFloat > DWORD(SkipFloat)) ByteSkip = (DWORD)SkipFloat + 1;
    else ByteSkip = (DWORD)SkipFloat;
    ByteSkip = ByteSkip * 512; // Must Be In a 512 Blocks...

    // Find Difference Between The Two Values
    ByteSkip = (DWORD)(BytesStatistics.cbSize.QuadPart - ByteSkip);

    // Allocate Only The "Pure" Picture Data
    RealData.LowPart = 0;
    RealData.HighPart = 0;
    RealData.QuadPart = ByteSkip;
    BufferBytes = (BYTE*)malloc(OutStream);
    if(BufferBytes == NULL)
    {
        Buffer->Release();
        MessageBoxEx(NULL, "Can not allocate enough memory\t", ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
    }

    Buffer->ReadAt(RealData, BufferBytes, OutStream, &OutData);

    if(BitmapFile.open(sFilePath))
    {
        BitmapFile.write(BufferBytes, OutData);
        BitmapFile.close();
        bResult = TRUE;
    }
    else // Write File Failed...
    {
        //TCHAR szCause[255];
        //e.GetErrorMessage(szCause, 255, NULL);
        //MessageBoxEx(NULL, szCause, ERROR_TITLE, MB_OK | MB_ICONSTOP, LANG_ENGLISH);
        bResult = FALSE;
    }

    Buffer->Release();
    free(BufferBytes);

    return bResult;
}

BOOL UPicture::updateSizeOnDC(HDC hDC)
{
    if(hDC == NULL || m_pIPicture == NULL)
    {
        m_nHeight = 0;
        m_nWidth = 0;
        return FALSE;
    }

    m_pIPicture->get_Height(&m_nHeight);
    m_pIPicture->get_Width(&m_nWidth);

    // Get Current DPI - Dot Per Inch
    int CurrentDPI_X = ::GetDeviceCaps(hDC, LOGPIXELSX);
    int CurrentDPI_Y = ::GetDeviceCaps(hDC, LOGPIXELSY);

    // Use a "Standard" Print (When Printing)
    //if(pDC->IsPrinting())
    //{
    //    CurrentDPI_X = 96;
    //    CurrentDPI_Y = 96;
    //}

    m_nHeight = MulDiv(m_nHeight, CurrentDPI_Y, HIMETRIC_INCH);
    m_nWidth  = MulDiv(m_nWidth,  CurrentDPI_X, HIMETRIC_INCH);

    return(TRUE);
}

BOOL UPicture::createFromIcon(HICON hIcon)
{
    BOOL bResult=FALSE;
    if(hIcon)
    {
        PICTDESC desc;
        desc.cbSizeofstruct = sizeof(PICTDESC);
        desc.picType = PICTYPE_ICON;
        desc.icon.hicon= hIcon;
        m_nWeight = 0;
        if(FAILED(::OleCreatePictureIndirect(&desc,IID_IPicture,FALSE,(LPVOID*)&m_pIPicture)))
        {
            bResult=FALSE;
        }
        else
        {
            bResult=TRUE;
        }
    }
    getImageSize();
    return bResult;
}

BOOL UPicture::createFromBitmap(HBITMAP hBitmap)
{
    BOOL bResult=FALSE;
    if(hBitmap)
    {
        PICTDESC desc;
        desc.cbSizeofstruct = sizeof(PICTDESC);
        desc.picType = PICTYPE_BITMAP;
        desc.bmp.hbitmap = hBitmap;
        desc.bmp.hpal = 0;
        m_nWeight = 0;
        if(FAILED(::OleCreatePictureIndirect(&desc, IID_IPicture, FALSE, (LPVOID*)&m_pIPicture)))
        {
            bResult=FALSE;
        }
        else
        {
            bResult=TRUE;
        }
    }
    getImageSize();
    return bResult;
}

void UPicture::unload()
{
    freePictureData();
}

void UPicture::getImageSize()
{
    if(m_pIPicture != NULL)
    {   // Do Not Try To Read From Memory That Is Not Exist...
        m_pIPicture->get_Height(&m_nHeight);
        m_pIPicture->get_Width(&m_nWidth);
        // Calculate Its Size On a "Standard" (96 DPI) Device Context
        m_nHeight = MulDiv(m_nHeight, 96, HIMETRIC_INCH);
        m_nWidth  = MulDiv(m_nWidth,  96, HIMETRIC_INCH);
    }
    else
    {// Picture Data Is Not a Known Picture Type
        m_nHeight = 0;
        m_nWidth = 0;
        m_nWeight=0;
    }
}


void UPicture::freePictureData()
{
    if(m_pIPicture != NULL)
    {
        m_pIPicture->Release();
        m_pIPicture = NULL;
        m_nHeight = 0;
        m_nWeight = 0;
        m_nWidth = 0;
    }
}
