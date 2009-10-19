#include <windows.h>
#include <tchar.h>

#include "memstream.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    IMemStream strm;

    strm << "This is ";
    strm << "an example ";
    strm << "of a multibyte ";
    strm << "string builder ";
    strm << "using CStream";
    // NULL terminate
    strm << char(0);

    // now get the pointer to the stream buffer, casting it to the correct type

    PSTR pszString = (PTSTR) strm.GetBuf();
    
    ::MessageBoxA( NULL, pszString, "info", MB_OK|MB_ICONINFORMATION );
    
    {
        IMemStream strm;

        strm << L"This is ";
        strm << L"an example ";
        strm << L"of a Unicode ";
        strm << L"string builder ";
        strm << L"using CStream";
        // NULL terminate
        strm << WCHAR(0);

        // now get the pointer to the stream buffer, casting it to the correct type

        PWSTR pszString = (PWSTR) strm.GetBuf();
        
        MessageBoxW( NULL, pszString, L"", 0 );
    }
    return 0;
}
