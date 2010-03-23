#include "StdAfx.h"

#pragma comment(lib, "winmm.lib")

#include <mmsystem.h>
#include "uSplash.h"

BEGIN_MESSAGE_MAP(USplash, CWnd)
  ON_WM_PAINT()
END_MESSAGE_MAP()

USplash::USplash(UINT nResID, const TCHAR *szSoundFile/*=NULL*/)
{
	//m_Bitmap.LoadBitmap(MAKEINTRESOURCE(nResID)); //Load Bitmap
	//m_Bitmap.GetBitmap(&bmBitmap);         //Get Bitmap Info
	m_pic.LoadFromID(nResID);
	
	/*Show Splash Window and Play SplashWindow.wav*/
	if (NULL != szSoundFile)
		::PlaySound(szSoundFile, NULL, SND_ASYNC | SND_FILENAME);
}

USplash::USplash( const TCHAR *szPath )
{
	CString str = szPath;
	m_pic.LoadFromFile(str);
}
USplash::~USplash()
{
}

void USplash::CreateSplash()
{
	//Create Splash Window
	CreateEx(WS_EX_TOPMOST,
		     AfxRegisterWndClass(
			 0,
			 AfxGetApp()->LoadStandardCursor(IDC_UPARROW)),
			 "SplashWindow Sample",
			 WS_POPUP|WS_VISIBLE,
			 0,
			 0,
			 m_pic.getWidth(),  //Bitmap Width = Splash Window Width
			 m_pic.getHeight(), //Bitmap Height = Splash Window Height
			 NULL,
			 NULL,
			 NULL);
}

void USplash::OnPaint()
{
	CPaintDC dc(this);

	CRect rc(0, 0, m_pic.getWidth(), m_pic.getHeight());
	m_pic.Show(&dc, &rc);
}

void USplash::show(LONG ms/*=3000*/)
{
	this->CreateSplash();
	this->CenterWindow();
	this->UpdateWindow();
	this->ShowWindow(SW_SHOW);
	Sleep(ms);
	this->DestroyWindow();	
}
