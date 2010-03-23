#ifndef U_LINK_H
#define U_LINK_H

class CHyperlink : public CString {
public:
    CHyperlink(LPCTSTR lpLink = NULL) : CString(lpLink) { }
    ~CHyperlink() { }
    const CHyperlink& operator=(LPCTSTR lpsz) {
        CString::operator=(lpsz);
        return *this;
    }
    operator LPCTSTR() {
        return CString::operator LPCTSTR(); 
    }
    virtual HINSTANCE Navigate() {
        return IsEmpty() ? NULL :
            ShellExecute(0, _T("open"), *this, 0, 0, SW_SHOWNORMAL);
    }
};


class ULink : public CStatic {
public:
	DECLARE_DYNAMIC(ULink)
	ULink(LPCTSTR lpText = NULL, BOOL bDeleteOnDestroy=FALSE);
	~ULink() { }

	// Hyperlink contains URL/filename. If NULL, I will use the window text.
	// (GetWindowText) to get the target.
	CHyperlink	m_link;
	COLORREF	m_color;

	// Default colors you can change
	// These are global, so they're the same for all links.
	static COLORREF g_colorUnvisited;
	static COLORREF g_colorVisited;
	static COLORREF g_colorOver;

	// Cursor used when mouse is on a link--you can set, or
	// it will default to the standard hand with pointing finger.
	// This is global, so it's the same for all links.
	static HCURSOR	 g_hCursorLink;

protected:
	CFont			m_font;				// underline font for text control
	BOOL			m_bDeleteOnDestroy;	// delete object when window destroyed?
    BOOL            m_bOverControl;     // cursor over control?
    UINT			m_nTimerID;

	virtual void PostNcDestroy();

	// message handlers
	DECLARE_MESSAGE_MAP()
	afx_msg UINT	OnNcHitTest(CPoint point);
	afx_msg HBRUSH  CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void	OnTimer( UINT nIDEvent );
};

#endif // U_LINK_H
