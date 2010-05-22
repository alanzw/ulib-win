#ifndef U_BUTTON_H
#define U_BUTTON_H

#include "ucontrol.h"
#include "ubasewindow.h"
//
class UButton;

//
typedef UButton UPushButton;

class ULIB_API UButton : public UControl
{
public:
    UButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    UButton() {}
    UButton(UBaseWindow *pWndParent, UINT nID);
    virtual ~UButton();
    virtual BOOL create();

    BOOL click();

    BOOL getIdealSize(SIZE *pSize);
protected:
private:
};

#if WINVER >= 0x0600 // VISTA

class ULIB_API USplitButton: public UButton
{
public:
    USplitButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~USplitButton();
protected:
private:
};

#endif // WINVER >= 0x0600

class ULIB_API UCheckButton: public UButton
{
public:
    UCheckButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UCheckButton();
    //
    BOOL check();
    BOOL uncheck();


};


class ULIB_API URadioButton: public UButton
{
public:
    URadioButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    URadioButton(UBaseWindow *pWndParent, UINT nID);
    virtual ~URadioButton();
    URadioButton() {};
    //
    BOOL check();
    BOOL uncheck();

    //
    BOOL isChecked();
};


class ULIB_API UGroupBox : public UButton
{
public:
    UGroupBox(HWND hParent, UINT nResource, HINSTANCE hInst);
    UGroupBox() {}
    UGroupBox(UBaseWindow *pWndParent, UINT nID);
    virtual ~UGroupBox();
};

class ULIB_API UIconButton : public UButton
{
public:
    UIconButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    UIconButton() {}
    virtual ~UIconButton();
    BOOL setIcon(HICON hicon);
};

class ULIB_API UBitmapButton : public UButton
{
public:
    UBitmapButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    virtual ~UBitmapButton();
    BOOL setBitmap(HBITMAP hbm);
};

class ULIB_API UIconTextButton : public UIconButton
{
public:
    UIconTextButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    UIconTextButton() {}
    virtual ~UIconTextButton();
};

class ULIB_API UOwnerDrawnButton : public UButton
{
public:
    UOwnerDrawnButton(HWND hParent, UINT nResource, HINSTANCE hInst);
    UOwnerDrawnButton() {}
    UOwnerDrawnButton(UBaseWindow *pWndParent, UINT nID);
    virtual ~UOwnerDrawnButton();
    virtual BOOL create();
    virtual BOOL onDrawItem(WPARAM,LPARAM);
};

#endif // U_BUTTON_H

/**
 *  Button Styles
 *
 *    BS_3STATE
 *    BS_AUTO3STATE
 *    BS_AUTOCHECKBOX
 *    BS_AUTORADIOBUTTON
 *    BS_CHECKBOX
 *    BS_DEFPUSHBUTTON
 *    BS_GROUPBOX
 *    BS_LEFTTEXT
 *    BS_OWNERDRAW
 *    BS_PUSHBUTTON
 *    BS_RADIOBUTTON
 *    BS_USERBUTTON
 *    BS_BITMAP
 *    BS_BOTTOM
 *    BS_CENTER
 *    BS_ICON
 *    BS_FLAT
 *    BS_LEFT
 *    BS_MULTILINE
 *    BS_NOTIFY
 *    BS_PUSHLIKE
 *    BS_RIGHT
 *    BS_RIGHTBUTTON
 *    BS_TEXT
 *    BS_TOP
 *    BS_TYPEMASK
 *    BS_VCENTER
 *    BS_SPLITBUTTON      >=VISTA
 *    BS_DEFSPLITBUTTON   >=VISTA
 *    BS_COMMANDLINK      >=VISTA
 *    BS_DEFCOMMANDLINK   >=VISTA
 */

