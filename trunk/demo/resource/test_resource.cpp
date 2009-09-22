#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

void ErrorHandler(LPCTSTR lpMsg)
{
    exit(1);
}


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s file1.exe file2.exe\n", argv[0]);
        return 0;
    }


    HRSRC hResLoad;     // handle to loaded resource
    HANDLE hExe;        // handle to existing .EXE file
    HRSRC hRes;         // handle/ptr. to res. info. in hExe
    HANDLE hUpdateRes;  // update resource handle
    char *lpResLock;    // pointer to resource data
    BOOL result;

    // Load the .EXE file that contains the dialog box you want to copy.
    hExe = LoadLibrary("hand.exe");
    if (hExe == NULL)
    {
        ErrorHandler("Could not load exe.");
    }

    // Locate the dialog box resource in the .EXE file.
    hRes = FindResource((HINSTANCE)hExe, "AboutBox", RT_DIALOG);
    if (hRes == NULL)
    {
        ErrorHandler("Could not locate dialog box.");
    }

    // Load the dialog box into global memory.
    hResLoad = (HRSRC)LoadResource((HINSTANCE)hExe, hRes);
    if (hResLoad == NULL)
    {
        ErrorHandler("Could not load dialog box.");
    }

    // Lock the dialog box into global memory.
    lpResLock = (char *)LockResource(hResLoad);
    if (lpResLock == NULL)
    {
        ErrorHandler("Could not lock dialog box.");
    }

    // Open the file to which you want to add the dialog box resource.
    hUpdateRes = BeginUpdateResource("foot.exe", FALSE);
    if (hUpdateRes == NULL)
    {
        ErrorHandler("Could not open file for writing.");
    }

    // Add the dialog box resource to the update list.
    result = UpdateResource(hUpdateRes,       // update resource handle
            RT_DIALOG,                   // change dialog box resource
            "AboutBox",                  // dialog box name
            MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),  // neutral language
            lpResLock,                   // ptr to resource info
            SizeofResource((HINSTANCE)hExe, hRes)); // size of resource info.

    if (result == FALSE)
    {
        ErrorHandler("Could not add resource.");
    }

    // Write changes to FOOT.EXE and then close it.
    if (!EndUpdateResource(hUpdateRes, FALSE))
    {
        ErrorHandler("Could not write changes to file.");
    }

    // Clean up.
    if (!FreeLibrary((HINSTANCE)hExe))
    {
        ErrorHandler("Could not free executable.");
    }

    return 0;

}

