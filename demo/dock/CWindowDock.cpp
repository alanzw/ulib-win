//---------------------------------------------------------
// Name: CWindowDock.cpp
// Desc: Win32 API window docking system. Makes a list of
//       windows snap and dock to the parent window when
//       moved within a specified distance. Docked windows
//       will move with the parent window.
//---------------------------------------------------------
// Copyright © 2005 Nick Bloor
//---------------------------------------------------------

//-----------------
// Includes
//-----------------
#include "CWindowDock.h"

//-----------------
// Functions
//-----------------

//---------------------------------------------------------
// Name: CWDChild::CWDChild()
// Desc: CWDChild constructor, defaults member data.
//---------------------------------------------------------
CWDChild::CWDChild(): m_Hwnd(0), m_DockType(0), m_x(0), m_y(0)
{
}

//---------------------------------------------------------
// Name: CWDChild::~CWDChild()
// Desc: CWDChild destructor.
//---------------------------------------------------------
CWDChild::~CWDChild()
{
}

//---------------------------------------------------------
// Name: CWDChild::operator==()
// Desc: CWDChild equality operator.
//---------------------------------------------------------
bool CWDChild::operator ==(const CWDChild &rhs)
{
    //-----------------
    // Return true if window handles match
    //-----------------
    if(m_Hwnd == rhs.GetHandle())
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------
// Name: CWDChild::SetHandle()
// Desc: Set the window handle.
//---------------------------------------------------------
void CWDChild::SetHandle(HWND hwnd)
{
    //-----------------
    // Set the window handle
    //-----------------
    m_Hwnd = hwnd;
}

//---------------------------------------------------------
// Name: CWDChild::SetDockType()
// Desc: Set the docking type.
//---------------------------------------------------------
void CWDChild::SetDockType(unsigned char DockType)
{
    //-----------------
    // Set the docking type
    //-----------------
    m_DockType = DockType;
}

//---------------------------------------------------------
// Name: CWDChild::SetX()
// Desc: Set the x co-ordinate.
//---------------------------------------------------------
void CWDChild::SetX(long x)
{
    //-----------------
    // Set the x co-ord
    //-----------------
    m_x = x;
}

//---------------------------------------------------------
// Name: CWDChild::SetY()
// Desc: Set the y co-ordinate.
//---------------------------------------------------------
void CWDChild::SetY(long y)
{
    //-----------------
    // Set the y co-ord
    //-----------------
    m_y = y;
}

//---------------------------------------------------------
// Name: CWDChild::GetHandle()
// Desc: Returns the window handle.
//---------------------------------------------------------
HWND CWDChild::GetHandle() const
{
    //-----------------
    // Return the window handle
    //-----------------
    return m_Hwnd;
}

//---------------------------------------------------------
// Name: CWDChild::GetDockType()
// Desc: Returns the docking type.
//---------------------------------------------------------
unsigned char CWDChild::GetDockType() const
{
    //-----------------
    // Return the docking type
    //-----------------
    return m_DockType;
}

//---------------------------------------------------------
// Name: CWDChild::GetX()
// Desc: Returns the x co-ord.
//---------------------------------------------------------
long CWDChild::GetX() const
{
    //-----------------
    // Return the x co-ord
    //-----------------
    return m_x;
}

//---------------------------------------------------------
// Name: CWDChild::GetY()
// Desc: Returns the y co-ord.
//---------------------------------------------------------
long CWDChild::GetY() const
{
    //-----------------
    // Return the y co-ord
    //-----------------
    return m_y;
}

//---------------------------------------------------------
// Name: CWindowDock::CWindowDock()
// Desc: The CWindowDock constructor, defaults member
//       data.
//---------------------------------------------------------
CWindowDock::CWindowDock(): m_Parent(0), m_Threshold(CWD_DEF_THRESHOLD), m_Working(false),
                            m_AutoDock(false)
{
}

//---------------------------------------------------------
// Name: CWindowDock::~CWindowDock()
// Desc: The CWindowDock destructor.
//---------------------------------------------------------
CWindowDock::~CWindowDock()
{
    CWDChild *curChild = 0;

    //-----------------
    // Delete the list
    //-----------------
    while(m_ChildList.size() > 0)
    {
        //Pop the last node off
        curChild = m_ChildList.back();
        m_ChildList.pop_back();
        delete curChild;
    }
}

//---------------------------------------------------------
// Name: CWindowDock::SetParent()
// Desc: Sets the parent window handle.
//---------------------------------------------------------
void CWindowDock::SetParent(HWND hParent)
{
    //-----------------
    // Set the parent window handle
    //-----------------
    if(IsWindow(hParent) == TRUE)
    {
        m_Parent = hParent;
    }
}

//---------------------------------------------------------
// Name: CWindowDock::SetThreshold()
// Desc: Sets the docking threshold - the distance
//       between parent and child windows before docking
//       occurs.
//---------------------------------------------------------
void CWindowDock::SetThreshold(long lThreshold)
{
    //-----------------
    // Set the docking threshold
    //-----------------
    m_Threshold = lThreshold;
}

//---------------------------------------------------------
// Name: CWindowDock::AddChild()
// Desc: Add a window to the list of child windows.
//---------------------------------------------------------
void CWindowDock::AddChild(HWND hChild, bool AutoDock)
{
    CWDChild *newChild;

    //-----------------
    // Add the child window to the list
    //-----------------
    //Validate the window handle
    if(IsWindow(hChild) == false)
    {
        return;
    }

    //Setup a CWD_CHILD structure for the window
    newChild = new CWDChild;
    if(newChild == 0)
    {
        return;
    }
    newChild->SetDockType(CWD_DT_NONE);
    newChild->SetHandle(hChild);
    newChild->SetX(0);
    newChild->SetY(0);

    //Add the child to the list
    m_ChildList.push_back(newChild);

    //-----------------
    // Auto dock the child
    //-----------------
    if(AutoDock == true)
    {
        m_AutoDock = true;
        WindowMoved(hChild);
        m_AutoDock = false;
    }
}

//---------------------------------------------------------
// Name: CWindowDock::RemoveChild()
// Desc: Remove a window from the list of child windows.
//---------------------------------------------------------
void CWindowDock::RemoveChild(HWND hChild)
{
    CWDChild tmpChild;

    //-----------------
    // Remove the window from the list
    //-----------------
    tmpChild.SetHandle(hChild);
    m_ChildList.remove(&tmpChild);
}

//---------------------------------------------------------
// Name: CWindowDock::FindChild()
// Desc: Returns a pointer to a child window with a handle
//       matching the one passed to the function.
//---------------------------------------------------------
CWDChild *CWindowDock::FindChild(HWND hChild)
{
    list<CWDChild*>::iterator i;

    //-----------------
    // Find the child
    //-----------------
    for(i = m_ChildList.begin(); i != m_ChildList.end(); i++)
    {
        //Is this the requested child?
        if((*i)->GetHandle() == hChild)
        {
            return (*i);
        }
    }

    //-----------------
    // Window not in list
    //-----------------
    return 0;
}

//---------------------------------------------------------
// Name: CWindowDock::WindowMoved()
// Desc: The main part of the window docking system.
//       Tests for docking cases and moves windows as
//       required.
//---------------------------------------------------------
void CWindowDock::WindowMoved(HWND hwnd)
{
    list<CWDChild*>::iterator i;
    CWDChild *child = 0;
    RECT rcParent;
    RECT rcChild;
    RECT rcNew;
    bool bVert = false;
    bool bHorz = false;

    //-----------------
    // Exit if currently working
    //-----------------
    if(m_Working == true)
    {
        return;
    }
    m_Working = true;

    //-----------------
    // Do window docking
    //-----------------
    if(hwnd != m_Parent)
    {
        //Find the window to process
        child = FindChild(hwnd);
        if(child == 0)
        {
            //Window is not in the list of child windows
            m_Working = false;
            return;
        }

        //Get window rectangles
        GetWindowRect(m_Parent, &rcParent);
        GetWindowRect(hwnd, &rcChild);
        memcpy(&rcNew, &rcChild, sizeof(RECT));

        //Check docking cases and setup the new window position
        if(rcChild.left > rcParent.right - m_Threshold && rcChild.left < rcParent.right + m_Threshold)
        {
            bVert = true;
            rcNew.left = rcParent.right;
            child->SetDockType(CWD_DT_RIGHT);
            child->SetX(0);
            child->SetY(rcChild.top - rcParent.top);
        }
        if(rcChild.right > rcParent.left - m_Threshold && rcChild.right < rcParent.left + m_Threshold)
        {
            bVert = true;
            rcNew.left = rcParent.left - (rcChild.right - rcChild.left);
            child->SetDockType(CWD_DT_LEFT);
            child->SetX(0);
            child->SetY(rcChild.top - rcParent.top);
        }
        if(rcChild.top > rcParent.bottom - m_Threshold && rcChild.top < rcParent.bottom + m_Threshold)
        {
            bHorz = true;
            rcNew.top = rcParent.bottom;
            child->SetDockType(CWD_DT_BOTTOM);
            child->SetX(rcChild.left - rcParent.left);
            child->SetY(0);
        }
        if(rcChild.bottom > rcParent.top - m_Threshold && rcChild.bottom < rcParent.top + m_Threshold)
        {
            bHorz = true;
            rcNew.top = rcParent.top - (rcChild.bottom - rcChild.top);
            child->SetDockType(CWD_DT_TOP);
            child->SetX(rcChild.left - rcParent.left);
            child->SetY(0);
        }
        if(rcChild.left > rcParent.left - m_Threshold && rcChild.left < rcParent.left + m_Threshold)
        {
            if(bHorz == true)
            {
                rcNew.left = rcParent.left;
                if(child->GetDockType() == CWD_DT_TOP)
                {
                    child->SetDockType(CWD_DT_TOPLEFT);
                    child->SetX(0);
                    child->SetY(0);
                }
                if(child->GetDockType() == CWD_DT_BOTTOM)
                {
                    child->SetDockType(CWD_DT_BOTTOMLEFT);
                    child->SetX(0);
                    child->SetY(0);
                }
            }
        }
        if(rcChild.right > rcParent.right - m_Threshold && rcChild.right < rcParent.right + m_Threshold)
        {
            if(bHorz == true)
            {
                rcNew.left = rcParent.right - (rcChild.right - rcChild.left);
                if(child->GetDockType() == CWD_DT_TOP)
                {
                    child->SetDockType(CWD_DT_TOPRIGHT);
                    child->SetX(0);
                    child->SetY(0);
                }
                if(child->GetDockType() == CWD_DT_BOTTOM)
                {
                    child->SetDockType(CWD_DT_BOTTOMRIGHT);
                    child->SetX(0);
                    child->SetY(0);
                }
            }
        }
        if(rcChild.top > rcParent.top - m_Threshold && rcChild.top < rcParent.top + m_Threshold)
        {
            if(bVert == true)
            {
                rcNew.top = rcParent.top;
                if(child->GetDockType() == CWD_DT_LEFT)
                {
                    child->SetDockType(CWD_DT_LEFTTOP);
                    child->SetX(0);
                    child->SetY(0);
                }
                if(child->GetDockType() == CWD_DT_RIGHT)
                {
                    child->SetDockType(CWD_DT_RIGHTTOP);
                    child->SetX(0);
                    child->SetY(0);
                }
            }
        }
        if(rcChild.bottom > rcParent.bottom - m_Threshold && rcChild.bottom < rcParent.bottom + m_Threshold)
        {
            if(bVert == true)
            {
                rcNew.top = rcParent.bottom - (rcChild.bottom - rcChild.top);
                if(child->GetDockType() == CWD_DT_LEFT)
                {
                    child->SetDockType(CWD_DT_LEFTBOTTOM);
                    child->SetX(0);
                    child->SetY(0);
                }
                if(child->GetDockType() == CWD_DT_RIGHT)
                {
                    child->SetDockType(CWD_DT_RIGHTBOTTOM);
                    child->SetX(0);
                    child->SetY(0);
                }
            }
        }

        //Snap the toolbar into position
        if(rcNew.left != rcChild.left || rcNew.right != rcChild.right || rcNew.top != rcChild.top || rcNew.bottom != rcChild.bottom || m_AutoDock == true)
        {
            SetWindowPos(hwnd, 0, rcNew.left, rcNew.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }
        else
        {
            //Window is no longer docked
            child->SetDockType(CWD_DT_NONE);
            child->SetX(0);
            child->SetY(0);
        }
    }
    else
    {
        //Get the parent rectangle
        GetWindowRect(m_Parent, &rcParent);
        memcpy(&rcNew, &rcParent, sizeof(RECT));

        //Iterate through the child windows
        for(i = m_ChildList.begin(); i != m_ChildList.end(); i++)
        {
            //Get child pointer
            child = *i;

            //Reset new position rectangle
            memcpy(&rcNew, &rcParent, sizeof(RECT));

            //Get child window rectangle
            GetWindowRect(child->GetHandle(), &rcChild);

            //Setup the new window position depending on the dock code
            switch(child->GetDockType())
            {
            case CWD_DT_NONE:
                rcNew.left = rcChild.left;
                rcNew.top = rcChild.top;
                break;

            case CWD_DT_TOP:
                rcNew.left += child->GetX();
                rcNew.top -= (rcChild.bottom - rcChild.top);
                break;

            case CWD_DT_TOPRIGHT:
                rcNew.left = rcParent.right - (rcChild.right - rcChild.left);
                rcNew.top -= (rcChild.bottom - rcChild.top);
                break;

            case CWD_DT_RIGHTTOP:
                rcNew.left = rcParent.right;
                break;

            case CWD_DT_RIGHT:
                rcNew.left = rcParent.right;
                rcNew.top += child->GetY();
                break;

            case CWD_DT_RIGHTBOTTOM:
                rcNew.left = rcParent.right;
                rcNew.top = rcParent.bottom - (rcChild.bottom - rcChild.top);
                break;

            case CWD_DT_BOTTOMRIGHT:
                rcNew.left = rcParent.right - (rcChild.right - rcChild.left);
                rcNew.top = rcParent.bottom;
                break;

            case CWD_DT_BOTTOM:
                rcNew.left += child->GetX();
                rcNew.top = rcParent.bottom;
                break;

            case CWD_DT_BOTTOMLEFT:
                rcNew.top = rcParent.bottom;
                break;

            case CWD_DT_LEFTBOTTOM:
                rcNew.left -= (rcChild.right - rcChild.left);
                rcNew.top = rcParent.bottom - (rcChild.bottom - rcChild.top);
                break;

            case CWD_DT_LEFT:
                rcNew.left -= (rcChild.right - rcChild.left);
                rcNew.top += child->GetY();
                break;

            case CWD_DT_LEFTTOP:
                rcNew.left -= (rcChild.right - rcChild.left);
                break;

            case CWD_DT_TOPLEFT:
                rcNew.top -= (rcChild.bottom - rcChild.top);
                break;

            default:
                //Error
                rcNew.left = rcChild.left;
                rcNew.top = rcChild.top;
                break;
            }

            //Move the window
            SetWindowPos(child->GetHandle(), 0, rcNew.left, rcNew.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }
    }

    //-----------------
    // No longer working
    //-----------------
    m_Working = false;
}
