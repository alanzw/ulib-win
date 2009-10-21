#include <windows.h>
#include <tchar.h>

#include "ufile.h"
#include "umsg.h"

#include "memstream.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
    {
        IMemStream strm;

        strm << "This is ";
        strm << "an example ";
        strm << "of a multibyte ";
        strm << "string builder ";
        strm << "using IMemStream";
        // NULL terminate
        strm << char(0);

        // now get the pointer to the stream buffer, casting it to the correct type

        PSTR pszString = (PTSTR) strm.GetBuf();
        
        ::MessageBoxA( NULL, pszString, "info", MB_OK|MB_ICONINFORMATION );
    }
    {
        IMemStream strm;

        strm << L"This is ";
        strm << L"an example ";
        strm << L"of a Unicode ";
        strm << L"string builder ";
        strm << L"using IMemStream";
        // NULL terminate
        strm << WCHAR(0);

        // now get the pointer to the stream buffer, casting it to the correct type

        PWSTR pszString = (PWSTR) strm.GetBuf();
        
        MessageBoxW( NULL, pszString, L"info", MB_OK|MB_ICONINFORMATION  );
    }
    {
        IMemStream strm;
        char szFilename[300];
        ::GetModuleFileName( NULL, szFilename, sizeof(szFilename) );

        UFile file;
        file.open(szFilename);

        BYTE buf[500];
        DWORD dwRead;

        while( file.read( buf, sizeof(buf), &dwRead) && 0 != dwRead )
        {
            strm.Write( buf, dwRead, NULL );
        }
        
        showMsgFormat("info", "Size of this .exe file: %.2f K", strm.GetEndPos()/1024.0);
    }
    return 0;
}
