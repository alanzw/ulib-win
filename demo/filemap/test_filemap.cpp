/*
   This program demonstrates file mapping, especially how to align a
   view with the system file allocation granularity.
*/

#include <windows.h>
#include <stdio.h>

#define BUFFSIZE 1024 // size of the memory to examine at any one time

#define FILE_MAP_START 138240 // starting point within the file of
                              // the data to examine (135K)

/* The test file. The code below creates the file and populates it,
   so there is no need to supply it in advance. */

TCHAR * lpcTheFile = TEXT("fmtest.txt"); // the file to be manipulated

int main(void)
{
  HANDLE hMapFile;      // handle for the file's memory-mapped region
  HANDLE hFile;         // the file handle
  BOOL bFlag;           // a result holder
  DWORD dBytesWritten;  // number of bytes written
  DWORD dwFileSize;     // temporary storage for file sizes
  DWORD dwFileMapSize;  // size of the file mapping
  DWORD dwMapViewSize;  // the size of the view
  DWORD dwFileMapStart; // where to start the file map view
  DWORD dwSysGran;      // system allocation granularity
  SYSTEM_INFO SysInfo;  // system information; used to get granularity
  LPVOID lpMapAddress;  // pointer to the base address of the
                        // memory-mapped region
  char * pData;         // pointer to the data
  int i;                // loop counter
  int iData;            // on success contains the first int of data
  int iViewDelta;       // the offset into the view where the data
                        //shows up

  // Create the test file. Open it "Create Always" to overwrite any
  // existing file. The data is re-created below
  hFile = CreateFile(lpcTheFile,
                     GENERIC_READ | GENERIC_WRITE,
                     0,
                     NULL,
                     CREATE_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    printf("hFile is NULL\n");
    printf("Target file is %s\n",
           lpcTheFile);
    return 4;
  }

  // Get the system allocation granularity.
  GetSystemInfo(&SysInfo);
  dwSysGran = SysInfo.dwAllocationGranularity;

  // Now calculate a few variables. Calculate the file offsets as
  // 64-bit values, and then get the low-order 32 bits for the
  // function calls.

  // To calculate where to start the file mapping, round down the
  // offset of the data into the file to the nearest multiple of the
  // system allocation granularity.
  dwFileMapStart = (FILE_MAP_START / dwSysGran) * dwSysGran;
  printf ("The file map view starts at %ld bytes into the file.\n",
          dwFileMapStart);

  // Calculate the size of the file mapping view.
  dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFSIZE;
  printf ("The file map view is %ld bytes large.\n",
          dwMapViewSize);

  // How large will the file mapping object be?
  dwFileMapSize = FILE_MAP_START + BUFFSIZE;
  printf ("The file mapping object is %ld bytes large.\n",
          dwFileMapSize);

  // The data of interest isn't at the beginning of the
  // view, so determine how far into the view to set the pointer.
  iViewDelta = FILE_MAP_START - dwFileMapStart;
  printf ("The data is %d bytes into the view.\n",
          iViewDelta);

  // Now write a file with data suitable for experimentation. This
  // provides unique int (4-byte) offsets in the file for easy visual
  // inspection. Note that this code does not check for storage
  // medium overflow or other errors, which production code should
  // do. Because an int is 4 bytes, the value at the pointer to the
  // data should be one quarter of the desired offset into the file

  for (i=0; i<(int)dwSysGran; i++)
  {
    WriteFile (hFile, &i, sizeof (i), &dBytesWritten, NULL);
  }

  // Verify that the correct file size was written.
  dwFileSize = GetFileSize(hFile,  NULL);
  printf("hFile size: %10d\n", dwFileSize);

  // Create a file mapping object for the file
  // Note that it is a good idea to ensure the file size is not zero
  hMapFile = CreateFileMapping( hFile,          // current file handle
                NULL,           // default security
                PAGE_READWRITE, // read/write permission
                0,              // size of mapping object, high
                dwFileMapSize,  // size of mapping object, low
                NULL);          // name of mapping object

  if (hMapFile == NULL)
  {
    printf("hMapFile is NULL: last error: %d\n", GetLastError() );
    return (2);
  }

  // Map the view and test the results.

  lpMapAddress = MapViewOfFile(hMapFile,            // handle to
                                                    // mapping object
                               FILE_MAP_ALL_ACCESS, // read/write
                               0,                   // high-order 32
                                                    // bits of file
                                                    // offset
                               dwFileMapStart,      // low-order 32
                                                    // bits of file
                                                    // offset
                               dwMapViewSize);      // number of bytes
                                                    // to map
  if (lpMapAddress == NULL)
  {
    printf("lpMapAddress is NULL: last error: %d\n", GetLastError());
    return 3;
  }

  // Calculate the pointer to the data.
  pData = (char *) lpMapAddress + iViewDelta;

  // Extract the data, an int. Cast the pointer pData from a "pointer
  // to char" to a "pointer to int" to get the whole thing
  iData = *(int *)pData;

  printf ("The value at the pointer is %d,\n"
           "which %s one quarter of the desired file offset.\n",
          iData,
          iData*4 == FILE_MAP_START ? "is" : "is not");

  // Close the file mapping object and the open file

  bFlag = UnmapViewOfFile(lpMapAddress);
  bFlag = CloseHandle(hMapFile); // close the file mapping object

  if(!bFlag)
  {
    printf("\nError %ld occurred closing the mapping object!",
           GetLastError());
  }

  bFlag = CloseHandle(hFile);   // close the file itself

  if(!bFlag)
  {
    printf("\nError %ld occurred closing the file!",
           GetLastError());
  }

  return 0;
}
