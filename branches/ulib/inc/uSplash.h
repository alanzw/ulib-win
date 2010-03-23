#ifndef HU_SPLASH_H
#define HU_SPLASH_H

#include "mypicture.h"

class USplash : public CWnd
{
private:
//	CDC MemDC;
//	BITMAP bmBitmap;
//	CBitmap m_Bitmap;
//	CBitmap *Old_Bitmap;
	huys::CPicture m_pic;

public:
	USplash(UINT nResID, const TCHAR *szSoundFile = NULL);
	USplash(const TCHAR *szPath);
	~USplash();
	void show(LONG ms = 3000);
protected:
	void CreateSplash();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif // HU_SPLASH_H
