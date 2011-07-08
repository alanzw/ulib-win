#include <windows.h>
#include <tchar.h>

#include <assert.h>

#include "utrayicon.h"

#include "umsg.h"

#ifndef UASSERT
#define UASSERT assert
#endif

class UTrayIconImpl
{
public:
	enum {
		ID_DEFAULT = -1
	};
public:
	UTrayIconImpl()
	{
		m_nid.cbSize = sizeof(NOTIFYICONDATA);
		m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_nid.uCallbackMessage = UWM_SHELLNOTIFY;
		m_nid.uID = ID_DEFAULT;
	}

	void setWnd(HWND hWnd)
	{
		m_nid.hWnd = hWnd;
	}

	HWND getWnd() const
	{
		return m_nid.hWnd;
	}

	void setIcon(HICON hIcon)
	{
		m_nid.hIcon = hIcon;
	}

	HICON getIcon() const
	{
		return m_nid.hIcon;
	}
	
	void setID(UINT id)
	{
		m_nid.uID = id;
	}

	UINT getID() const
	{
		return m_nid.uID;
	}

	void setTip( LPCTSTR szTip )
	{
		lstrcpy(m_nid.szTip, szTip);
	}
	
	LPCTSTR getTip() const
	{
		return m_nid.szTip;	
	}

	BOOL show()
	{
		return Shell_NotifyIcon(NIM_ADD, &m_nid);
	}

	BOOL hide()
	{
		return Shell_NotifyIcon(NIM_DELETE, &m_nid);
	}
private:
	NOTIFYICONDATA m_nid;
};

LRESULT Default_Notify_Callback(UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	UASSERT(nMessage == UWM_SHELLNOTIFY);

	UASSERT(wParam == UTrayIconImpl::ID_DEFAULT);

	if (lParam==WM_RBUTTONDOWN)
	{
		showMsg("Right Button Down!");
	}
	else if (lParam==WM_LBUTTONDBLCLK)
	{
		showMsg("Left Button Down!");
    }

	return 0;
}

UTrayIcon::UTrayIcon() 
{
	m_ptii = new UTrayIconImpl;
	m_pfnc_default = &Default_Notify_Callback;
}

UTrayIcon::~UTrayIcon()
{
	CHECK_PTR(m_ptii);
}

void UTrayIcon::setWnd( HWND hWnd )
{
	m_ptii->setWnd(hWnd);
}

HWND UTrayIcon::getWnd() const
{
	return m_ptii->getWnd();
}

void UTrayIcon::setIcon( HICON hIcon )
{
	m_ptii->setIcon(hIcon);
}

HICON UTrayIcon::getIcon() const
{
	return m_ptii->getIcon();
}

void UTrayIcon::setID( UINT id )
{
	m_ptii->setID(id);
}

UINT UTrayIcon::getID() const
{
	return m_ptii->getID();
}

void UTrayIcon::setTip( LPCTSTR szTip )
{
	m_ptii->setTip(szTip);
}

LPCTSTR UTrayIcon::getTip() const
{
	return m_ptii->getTip();	
}

void UTrayIcon::show()
{
	UASSERT(m_ptii->show());
}

void UTrayIcon::hide()
{
	UASSERT(m_ptii->hide());
}

LRESULT UTrayIcon::handle_notify( UINT nMessage, WPARAM wParam, LPARAM lParam )
{
	return m_pfnc_default(nMessage, wParam, lParam);	
}



