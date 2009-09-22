//---------------------------------------------------------
// Name: Example.cpp
// Desc: Simple program that demonstrates usage of my
//       CWindowDock window docking system.
//---------------------------------------------------------
// Copyright © 2005 Nick Bloor
//---------------------------------------------------------

//-----------------
// Includes
//-----------------
#include <windows.h>
#include "CWindowDock.h"

//-----------------
// Globals
//-----------------
CWindowDock gDock; //The window docking system
HWND gParent = 0; //A window to act as the docking parent
HWND gChild1 = 0; //A window to act as one docking child
HWND gChild2 = 0; //A window to act as a second docking child
/*
NOTE: gChild1 and gChild2 are not created as child windows to gParent, I simply use the term
      child to describe the relationship in the docking system - Child windows dock with the
      Parent window.
*/

//-----------------
// Function Prototypes
//-----------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-----------------
// Functions
//-----------------

//---------------------------------------------------------
// Name: WindowProc()
// Desc: The window message procedure.
//---------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //Process messages
    switch(msg)
    {
    case WM_MOVE:
        //Notify the docking system of a window moving
        gDock.WindowMoved(hwnd);
        return 0;
        break;

    case WM_DESTROY:
        //Post a quit message
        PostQuitMessage(0);
        return 0;
        break;

    default:
        //Pass the message on to windows for processing
        return DefWindowProc(hwnd, msg, wParam, lParam);
        break;
    }
}

//---------------------------------------------------------
// Name: WinMain()
// Desc: Application entry point. Creates several windows
//       and adds them to the docking system.
//---------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEX wc;
    RECT rc;
    MSG msg;

    //-----------------
    // Create the windows
    //-----------------
    //Setup the window class
    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = 0;
    wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hIcon = LoadIcon(GetModuleHandle(0), IDI_APPLICATION);
    wc.hIconSm = LoadIcon(GetModuleHandle(0), IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(0);
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = "DOCKINGWINDOW";
    wc.lpszMenuName = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    //Register the window class
    if(!RegisterClassEx(&wc))
    {
        //Error
        return 0;
    }

    //Create the parent window - 300*300 and centered on the screen
    gParent = CreateWindowEx(0, "DOCKINGWINDOW", "CWindowDock Demo - Parent", WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE, (GetSystemMetrics(SM_CXSCREEN) / 2) - 150, (GetSystemMetrics(SM_CYSCREEN) / 2) - 150, 300, 300, 0, 0, hInstance, 0);
    if(gParent == 0)
    {
        //Error
        return 0;
    }

    //Get the parent window rectangle for positioning the child windows
    GetWindowRect(gParent, &rc);

    //Create the child windows
    gChild1 = CreateWindowEx(0, "DOCKINGWINDOW", "CWindowDock Demo - Child 1", WS_OVERLAPPED | WS_VISIBLE, rc.right, rc.top, 150, 150, gParent, 0, hInstance, 0);
    if(gChild1 == 0)
    {
        //Error
        return 0;
    }
    gChild2 = CreateWindowEx(0, "DOCKINGWINDOW", "CWindowDock Demo - Child 2", WS_OVERLAPPED | WS_VISIBLE, rc.right, rc.top + 150, 150, 150, gParent, 0, hInstance, 0);
    if(gChild2 == 0)
    {
        //Error
        return 0;
    }

    //-----------------
    // Add the windows to the docking system
    //-----------------
    //Set the parent window
    gDock.SetParent(gParent);

    //Add the child windows
    gDock.AddChild(gChild1); //gChild1 will automatically dock
    gDock.AddChild(gChild2, false); //gChild2 will not automatically dock

    //-----------------
    // The message loop
    //-----------------
    while(GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //Return to windows
    return msg.wParam;
}
