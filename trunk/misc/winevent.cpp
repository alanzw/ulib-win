
// public domain
// by Christopher Diggins 2004
// http://www.cdiggins.com 
//
// This program demonstrates a technique of static dispatch for event-driven programming 
// using template specialization. 

#include <windows.h>
#include "winevent.hpp"

#include <time.h>
#include <stdio.h>

int nInt = 0;
int nStart = 0;
int nEnd = 0;
char buf[255];

//////////////////////////////////////////////////////////////////////////////////////
// Event handlers 

template<>
LRESULT OnMsg<WM_CREATE>(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
  strcpy(buf, "Press any key to start test");
  return DefWindowProc(hWnd, WM_CREATE, wParam, lParam);
}

template<> 
LRESULT OnMsg<WM_KEYDOWN>(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
  nInt = 0;
  strcpy(buf, "running test, please wait ...");
  InvalidateRect(hWnd, NULL, true);
  SendMessage(hWnd, WM_PAINT, 0, 0);
  nStart = GetTickCount();
  PostMessage(hWnd, WM_TIMER, 0, 0);
  return DefWindowProc(hWnd, WM_KEYDOWN, wParam, lParam);
}

template<> 
LRESULT OnMsg<WM_TIMER>(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
  if (nInt++ < 100000) {
    PostMessage(hWnd, WM_TIMER, 0, 0);
  } else {
    nEnd = GetTickCount();
    sprintf(buf, "Time elapsed = %d msec", ((nEnd - nStart) * 1000) / CLOCKS_PER_SEC);
    InvalidateRect(hWnd, NULL, true);
	}
	return 0;
}

template<>
LRESULT OnMsg<WM_DESTROY>(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
  PostQuitMessage(0);
  return 0;
}

template<>
LRESULT OnMsg<WM_PAINT>(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;
  hdc = BeginPaint(hWnd, &ps);    
	TextOut(hdc, 100, 100, buf, static_cast<int>(strlen(buf)));
	EndPaint(hWnd, &ps);
	return DefWindowProc(hWnd, WM_PAINT, wParam, lParam);  
}

//////////////////////////////////////////////////////////////////////////////////////
// Entry point for the application. 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) 
{
  InitMsgHandlers<NULL>();
  
  static char szAppName[] = "demo";
  HWND        hwnd;
  MSG         msg;
  WNDCLASSEX  wndclass;

  wndclass.cbSize         = sizeof(wndclass);
  wndclass.style          = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc    = WndProc;
  wndclass.cbClsExtra     = 0;
  wndclass.cbWndExtra     = 0;
  wndclass.hInstance      = hInstance;
  wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wndclass.lpszClassName  = szAppName;
  wndclass.lpszMenuName   = NULL;
 
  RegisterClassEx(&wndclass);

  hwnd = CreateWindow(szAppName, "Static Dispatch Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
  
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while ( GetMessage(&msg, NULL, 0, 0) ) {
    TranslateMessage(&msg);   
	  DispatchMessage(&msg);    
  } 

  return static_cast<int>(msg.wParam);
}



