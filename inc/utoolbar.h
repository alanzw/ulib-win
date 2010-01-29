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
    BOOL addBitmap(int n, UINT nBitmap, HINSTANCE hInst = ::GetModuleHandle(NULL));
    //
    virtual BOOL addButtons(int num, TBBUTTON *ptbb);
    BOOL addButton(TBBUTTON *ptbb);
    BOOL addButton(int iBitmap, int iCommand, BYTE fsState, BYTE fsStyle, INT_PTR iString = -1);
    virtual BOOL autosize();
    //
    BOOL addSeparator(int nWidth);
    //
    virtual BOOL enableButton(int idButton);
    virtual BOOL disableButton(int idButton);
    //
    LONG getState(int nID);
    BOOL setState(int nID, BYTE fState);
    //
    // Return: the previous extended styles
    DWORD setExtendStyle(DWORD dwExStyle);
    //
    DWORD getExtendStyle();

    //
    DWORD getButtonSize();
    DWORD getButtonRect(int nId, LPRECT lpRect);

    //
    BOOL setButtonWidth(int cxMin, int cxMax);
    BOOL setButtonSize(int dx, int dy);
    BOOL setBitmapSize(int dx, int dy);
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
    BOOL setButtonInfo(UINT nID, LPTBBUTTONINFO lptbbi);
    //
    BOOL getString(int nIndex, LPTSTR lpText, BYTE cchMax);
protected:
private:
};

#endif // U_TOOLBAR_H

