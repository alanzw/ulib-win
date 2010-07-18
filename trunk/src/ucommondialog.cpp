#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#include "ucommondialog.h"

namespace UCommonDialog
{

UColorDialog::UColorDialog(HWND hWnd, huys::Color clr )
: m_hWnd(hWnd), m_clr(clr)
{
    ::ZeroMemory(&m_cc, sizeof(m_cc));
    m_cc.lStructSize = sizeof(m_cc);
    m_cc.hwndOwner = m_hWnd;
    m_cc.lpCustColors = (LPDWORD) acrCustClr;
    m_cc.rgbResult = m_clr;
    m_cc.Flags = CC_FULLOPEN | CC_RGBINIT;
}

UColorDialog::~UColorDialog()
{}

BOOL UColorDialog::choose()
{
    return ::ChooseColor(&m_cc);
}

huys::Color UColorDialog::resultColor()
{
    return m_cc.rgbResult;
}

#ifdef UNICODE

extern "C"
{
	unsigned int CALLBACK FileDialogHook(HWND hwnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam)
	{
		static HWND hwndParentDialog;
		LPOFNOTIFY lpofn;
		int cbLength;
		static LPTSTR lpsz;
		static int LastLen;

		switch (uMsg)
		{
		case WM_INITDIALOG:
			// You need to use a copy of the OPENFILENAME struct used to
			// create this dialog. You can store a pointer to the
			// OPENFILENAME struct in the ofn.lCustData so you can
			// retrieve it here in the lParam. Once you have it, you
			// need to hang on to it. Using window properties provides a
			// good thread safe solution to using a global variable.

			if(!SetProp(GetParent(hwnd), "OFN", (void *) lParam))
				MessageBox(NULL, "SET PRop Failed", "ERROR", MB_OK);
			return (0);

		case WM_COMMAND:
			{
				OutputDebugString("command\n");
			}
			break;
		case WM_NOTIFY:
			// The OFNOTIFY struct is passed in the lParam of this
			// message.

			lpofn = (LPOFNOTIFY) lParam;

			switch (lpofn->hdr.code)
			{
			case CDN_SELCHANGE:

				LPOPENFILENAME lpofn;
				cbLength = CommDlg_OpenSave_GetSpec(GetParent(hwnd), NULL, 0);

				cbLength += _MAX_PATH;

				// The OFN struct is stored in a property of dialog window

				lpofn = (LPOPENFILENAME) GetProp(GetParent(hwnd),
					"OFN");

				if (lpofn->nMaxFile < cbLength)

				{
					// Free the previously allocated buffer.
					if(lpsz)
						HeapFree(GetProcessHeap(),
						0,
						lpsz);
					// Allocate a new buffer
					lpsz = (LPTSTR) HeapAlloc(GetProcessHeap(),
						HEAP_ZERO_MEMORY,
						cbLength);
					if (lpsz)
			  {

				  lpofn->lpstrFile = lpsz;
				  lpofn->nMaxFile  = cbLength;
			  }
				}
				break;

			}
			return (0);

		case WM_DESTROY:

			// Also need to free the property with the OPENFILENAME
			// struct.
			RemoveProp(GetParent(hwnd), "OFN");
			return (0);
		}
		return (0);
	}

	//WNDPROC* g_lpfnDialogProc = ;
}
#endif

UFileDialog::UFileDialog( HWND hWnd )
: sFilename(MAX_PATH)
{
	sFilename.reserve(20000);
    // Initialize OPENFILENAME
    ZeroMemory(&m_ofn, sizeof(m_ofn));
    m_ofn.lStructSize = sizeof(m_ofn);
    m_ofn.hwndOwner = hWnd;
    m_ofn.lpstrFile = sFilename;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
    // use the contents of szFile to initialize itself.
    m_ofn.lpstrFile[0] = '\0';
    m_ofn.nMaxFile = sFilename.buffer_size();
    m_ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    m_ofn.nFilterIndex = 1;
    m_ofn.lpstrFileTitle = NULL;
    m_ofn.nMaxFileTitle = 0;
    m_ofn.lpstrInitialDir = NULL;
	m_ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
#ifdef UNICODE
    m_ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST|OFN_ENABLEHOOK;
	m_ofn.lCustData = (LPARAM)&m_ofn;
	m_ofn.lpfnHook = &FileDialogHook;
#endif
}

UFileDialog::~UFileDialog()
{

}

BOOL UFileDialog::open()
{
    BOOL bRet = ::GetOpenFileName(&m_ofn);

   if (!bRet && FNERR_BUFFERTOOSMALL == CommDlgExtendedError())
   {
   //    WORD size = *((WORD *)m_ofn.lpstrFile);
  //      sFilename.reserve(size+20);
		//m_ofn.lpstrFile = sFilename;
		//m_ofn.lpstrFile[0] = '\0';
		//m_ofn.nMaxFile = sFilename.buffer_size();
  //     bRet = ::GetOpenFileName(&m_ofn);
  }

    return bRet;
}

void UFileDialog::setFilter(LPCTSTR lpFilter)
{
    m_ofn.lpstrFilter = lpFilter;
}

void UFileDialog::setFlags(DWORD dwFlags)
{
    m_ofn.Flags = dwFlags;
}

WORD UFileDialog::getFileOffset() const
{
    return m_ofn.nFileOffset;
}

UFontDialog::UFontDialog( HWND hwnd )
: m_hWnd(hwnd)
{
    // Initialize members of the CHOOSEFONT structure.
    m_cf.lStructSize = sizeof(CHOOSEFONT);
    m_cf.hwndOwner = (HWND)NULL;
    m_cf.hDC = (HDC)NULL;
    m_cf.lpLogFont = &m_lf;
    m_cf.iPointSize = 0;
    m_cf.Flags = CF_SCREENFONTS;
    m_cf.rgbColors = RGB(0,0,0);
    m_cf.lCustData = 0L;
    m_cf.lpfnHook = (LPCFHOOKPROC)NULL;
    m_cf.lpTemplateName = (LPSTR)NULL;
    m_cf.hInstance = (HINSTANCE) NULL;
    m_cf.lpszStyle = (LPSTR)NULL;
    m_cf.nFontType = SCREEN_FONTTYPE;
    m_cf.nSizeMin = 0;
    m_cf.nSizeMax = 0;
}

UFontDialog::~UFontDialog()
{

}

BOOL UFontDialog::choose()
{
    return ::ChooseFont(&m_cf);
}

UPrinterDialog::UPrinterDialog(HWND hwnd)
: m_hWnd(hwnd)
{
    // Initialize PRINTDLG
    ZeroMemory(&m_pd, sizeof(m_pd));
    m_pd.lStructSize = sizeof(m_pd);
    m_pd.hwndOwner   = m_hWnd;
    m_pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode.
    m_pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames.
    m_pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
    m_pd.nCopies     = 1;
    m_pd.nFromPage   = 0xFFFF;
    m_pd.nToPage     = 0xFFFF;
    m_pd.nMinPage    = 1;
    m_pd.nMaxPage    = 0xFFFF;
}

UPrinterDialog::~UPrinterDialog()
{
    // Delete DC when done.
    DeleteDC(m_pd.hDC);
}

BOOL UPrinterDialog::choose()
{
    return PrintDlg(&m_pd);
}

UFindReplaceDialog::UFindReplaceDialog( HWND hOwner )
{
    // Initialize FINDREPLACE
    ZeroMemory(&m_fr, sizeof(m_fr));
    m_fr.lStructSize = sizeof(m_fr);
    m_fr.hwndOwner = hOwner;
    m_fr.lpstrFindWhat = m_buffer;
    m_fr.wFindWhatLen = 1024;
    m_fr.Flags = 0;
}

BOOL UFindReplaceDialog::go( LPCTSTR lpText )
{
    lstrcpy(m_buffer, lpText);
    m_hDlg = FindText(&m_fr);
    return FALSE;
}



}; // namespace UCommonDialog

