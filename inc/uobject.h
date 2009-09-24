/*
 * =====================================================================================
 *
 *       Filename:  uobject.h
 *
 *    Description:  windows user object
 *
 *        Version:  1.0
 *        Created:  2009-7-3 0:01:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_OBJECT_H
#define U_OBJECT_H

#include "ulib.h"

typedef struct tagUObject
{
    ///
    HANDLE m_hObj;

    ///
    BOOL isNull()
    {
        return NULL == m_hObj;
    }
} UObject, *UObjectPtr;

class ULIB_API UUserObject : public UObject
{
public:
    UUserObject();
    ~UUserObject();

    bool load();
    bool create();
    virtual bool destroy();
protected:
private:
};

class ULIB_API UKernelObject : public UObject
{
public:
    UKernelObject();
    ~UKernelObject();

    bool create();
    bool open();
    bool close();

    operator HANDLE ()
    {
        return m_hObj;
    }
protected:
private:
};

class ULIB_API UGDIObject : public UObject
{
public:
    UGDIObject();
    virtual ~UGDIObject();

    bool select(HDC hdc);
    bool restore();
protected:
    //HANDLE m_hObj;
    HANDLE m_hOldObj;
    //
    HDC m_hdc;
private:
};

#endif // U_OBJECT_H

/**
 *  User object
 *    Accelerator table
 *    Caret
 *    Cursor
 *    DDE conversation
 *    Hook
 *    Icon
 *    Menu
 *    Window
 *    Window position
 */

/**
 *  GDI object
 *    Bitmap
 *    Brush
 *    DC
 *    Enhanced metafile
 *    Enhanced-metafile DC
 *    Font
 *    Memory DC
 *    Metafile
 *    Metafile DC
 *    Palette
 *    Pen and extended pen
 *    Region
 */

/**
 *   Kernel object
 *     Access token
 *     Change notification
 *     Communications device
 *     Console input
 *     Console screen buffer
 *     Desktop
 *     Event
 *     Event log
 *     File
 *     File mapping
 *     Heap
 *     Job
 *     Mailslot
 *     Module
 *     Mutex
 *     Pipe
 *     Process
 *     Semaphore
 *     Socket
 *     Thread
 *     Timer
 *     Timer queue
 *     Timer-queue timer
 *     Update resource
 *     Window station
 */
