/*
 * =====================================================================================
 *
 *       Filename:  customctrl.h
 *
 *    Description:  Customized Control Class
 *
 *        Version:  1.0
 *        Created:  2009-9-18 14:42:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_CUSTOMIZED_CONTROL_H
#define U_CUSTOMIZED_CONTROL_H

#include "ucontrol.h"

class ULIB_API UCustomCtrl : public UControl
{
public:
    UCustomCtrl(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UCustomCtrl();

    virtual BOOL create();
    virtual BOOL onLButtonDown(WPARAM wParam, LPARAM lParam);
    virtual BOOL onLButtonUp(WPARAM wParam, LPARAM lParam);
    virtual BOOL onMouseMove(WPARAM wParam, LPARAM lParam);
private:
};

#endif // U_CUSTOMIZED_CONTROL_H

