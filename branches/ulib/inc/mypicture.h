/********************************************************************
 created:  2007/12/10
 filename: mypicture.h
 author:   hu
 purpose:  Define CPicture.
           Simplify the display of BMP.
*********************************************************************/

#ifndef MYPICTURE_H
#define MYPICTURE_H

namespace huys
{

class CPicture
{
public:
	CPicture();
	CPicture(CString);
	virtual ~CPicture();
	//
	BOOL LoadFromFile(CString sPath);
    BOOL LoadFromID(UINT nID);
    BOOL Show(CDC *pDC, CRect *DrawRect,
              double MagnifyX=1, double MagnifyY=1);
    //
    int getWidth() const;
    int getHeight() const;
protected:

private:
	CString   m_sPath;
	LONG      m_Height;
	LONG      m_Width;
    HBITMAP   m_hBitmap;
    CBitmap   m_bmp;
    BITMAP    m_bi;
    HBITMAP   m_oldbitmap;
};

}

#endif
