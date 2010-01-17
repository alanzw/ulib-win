#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <ocidl.h>
#include <olectl.h>

#include "ubasewindow.h"
#include "uwinapp.h"

#include "ufile.h"

BOOL save2file(LPCSTR szFileName, IPicture *pPic)
{
    // Create ILockBytes Buffer
    ILockBytes *plkbyt = NULL;
    CreateILockBytesOnHGlobal(NULL, TRUE, &plkbyt);

    // Create IStorage
    IStorage *pStorage = NULL;
    HRESULT hr = ::StgCreateDocfileOnILockBytes(plkbyt,
        STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);

    if (FAILED(hr))
    {
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }

    // Create IStream
    IStream *pStream = NULL;
    hr = pStorage->CreateStream(L"PICTURE",
        STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &pStream);

    if (FAILED(hr))
    {
        pStorage->Release();
        pStorage = NULL;
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }

    // Copy Data Stream
    long  lSize;
    hr = pPic->SaveAsFile(pStream, TRUE, &lSize);

    if (FAILED(hr))
    {
        pStream->Release();
        pStream = NULL;
        pStorage->Release();
        pStorage = NULL;
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }

    // Get Statistics For Final Size Of Byte Array
    STATSTG  statStg;
    hr = plkbyt->Stat(&statStg, STATFLAG_NONAME);

    if (FAILED(hr))
    {
        pStream->Release();
        pStream = NULL;
        pStorage->Release();
        pStorage = NULL;
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }

    // calculate "Pure" Picture Data, Must Be In a 512 Blocks...
    double  dbSkipFloat = (double(lSize) / 512);
    DWORD  dwPicDataSize = 0;
    if(dbSkipFloat > DWORD(dbSkipFloat))
    {
        dwPicDataSize = (DWORD)dbSkipFloat + 1;
    }
    else
    {
        dwPicDataSize = (DWORD)dbSkipFloat;
    }
    dwPicDataSize = dwPicDataSize * 512;
    // Allocate Only The "Pure" Picture Data
    BYTE  *pPicDataBuffer = (BYTE*)malloc(dwPicDataSize);

    if (pPicDataBuffer == NULL)
    {
        pStream->Release();
        pStream = NULL;
        plkbyt->Release();
        pStorage->Release();
        pStorage = NULL;
        plkbyt = NULL;
        return FALSE;
    }

    // Read "Pure" Picture Data to Buffer
    _ULARGE_INTEGER ulOffset;
    ulOffset.LowPart = 0;
    ulOffset.HighPart = 0;
    ulOffset.QuadPart = (DWORD)(statStg.cbSize.QuadPart - dwPicDataSize);
    DWORD dwRealDataSize;
    hr = plkbyt->ReadAt(ulOffset, pPicDataBuffer, dwPicDataSize, &dwRealDataSize);

    if (FAILED(hr))
    {
        free(pPicDataBuffer);
        pPicDataBuffer = NULL;
        pStream->Release();
        pStream = NULL;
        pStorage->Release();
        pStorage = NULL;
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }

    // Save "Pure" Picture Data to file
#if 0
    CFile  fBmp;
    CFileException e;
    if (!fBmp.Open(szFileName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite, &e))
    {
        free(pPicDataBuffer);
        pPicDataBuffer = NULL;
        pStream->Release();
        pStream = NULL;
        pStorage->Release();
        pStorage = NULL;
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }
    fBmp.Write(pPicDataBuffer, dwRealDataSize);
    fBmp.Close();
#endif

    UFile fBmp;
    if (!fBmp.create(szFileName))
    {
        free(pPicDataBuffer);
        pPicDataBuffer = NULL;
        pStream->Release();
        pStream = NULL;
        pStorage->Release();
        pStorage = NULL;
        plkbyt->Release();
        plkbyt = NULL;
        return FALSE;
    }
    fBmp.write(pPicDataBuffer, dwRealDataSize);
    fBmp.close();

    free(pPicDataBuffer);
    pPicDataBuffer = NULL;
    pStream->Release();
    pStream = NULL;
    pStorage->Release();
    pStorage = NULL;
    plkbyt->Release();
    plkbyt = NULL;
    return TRUE;
}


