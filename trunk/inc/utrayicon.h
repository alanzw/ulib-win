/*
 * =====================================================================================
 *
 *       Filename:  utrayicon.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011/5/29 20:20:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_TRAYICON_H
#define U_TRAYICON_H

#include "ulib.h"

class UTrayIconImpl;

#define UWM_SHELLNOTIFY  WM_USER+5


class ULIB_API UTrayIcon
{
public:
	UTrayIcon();

	~UTrayIcon();


	void setWnd(HWND hWnd);
	HWND getWnd() const;

	void setIcon(HICON hIcon);
	HICON getIcon() const;

	void setID(UINT id);
	UINT getID() const;

	void setTip(LPCTSTR szTip);
	LPCTSTR getTip() const; 

	void show();
	void hide();

	LRESULT handle_notify(UINT nMessage, WPARAM wParam, LPARAM lParam);

public:
	typedef LRESULT (*PF_Notify_Callback)(UINT nMessage, WPARAM wParam, LPARAM lParam);
private:
	UTrayIconImpl * m_ptii;
	PF_Notify_Callback m_pfnc_default;
};

#endif // U_TRAYICON_H

