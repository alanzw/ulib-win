// implement of CPicture

#include "stdafx.h"
#include "mypicture.h"

namespace huys
{

CPicture::CPicture()
:m_Width(0), m_Height(0), m_hBitmap(NULL)
{
    m_sPath = "";
};

CPicture::CPicture(CString sPath)
:m_Width(0), m_Height(0), m_hBitmap(NULL)
{
	m_sPath = sPath;
    this->LoadFromFile(sPath);
};

CPicture::~CPicture()
{
};

BOOL CPicture::LoadFromID(UINT nID)
{
    m_bmp.LoadBitmap(nID);
    m_bmp.GetBitmap(&m_bi);
    //
    this->m_Width = m_bi.bmWidth;
    this->m_Height = m_bi.bmHeight;
    //
    return TRUE;
}

BOOL CPicture::LoadFromFile(CString sPath)
{
	m_hBitmap = (HBITMAP)LoadImage(
        NULL, 
        sPath, 
        IMAGE_BITMAP, 
 		0, 
        0, 
        LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
    //
    ASSERT(m_hBitmap);
    //
    m_bmp.Attach(m_hBitmap);
    //
    m_bmp.GetBitmap(&m_bi);
    //
    this->m_Width = m_bi.bmWidth;
    this->m_Height = m_bi.bmHeight;
    //
    return TRUE;
};

BOOL CPicture::Show(CDC *pDC, CRect *DrawRect, 
        double MagnifyX, double MagnifyY)
{  
    CDC MemDC;
    MemDC.CreateCompatibleDC(pDC);
    m_oldbitmap=(HBITMAP)MemDC.SelectObject(m_bmp);
    pDC->StretchBlt(DrawRect->left, 
                    DrawRect->top,
                    (int)(DrawRect->Width()*MagnifyX), 
                    (int)(DrawRect->Height()*MagnifyY), 
                    &MemDC, 
                    0, 
                    0, 
                    m_Width, 
                    m_Height, 
                    SRCCOPY);
    MemDC.SelectObject(m_oldbitmap);
    return TRUE;
};

int CPicture::getWidth() const
{
    return m_Width;
}

int CPicture::getHeight() const 
{
    return m_Height;
}


};
