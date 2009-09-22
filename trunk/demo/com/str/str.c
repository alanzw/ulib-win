#include <windows.h>

int main()
{

    DWORD len;
    BSTR  strptr;
    char  MyString[] = "可爱的世界123";

    wchar_t title[30];

    // Get the length (in wide chars) of the UNICODE buffer we'll need to
    // convert MyString to UNICODE.
    len = MultiByteToWideChar(CP_ACP, 0, MyString, -1, 0, 0);

    // Allocate a UNICODE buffer of the needed length. SysAllocStringLen 
    // also allocate room for a terminating 0 short, and the unsigned lon
    // SysAllocStringLen will fill in the unsigned long with the value of
    // "len * sizeof(wchar_t)" and then return a pointer to the third sho
    // the buffer it allocates. All that is left for us to do is simply s
    // UNICODE version of our C string into this buffer.
    strptr = SysAllocStringLen(0, len);

    // Convert MyString to UNICODE in the buffer allocated by SysAllocStr
    MultiByteToWideChar(CP_ACP, 0, MyString, -1, strptr, len);

    // strptr is now a pointer (BSTR) to an Automation string datatype.

    wsprintfW(title, L"String Length: %d \n"
                     L"In bytes: %d",
                     SysStringLen(strptr),
                     SysStringByteLen(strptr));

    MessageBoxW(NULL, title, strptr, MB_OK);

    SysFreeString(strptr);

    return 0;
}

