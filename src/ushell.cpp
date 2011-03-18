#define WINVER 0x0400

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <shlobj.h>

/// For Vista and later versions
#if (WINVER >= 0x0600)
  #include <shobjidl.h>
#endif // (WINVER >= 0x0600)

#include "ushell.h"

namespace UShell
{

#if (WINVER >= 0x0600)
HRESULT SimpleInvoke(HWND hwnd)
{
    IFileDialog *pfd;

    // CoCreate the dialog object.
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&pfd));

    if (SUCCEEDED(hr))
    {
        // Show the dialog
        hr = pfd->Show(hwnd);

        if (SUCCEEDED(hr))
        {
            // Obtain the result of the user's interaction with the dialog.
            IShellItem *psiResult;
            hr = pfd->GetResult(&psiResult);

            if (SUCCEEDED(hr))
            {
                // Do something with the result.
                psiResult->Release();
            }
        }
        pfd->Release();
    }
    return hr;
}
#endif // (WINVER >= 0x0600)

int execute( HWND hwnd,
             LPCTSTR lpFile,
             LPCTSTR lpParameters /* = NULL */,
             LPCTSTR lpDirectory /* = NULL */,
             INT nShowCmd /* = SW_SHOWNORMAL */
           )
{
    return (int)::ShellExecute(hwnd, _T("open"), lpFile, lpParameters, lpDirectory, nShowCmd);
}

int open(LPCTSTR url)
{
    return (int)::ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_NORMAL);
}

BOOL executeEx( HWND hwnd,
                         LPCTSTR lpFile,
                         LPCTSTR lpParameters /* = NULL */,
                         LPCTSTR lpDirectory /* = NULL */,
                         INT nShowCmd /* = SW_SHOWNORMAL */
                       )
{
    SHELLEXECUTEINFO sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = hwnd;
    sei.lpVerb = _T("open");
    sei.lpFile = lpFile;
    sei.lpParameters = lpParameters;
    sei.lpDirectory = lpDirectory;
    sei.nShow = nShowCmd;
    return ::ShellExecuteEx(&sei);
}

int msgbox(HWND hWnd, LPCTSTR pszMsg, LPCTSTR pszTitle, UINT fuStyle)
{
#if _WIN32_IE >= 0x0601
    return ::ShellMessageBox(NULL, hWnd, pszMsg, pszTitle, fuStyle);
#else
    return ::MessageBox(hWnd, pszMsg, pszTitle, fuStyle);
#endif // _WIN32_IE
}

namespace Path
{

void stripPath(LPTSTR sPath)
{
    ::PathStripPath(sPath);
}

LPTSTR addBackslash(LPTSTR sPath)
{
    return ::PathAddBackslash(sPath);
}

LPTSTR removeBackslash(LPTSTR sPath)
{
    return ::PathRemoveBackslash(sPath);
}

BOOL addExtension(LPTSTR sPath, LPCTSTR sExtension)
{
    return ::PathAddExtension(sPath, sExtension);
}

void removeExtension(LPTSTR sPath)
{
    ::PathRemoveExtension(sPath);
}

BOOL createLink(LPSTR szPath,LPSTR szLink)
{
    ::CoInitialize(NULL);

    HRESULT hres;
    IShellLink* psl;
    IPersistFile* ppf;
    WCHAR wsz[MAX_PATH];

    hres = ::CoCreateInstance(CLSID_ShellLink,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IShellLink,
        (void**)&psl);
    if(FAILED(hres))
        return FALSE;

    psl->SetPath(szPath);

    hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
    if(FAILED(hres))
        return FALSE;

    ::MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);

    hres = ppf->Save(wsz, STGM_READWRITE);

    ppf->Release();
    psl->Release();

    ::CoUninitialize();
    return TRUE;
}

BOOL getDesktopPath(LPTSTR sPath)
{
    LPITEMIDLIST ppidl;
    SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &ppidl);
    SHGetPathFromIDList(ppidl, sPath);

    return TRUE;
}

}; // namespace Path

}; // namespace UShell
