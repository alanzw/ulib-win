#define _WIN32_IE 0x0400

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "uimagelist.h"

UImageList::UImageList( int nIconNum /*= 3*/,
                        int cx /*= GetSystemMetrics(SM_CXSMICON)*2*/,
                        int cy /*= GetSystemMetrics(SM_CXSMICON)*2*/ )
: m_nIconNum(nIconNum), m_cx(cx), m_cy(cy)
{
    // Ensure that the common control DLL is loaded.
	::InitCommonControls();
    //
    m_hImageList = ImageList_Create( m_cx, m_cy, ILC_MASK|ILC_COLOR32, m_nIconNum, 0 );
}

UImageList::UImageList( UINT nBitmapResID, HINSTANCE hInst )
: m_cx(16), m_cy(16)
{
    // Ensure that the common control DLL is loaded.
    InitCommonControls();
    //
    m_hImageList = ImageList_LoadBitmap(hInst,
                                        MAKEINTRESOURCE(nBitmapResID),
                                        m_cx,
                                        3,
                                        CLR_NONE);
    m_nIconNum = ImageList_GetImageCount(m_hImageList);
}

UImageList::~UImageList()
{
    if (m_hImageList)
    {
        ImageList_Destroy(m_hImageList);
    }
}

int UImageList::addIcon(UINT nResID, HINSTANCE hInst)
{
    HICON hIcon;
    //hIcon = LoadIcon(hInst, MAKEINTRESOURCE(nResID));
    hIcon = (HICON)LoadImage( hInst, // small class icon
                       MAKEINTRESOURCE(nResID),
                       IMAGE_ICON,
                       m_cx,
                       m_cy,
                       LR_DEFAULTCOLOR );

    return ImageList_AddIcon(m_hImageList, hIcon);
}

BOOL UImageList::drawIcon(int nIndex, HWND hWnd, int x, int y)
{
    HDC hdc;

    if ((hdc = GetDC(hWnd)) == NULL)
        return FALSE;

    if (!ImageList_Draw(m_hImageList, nIndex, hdc, x, y, ILD_TRANSPARENT))
        return FALSE;

    ReleaseDC(hWnd, hdc);

    return TRUE;
}

int UImageList::getIconNum() const
{
    return m_nIconNum;
}

HIMAGELIST UImageList::getHandle() const
{
    return m_hImageList;
}
/*
BOOL UImageList::drawIndirect( IMAGELISTDRAWPARAMS *pimldp )
{
	return ::ImageList_DrawIndirect(pimldp);
}
*/
int UImageList::addMasked( HBITMAP hBitmap, COLORREF clrMask )
{
	return ::ImageList_AddMasked(m_hImageList, hBitmap, clrMask);
}

BOOL UImageList::create( int cx, int cy, UINT uFlag, int nInitial, int nGrow )
{
	if (m_hImageList)
	{
		ImageList_Destroy(m_hImageList);
	}

	m_hImageList = ImageList_Create(cx, cy, uFlag, nInitial, nGrow);

	if (NULL == m_hImageList)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL UImageList::drawIndirect( HDC hdc, int nImage, LONG x, LONG y, LONG cx, LONG cy,
							  UINT fStyle /*= ILD_NORMAL*/, DWORD dwRop /*= SRCCOPY*/, 
							  COLORREF rgbBack /*= CLR_DEFAULT*/, 
							  COLORREF rgbFore /*= CLR_DEFAULT*/ )
{
	IMAGELISTDRAWPARAMS imldp = {0};
	imldp.cbSize = sizeof(IMAGELISTDRAWPARAMS);
	imldp.himl = m_hImageList;
	imldp.i = nImage;
	imldp.hdcDst = hdc;
	imldp.x = x;
	imldp.y = y;
	imldp.cx = cx;
	imldp.cy = cy;
	imldp.xBitmap = 0;
	imldp.yBitmap = 0;
	imldp.rgbBk = rgbBack;
	imldp.rgbFg = rgbFore;
	imldp.fStyle = fStyle;
	imldp.dwRop = dwRop;

	return ::ImageList_DrawIndirect(&imldp);
}

BOOL UImageList::destroy()
{
	if (m_hImageList)
	{
		ImageList_Destroy(m_hImageList);
	}
	return TRUE;
}

BOOL UImageList::writeToStream( LPSTREAM lpStream )
{
	return ImageList_Write(m_hImageList, lpStream);
}

BOOL UImageList::setIconSize( int cx, int cy )
{
	m_cx = cx;
	m_cy = cy;
	return ImageList_SetIconSize(m_hImageList, cx, cy);
}

BOOL UImageList::setImageCount( int nCount )
{
	m_nIconNum = nCount;
	return ImageList_SetImageCount(m_hImageList, nCount);
}

BOOL UImageList::readFromStream( LPSTREAM lpStream )
{
	return (NULL != (m_hImageList = ImageList_Read(lpStream)));
}


