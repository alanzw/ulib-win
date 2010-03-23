#ifndef U_LISTCTRL_H
#define U_LISTCTRL_H

// 
class UCellEdit : public CEdit
{
public:
	UCellEdit();
	virtual ~UCellEdit();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};


//
class UCellCombo : public CComboBox
{
public:
	UCellCombo();
	virtual ~UCellCombo();
protected:

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnKillFocus(CWnd* pNewWnd);
};


//
class UMixedListCtrl : public CListCtrl
{
public:
	UMixedListCtrl();
	virtual ~UMixedListCtrl();

	//
	typedef enum {NONE=0, EDITABLE, COMBOABLE} CellState;

	//
	LRESULT beginCellEdit();
	LRESULT endCellEdit();
	LRESULT beginCellCombo();
	LRESULT endCellCombo();

    LRESULT changeHeight(int height);

    BOOL updateItemText(const CString strText);

	//
	virtual BOOL PreTranslateMessage(MSG *pMsg);

    //
    void setEditable(int col);
protected:
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult);

private:
	UCellEdit *m_pCellEdit;
	UCellCombo *m_pCellCombo;

	int m_row;
	int m_col;

	CString m_strItemText;

    CList<int, int> lstEditCols;
    CList<int, int> lstCmboCols;

	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

    DWORD m_clickMsg;
};



#endif // U_LISTCTRL_H
