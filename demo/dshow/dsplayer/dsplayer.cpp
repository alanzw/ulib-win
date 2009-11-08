#include "dsplayer.h"
#include "resource.h"
#include "playerclass.h"

/********* GLOBAL VARIABLES **********/
HINSTANCE g_hInst;
HWND g_hDialogWindow;

// pointer to my PlayerClass obejct
PlayerClass *g_PlayerObject = NULL;

/******** FUNCTION DECLARATIONS ******/
BOOL CALLBACK DlgDSPlayerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	HICON iconLarge = NULL;

	InitCommonControls();

	g_hInst = hInstance;

	g_hDialogWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DLGDSPLAYER), NULL, (DLGPROC)DlgDSPlayerProc);

	// this will set the icon for my player
	iconLarge = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONLARGE));

	if (iconLarge)
	{
		SendMessage(g_hDialogWindow, WM_SETICON, ICON_BIG, (LPARAM)iconLarge);
	}


	// Initialize the COM library
	CoInitialize(NULL);


	if (!g_hDialogWindow)
	{
		MessageBox(NULL, "Dialog creation failed! Aborting..", "Error", MB_OK);
		return -1;
	}

	ShowWindow(g_hDialogWindow, nCmdShow);
	UpdateWindow(g_hDialogWindow);

	if (g_PlayerObject == NULL)
	{
		// create the player object
		g_PlayerObject = new PlayerClass();

		if (g_PlayerObject)
		{
			g_PlayerObject->Initialise(g_hDialogWindow);
		}
		else
		{
			MessageBox(NULL, "Error creating player object", "Error", MB_OK);
			return -1;
		}
	}

	// standard message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(g_hDialogWindow, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	return msg.wParam;
}



BOOL CALLBACK DlgDSPlayerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;

	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			switch ( LOWORD(wParam))
			{
				case IDC_OPENFILE:
					handled = true;
					///SetWindowText(GetDlgItem(hDlg, IDC_NOWPLAYING), "You selected Play File..");
					g_PlayerObject->OpenFileDialog();
					break;

				case IDC_PLAYPAUSE:
					handled = true;
					//SetWindowText(GetDlgItem(hDlg, IDC_NOWPLAYING), "You selected Pause");
					g_PlayerObject->DoPlayPause();
					break;

				case IDC_STOP:
					handled = true;
					//SetWindowText(GetDlgItem(hDlg, IDC_NOWPLAYING), "You selected Stop");
					g_PlayerObject->DoStop();
					break;

				case IDC_EXIT:
					handled = true;
					free(g_PlayerObject);
					EndDialog(hDlg, LOWORD(wParam));
					PostQuitMessage(0);
					break;
					//handled = true;
			}

		case WM_TIMER:
			g_PlayerObject->DoTimerStuff();
			break;

		case WM_CLOSE:
			//MessageBox(NULL, "got close", "info", MB_OK);
			PostQuitMessage(0);
			break;

		case WM_GRAPHNOTIFY:
			handled = true;
			g_PlayerObject->EventReceiver();
			break;




/*		case WM_CLOSE:
			CleanUp(hDlg);
			handled = true;
			EndDialog(hDlg, LOWORD(wParam));
			break;
*/
	}

    return handled;

}