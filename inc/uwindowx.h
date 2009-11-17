#ifndef U_WINDOW_H
#define U_WINDOW_H

#include "ulib.h"
#include "uwndclassx.h"

namespace huys
{
//
class UWindow;

//
#define MESSAGE_MAP_DECLARE() \
public: \
    static MSGMAP_ENTRY MessageEntry[];

#define MESSAGE_MAP_BEGIN(ClassName) \
    MSGMAP_ENTRY ClassName::MessageEntry[] = \
{

#define MESSAGE(MSG, PROC) \
{MSG, &PROC},

#define MESSAGE_MAP_END() \
{0, 0} \
};



typedef struct _MSGMAP_ENTRY
{
    UINT nMessage;
    void (UWindow::*pfn)();
} MSGMAP_ENTRY;

//
LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);

class ULIB_API UWindow
{
public:
    //
    UWindow();
    //
    virtual ~UWindow() {};
    //
    BOOL create(HINSTANCE hInstance);

    //
    HWND getHParent() const { return _hParent; };
    //
    HINSTANCE getHinst() const {
        if (!_hInst)
        {
            ::MessageBox(NULL, _T("_hInst == NULL"), _T("class Window"), MB_OK);
            throw int(1999);
        }
        return _hInst;
    };
    //
    void setCaption(LPCTSTR str) { this->lpszCaption = str; }
    //
    HWND getHWnd() const { return _hSelf; }
    //
    LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

private:
    void onPaint();
    void onClose();
    void onChar();
protected:
    HINSTANCE _hInst;
    HWND _hParent;
    HWND _hSelf;
    LPCTSTR lpszCaption;

    MESSAGE_MAP_DECLARE()
};

//
class ULIB_API UWindowApp
{
public:
    virtual BOOL initInstance(HINSTANCE hInstance);

    virtual ~UWindowApp();

    virtual int run();

    UWindow *GetWnd() { return m_pWnd; }

protected:
    UWindow *m_pWnd;
};

extern ULIB_API huys::UWindowApp *theApp;

}; // namespace huys

#endif // U_WINDOW_H

