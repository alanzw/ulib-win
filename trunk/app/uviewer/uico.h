#ifndef U_ICO_H
#define U_ICO_H

typedef struct
{
    BYTE        bWidth;          // Width, in pixels, of the image
    BYTE        bHeight;         // Height, in pixels, of the image
    BYTE        bColorCount;     // Number of colors in image (0 if >=8bpp)
    BYTE        bReserved;       // Reserved ( must be 0)
    WORD        wPlanes;         // Color Planes
    WORD        wBitCount;       // Bits per pixel
    DWORD       dwBytesInRes;    // How many bytes in this resource?
    DWORD       dwImageOffset;   // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct
{
    WORD           idReserved;   // Reserved (must be 0)
    WORD           idType;       // Resource Type (1 for icons)
    WORD           idCount;      // How many images?
    ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;

typedef struct
{
    UINT            Width, Height, Colors; // Width, Height and bpp
    LPBYTE            lpBits;                // ptr to DIB bits
    DWORD            dwNumBytes;            // how many bytes?
    LPBITMAPINFO    lpbi;                  // ptr to header
    LPBYTE            lpXOR;                 // ptr to XOR image bits
    LPBYTE            lpAND;                 // ptr to AND image bits
} ICONIMAGE, *LPICONIMAGE;

typedef struct
{
    BOOL        bHasChanged;                     // Has image changed?
    TCHAR        szOriginalICOFileName[MAX_PATH]; // Original name
    TCHAR        szOriginalDLLFileName[MAX_PATH]; // Original name
    UINT        nNumImages;                      // How many images?
    ICONIMAGE    IconImages[1];                   // Image entries
} ICONRESOURCE, *LPICONRESOURCE;

#pragma pack( push )
#pragma pack( 2 )

typedef struct
{
   BYTE   bWidth;               // Width, in pixels, of the image
   BYTE   bHeight;              // Height, in pixels, of the image
   BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
   BYTE   bReserved;            // Reserved
   WORD   wPlanes;              // Color Planes
   WORD   wBitCount;            // Bits per pixel
   DWORD   dwBytesInRes;         // how many bytes in this resource?
   WORD   nID;                  // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct
{
   WORD            idReserved;   // Reserved (must be 0)
   WORD            idType;       // Resource type (1 for icons)
   WORD            idCount;      // How many images?
   GRPICONDIRENTRY   idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;

typedef struct
{
    BYTE    bWidth;               // Width of the image
    BYTE    bHeight;              // Height of the image (times 2)
    BYTE    bColorCount;          // Number of colors in image (0 if >=8bpp)
    BYTE    bReserved;            // Reserved
    WORD    wPlanes;              // Color Planes
    WORD    wBitCount;            // Bits per pixel
    DWORD    dwBytesInRes;         // how many bytes in this resource?
    WORD    nID;                  // the ID
} MEMICONDIRENTRY, *LPMEMICONDIRENTRY;
typedef struct 
{
    WORD            idReserved;   // Reserved
    WORD            idType;       // resource type (1 for icons)
    WORD            idCount;      // how many images?
    MEMICONDIRENTRY    idEntries[1]; // the entries for each image
} MEMICONDIR, *LPMEMICONDIR;

#pragma pack( pop )

LPICONRESOURCE ReadIconFromICOFile( LPCTSTR szFileName );

BOOL WriteIconToICOFile( LPICONRESOURCE lpIR, LPCTSTR szFileName );

HICON MakeIconFromResource( LPICONIMAGE lpIcon );

LPICONRESOURCE ReadIconFromEXEFile( LPCTSTR szFileName );

BOOL IconImageToClipBoard( LPICONIMAGE lpii );

BOOL IconImageFromClipBoard( LPICONIMAGE lpii, BOOL bStretchToFit );

BOOL CreateBlankNewFormatIcon( LPICONIMAGE lpii );

BOOL DrawXORMask( HDC hDC, RECT Rect, LPICONIMAGE lpIcon );

BOOL DrawANDMask( HDC hDC, RECT Rect, LPICONIMAGE lpIcon );

RECT GetXORImageRect( RECT Rect, LPICONIMAGE lpIcon );

BOOL MakeNewANDMaskBasedOnPoint( LPICONIMAGE lpIcon, POINT pt );

BOOL IconImageFromBMPFile( LPCTSTR szFileName, LPICONIMAGE lpii, BOOL bStretchToFit );

BOOL IconImageToBMPFile( LPCTSTR szFileName, LPICONIMAGE lpii );


#endif // U_ICO_H
