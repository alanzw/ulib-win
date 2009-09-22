//---------------------------------------------------------
// Name: CWindowDock.h
// Desc: Win32 API window docking system. Makes a list of
//       windows snap and dock to the parent window when
//       moved within a specified distance. Docked windows
//       will move with the parent window.
//---------------------------------------------------------
// Copyright © 2005 Nick Bloor
//---------------------------------------------------------

//Inclusion Guard
#ifndef _CWINDOWDOCK_INCLUDE_
#define _CWINDOWDOCK_INCLUDE_

//-----------------
// Includes
//-----------------
#include <windows.h>
#include <list>
using namespace std;

//-----------------
// Constants
//-----------------
//Docking types - The first word refers to the parent edge that the child is docked to. The second
//                word refers to the parent edge that the child is aligned with.
const unsigned char CWD_DT_NONE = 0; //Not docked
const unsigned char CWD_DT_TOP = 1; //Docked at the top of the parent
const unsigned char CWD_DT_TOPRIGHT = 2; //Docked at the top right of the parent
const unsigned char CWD_DT_RIGHTTOP = 3; //Docked at the right top of the parent
const unsigned char CWD_DT_RIGHT = 4; //Docked at the right of the parent
const unsigned char CWD_DT_RIGHTBOTTOM = 5; //Docked at the right bottom of the parent
const unsigned char CWD_DT_BOTTOMRIGHT = 6; //Docked at the bottom right of the parent
const unsigned char CWD_DT_BOTTOM = 7; //Docked at the bottom of the parent
const unsigned char CWD_DT_BOTTOMLEFT = 8; //Docked at the bottom left of the parent
const unsigned char CWD_DT_LEFTBOTTOM = 9; //Docked at the left bottom of the parent
const unsigned char CWD_DT_LEFT = 10; //Docked at the left of the parent
const unsigned char CWD_DT_LEFTTOP = 11; //Docked at the left top of the parent
const unsigned char CWD_DT_TOPLEFT = 12; //Docked at the top left of the parent;

//Default values
const long CWD_DEF_THRESHOLD = 10; //Default docking threshold

//-----------------
// Classes
//-----------------
//CWDChild - Defines a child window
class CWDChild
{
public:
    //Constructors
    CWDChild();
    ~CWDChild();

    //Operators
    bool operator ==(const CWDChild &rhs);

    //Set properties
    void SetHandle(HWND hwnd);
    void SetDockType(unsigned char DockType);
    void SetX(long x);
    void SetY(long y);

    //Get properties
    HWND GetHandle() const;
    unsigned char GetDockType() const;
    long GetX() const;
    long GetY() const;

private:
    //Member data
    HWND m_Hwnd; //Window handle
    unsigned char m_DockType; //Docking type
    long m_x; //X co-ordinate of the window relative to the parent upon docking
    long m_y; //Y co-ordinate of the window relative to the parent upon docking
};

//CWindowDock - The window docking system
class CWindowDock
{
public:
    //Constructors
    CWindowDock();
    ~CWindowDock();

    //Set properties
    void SetParent(HWND hParent);
    void SetThreshold(long lThreshold);

    //Add, remove and find child windows
    void AddChild(HWND hChild, bool AutoDock = true);
    void RemoveChild(HWND hChild);
    CWDChild *FindChild(HWND hChild);

    //Update window positions and dock states
    void WindowMoved(HWND hwnd);

private:
    //Member data
    HWND m_Parent; //The parent window
    long m_Threshold; //The docking threshold
    list<CWDChild*> m_ChildList; //List of child windows
    bool m_Working; //Stops WindowMoved from being called while it is already running
    bool m_AutoDock; //Set to true when adding a window to auto dock it
};

#endif //_CWINDOWDOCK_INCLUDE_
