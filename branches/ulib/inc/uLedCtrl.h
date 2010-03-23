#if !defined(AFX_STATELEDCTRL_H__B94C77A2_7C5D_423F_B890_22BF64B0C82B__INCLUDED_)
#define AFX_STATELEDCTRL_H__B94C77A2_7C5D_423F_B890_22BF64B0C82B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StateLedCtrl.h : header file
//
//Productor:qigao
//date"2005-09-20
//use:can used to describe the state of switch
//note:you can use it anywhere ,but I hope not to business way



// The constants needed to define the shape of the LED
#define ID_SHAPE_ROUND	3001
#define ID_SHAPE_SQUARE	3002

typedef enum tagItemCOLOR{RED, GREEN, BLUE, YELLOW};
/////////////////////////////////////////////////////////////////////////////
// CStateLedCtrl window

class ULedCtrl : public CStatic
{
// Construction
public:
	ULedCtrl();

// Attributes
public:

// Operations
public:

protected:	
	// The pens and brushes needed to do the drawing
	CPen m_PenBright,m_PenDark;
	CBrush m_BrushBright,m_BrushDark,m_BrushCurrent;
	tagItemCOLOR  m_itemColor;
	
	// This variable is used to store the shape and color
	// set by the user for resetting the led later 
	UINT m_nShape;
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStateLedCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	tagItemCOLOR GetSwitchState();
	void SetSwitchState(tagItemCOLOR itemState);
	void SetLED(UINT nIDShape);
	virtual ~ULedCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStateLedCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATELEDCTRL_H__B94C77A2_7C5D_423F_B890_22BF64B0C82B__INCLUDED_)