class UIPicWindow : public UBaseWindow
{
public:
    UIPicWindow()
    : UBaseWindow(NULL, ::GetModuleHandle(NULL)),
    m_pIPic(0),
    m_lWidth(0), m_lHeight(0)
    {
        this->setTitle(_T("IPicture Test 0.0.1"));
        this->setPos(100, 100, 800, 600);
    }

    ~UIPicWindow()
    {
        if ( m_pIPic )
        {
            m_pIPic->Release();
            m_pIPic = 0;
        }
    }

    BOOL onCreate()
    {
        this->setIconBig(IDI_APP);

        if (!loadFile("back.jpg"))
        {
            return FALSE;
        }

        m_pIPic->get_Width(&m_lWidth);
        m_pIPic->get_Height(&m_lHeight);

        return UBaseWindow::onCreate();
    }

    //
    virtual void onDraw(HDC hdc)
    {
        RECT rc = {0};
        this->getClientRect(&rc);

        if (0 != m_pIPic)
        {
            m_pIPic->Render(hdc, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
            0, m_lHeight, m_lWidth, -m_lHeight, NULL);
        }
    }

    BOOL onChar(WPARAM wParam, LPARAM lParam)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            return UBaseWindow::onClose();
        default:
            return UBaseWindow::onChar(wParam, lParam);
        }
    }
private:
    IPicture *m_pIPic;

    BOOL loadFile(const char *sFilename)
    {
        bool bResult = false;

        if ( m_pIPic != NULL)
        {
            m_pIPic->Release();
            m_pIPic = NULL;
        }

        UFile ufile;
        if (ufile.open(sFilename))
        {
            long length = ufile.size();

            HGLOBAL hGlobal = GlobalAlloc( GMEM_MOVEABLE, length );
            void* p = GlobalLock(hGlobal);

            DWORD dwRead;
            ufile.read((LPBYTE)p, length, &dwRead);
            ufile.close();

            GlobalUnlock(hGlobal);
            IStream* pStream = NULL;
            if ( CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
            {
                HRESULT hr = ::OleLoadPicture( pStream,
                length,
                FALSE,
                IID_IPicture,
                (LPVOID *) &m_pIPic);
                pStream->Release();
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL loadFileUrl(const char *szFileUrl)
    {
        OleLoadPicturePath((WCHAR *)szFileUrl,
            (LPUNKNOWN)NULL,
            0,
            0,
            IID_IPicture,
            (LPVOID*)&m_pIPic);

        return TRUE;
    }

    BOOL saveFile(const char *filename, int nWidth, int nHeight)
    {
        // create a new IPicture
        OLE_HANDLE hPic = NULL;
        if (FAILED(m_pIPic->get_Handle(&hPic)))
        {
            return FALSE;
        }
        HBITMAP hBmp = (HBITMAP)CopyImage((HANDLE)hPic,
        IMAGE_BITMAP,
        nWidth,
        nWidth,
        LR_CREATEDIBSECTION);
        if (hBmp == NULL)
        {
            return FALSE;
        }
        PICTDESC picDesc;
        picDesc.cbSizeofstruct = sizeof(PICTDESC);
        picDesc.picType = PICTYPE_BITMAP;
        picDesc.bmp.hbitmap = hBmp;

        IPicture *pNewPic = NULL;
        if (SUCCEEDED(OleCreatePictureIndirect(&picDesc, IID_IPicture, FALSE, (LPVOID *)&pNewPic)))
        {
            // Save to file
            save2file(filename, pNewPic);
            pNewPic->Release();
            pNewPic = NULL;
            DeleteObject(hBmp);
            hBmp = NULL;
            return TRUE;
        }
        DeleteObject(hBmp);
        hBmp = NULL;
        return FALSE;
        return TRUE;
    }

    long m_lWidth;
    long m_lHeight;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    UWinApp app;
    app.setMainWindow(new UIPicWindow);
    app.init(hInstance);
    return app.run();
}
