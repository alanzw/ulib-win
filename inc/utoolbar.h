#ifndef U_TOOLBAR_H
#define U_TOOLBAR_H

#include "ucontrol.h"

class ULIB_API UToolBar : public UControl
{
public:
    UToolBar(HWND hParent, UINT nResource, HINSTANCE hInst);
    UToolBar() {}
    ~UToolBar();
    virtual BOOL create();
    //
    virtual BOOL setImageList(HIMAGELIST himl);
    virtual BOOL loadImages(UINT nIDBitmap);
    //
    virtual BOOL addButtons(int num, TBBUTTON *ptbb);
    virtual BOOL autosize();
    //
    virtual BOOL enableButton(int idButton);
    virtual BOOL disableButton(int idButton);
    //
    // Return: the previous extended styles
    DWORD setExtendStyle(DWORD dwExStyle);
    //
    DWORD getExtendStyle();

    //
    DWORD getButtonSize();
    DWORD getButtonRect(int nId, LPRECT lpRect);

    //
    BOOL hideButton(int nId);
    BOOL showButton(int nId);
    //
    virtual BOOL setHotImageList(HIMAGELIST himl);

    //
    int getButtonCount();
    //
    BOOL getButton(int nIndex, LPTBBUTTON lpTbBtn);
    BOOL getButtonInfo(UINT nID, LPTBBUTTONINFO lptbbi);
    BOOL getButtonText(UINT nID, LPTSTR lpszText);
    //
    BOOL getString(int nIndex, LPTSTR lpText, BYTE cchMax);
protected:
private:
};

#endif // U_TOOLBAR_H

