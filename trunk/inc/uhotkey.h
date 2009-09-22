/*
 * =====================================================================================
 *
 *       Filename:  uhotkey.h
 *
 *    Description:  hotkey control
 *
 *        Version:  1.0
 *        Created:  2009-7-19 10:00:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_HOTKEY_H
#define U_HOTKEY_H

#include "ucontrol.h"

class ULIB_API UHotKey : public UControl
{
public:
    UHotKey(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UHotKey();
    virtual BOOL create();
protected:
private:
};

#endif // U_HOTKEY_H

