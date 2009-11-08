#include "resource.h"

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <iostream>

#include "adt/utable.h"

#include "uguievent.h"

typedef huys::ADT::UTable<UGUIEvent, EventHandler> EventTable;

EventTable g_evtable;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UGUIEvent e(message);
    if (g_evtable.inTable(e))
    {
        g_evtable[e].handle(e);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void exitApp()
{
    PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow) 
{
    g_evtable.add(UGUIEvent(UGUIEvent::CloseEvt), EventHandler(&exitApp));
    
    static char szAppName[] = "demo";
    static char szWinName[] = "UGUIEvent";
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

    hwnd = CreateWindow(
        szAppName,
        szWinName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while ( GetMessage(&msg, NULL, 0, 0) ) {
        TranslateMessage(&msg);   
        DispatchMessage(&msg);    
    } 

    return static_cast<int>(msg.wParam);
}

