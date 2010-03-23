#if !defined(AFX_DIGITALMETER_H__BB5D528C_8152_47FC_A2A8_B8A8FDC9739A__INCLUDED_)
#define AFX_DIGITALMETER_H__BB5D528C_8152_47FC_A2A8_B8A8FDC9739A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// digitalMeter.h : header file
//

//////////////////////////////////////////////////
// CDigitalMeter - static for Digital Meter
// Author: huys
// Updated: 20080217
// Usage:
//      Create a Static resource, change its 
//      symbol,eg "IDC_DIGITAL_METER", and 
//      bind it with a variable, eg "m_MyMeter".
//      Use the method display() to show your
//      number.
//
//////////////////////////////////////////////////


const DWORD STCOUNTER0   = 252;
const DWORD STCOUNTER1   = 96;
const DWORD STCOUNTER2   = 218;
const DWORD STCOUNTER3   = 242;
const DWORD STCOUNTER4   = 102;
const DWORD STCOUNTER5   = 182;
const DWORD STCOUNTER6   = 190;
const DWORD STCOUNTER7   = 224;
const DWORD STCOUNTER8   = 254;
const DWORD STCOUNTER9   = 246;
const DWORD STCOUNTERALL = 999;

const DWORD NOTCH1 = 128;
const DWORD NOTCH2 = 64;
const DWORD NOTCH3 = 32;
const DWORD NOTCH4 = 16;
const DWORD NOTCH5 = 8;
const DWORD NOTCH6 = 4;
const DWORD NOTCH7 = 2;
const DWORD NOTCH8 = 1;

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//				 - 14/7/99 Added optional clip rect parameter [jgh]
//
// This class implements a memory Device Context

class CMemDC : public CDC {
private:
    CBitmap m_bitmap; // Offscreen bitmap
    CBitmap* m_oldBitmap; // bitmap originally found in CMemDC
    CDC* m_pDC; // Saves CDC passed in constructor
    CRect m_rect; // Rectangle of drawing area.
    BOOL m_bMemDC; // TRUE if CDC really is a Memory DC.
public:
    CMemDC(CDC* pDC, CRect rect = CRect(0,0,0,0)) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
    {
        ASSERT(m_pDC != NULL); // If you asserted here, you passed in a NULL CDC.
        
        m_bMemDC = !pDC->IsPrinting();
        
        if (m_bMemDC){
            // Create a Memory DC
            CreateCompatibleDC(pDC);
            if ( rect == CRect(0,0,0,0) )
                pDC->GetClipBox(&m_rect);
            else
                m_rect = rect;
            
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_oldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        } else {
            // Make a copy of the relevent parts of the current DC for printing
            m_bPrinting = pDC->m_bPrinting;
            m_hDC = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    
    ~CMemDC()
    {
        if (m_bMemDC) {
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                this, m_rect.left, m_rect.top, SRCCOPY);
            //Swap back the original bitmap.
            SelectObject(m_oldBitmap);
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.
            m_hDC = m_hAttribDC = NULL;
        }
    }
    
    // Allow usage as a pointer
    CMemDC* operator->() {return this;}
    
    // Allow usage as a pointer
    operator CMemDC*() {return this;}
};

/////////////////////////////////////////////////////////////////////////////
// digitalMeter window

class CDigitalMeter : public CStatic
{
// Construction
public:
	CDigitalMeter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(digitalMeter)
	//}}AFX_VIRTUAL

// Implementation
public:
    CString m_strNumber;
    void SetDrawFaded(bool bState = true);
    void Display( int nNumber );
    void SetColorForeGround(COLORREF crColor = 0xffffffff);
	void SetColorBackGround(COLORREF crColor = 0xffffffff);
    virtual ~CDigitalMeter();

protected:
    void Draw( CMemDC* pDC, DWORD dwChar, int nCol);
    bool m_bDrawFadedNotches;
    bool m_bGotMetrics;
    
    RECT m_recClient;
    int m_nNotchWidth;
    int m_nNotchLength;
    int m_nMargin;
    
    COLORREF m_crColorBackground;
    COLORREF m_crColorForeground;
    COLORREF m_crColorDimForeground;
    CBrush m_brBackground;
	CBrush m_brForeground;
	// Generated message map functions
protected:
	//{{AFX_MSG(digitalMeter)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALMETER_H__BB5D528C_8152_47FC_A2A8_B8A8FDC9739A__INCLUDED_)
