#ifndef U_IMAGELIST_H
#define U_IMAGELIST_H

#include "ulib.h"

class ULIB_API UImageList
{
public:
    UImageList( int nIconNum = 3,
                int cx = GetSystemMetrics(SM_CXSMICON)*2,
                int cy = GetSystemMetrics(SM_CXSMICON)*2 );
    UImageList( UINT nBitmapResID,
                HINSTANCE hInst );

    virtual ~UImageList();

    BOOL destroy();

    //
    BOOL create(int cx, int cy, UINT uFlag, int nInitial, int nGrow);

    //
    operator HIMAGELIST ()
    {  return m_hImageList; }
    //
    int addIcon(UINT nResID, HINSTANCE hInst);
    //
    BOOL drawIcon(int nIndex, HWND hwnd, int x, int y);
    //
    BOOL writeToStream(LPSTREAM lpStream);
    BOOL readFromStream(LPSTREAM lpStream);
    //
    int getIconNum() const;
    //
    HIMAGELIST getHandle() const;

    //
    //BOOL drawIndirect(IMAGELISTDRAWPARAMS *pimldp);
    BOOL drawIndirect(HDC hdc, int nImage, LONG x, LONG y, LONG cx, LONG cy,
        UINT fStyle = ILD_NORMAL,
        DWORD dwRop = SRCCOPY,
        COLORREF rgbBack = CLR_DEFAULT,
        COLORREF rgbFore = CLR_DEFAULT);
    //
    int addMasked(HBITMAP hBitmap, COLORREF clrMask);

    //
    BOOL setIconSize(int cx, int cy);
    BOOL setImageCount(int nCount);

protected:
    HIMAGELIST m_hImageList;
    int m_nIconNum;
    int m_cx;
    int m_cy;
private:
};

#endif // U_IMAGELIST_H

