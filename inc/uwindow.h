#ifndef U_WINDOW_H
#define U_WINDOW_H

#ifdef   WINVER
#undef   WINVER
#endif
#define  WINVER   0x0501

#ifndef   IDI_APP
#define   IDI_APP  1
#endif


#include <windows.h>

//
class UWindow;

//
#define MESSAGE_MAP_DECLARE() \
public: \
    static MSGMAP_ENTRY MessageEntry[];

#define MESSAGE_MAP_BEGIN(ClassName) \
    MSGMAP_ENTRY UWindow::MessageEntry[] = \
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


class UWindow
{
public:
    //
    UWindow(): _hInst(NULL), _hParent(NULL), _hSelf(NULL)
    {
        this->lpszCaption = "UWindow";
    };
    //
    virtual ~UWindow() {};
    //
    void create(HINSTANCE hInstance)
    {
        WNDCLASSEX winclass = {0};

        winclass.cbSize = sizeof(WNDCLASSEX);
        winclass.style = CS_HREDRAW | CS_VREDRAW;
        winclass.lpfnWndProc = WindowProcedure;
        winclass.hInstance = hInstance;
        winclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        winclass.lpszClassName = "WinInit";
        //
        //winclass.lpszMenuName =
        winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        /* Use default icon and mouse-pointer */
        winclass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APP));
        winclass.hIconSm =  (HICON)LoadImage(hInstance, // small class icon
            MAKEINTRESOURCE(IDI_APP),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON),
            LR_DEFAULTCOLOR);

        RegisterClassEx(&winclass);

        _hSelf = CreateWindowEx(
                     0,
                     "WinInit",
                     this->lpszCaption,
                     WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
                     0,
                     0,
                     200,
                     240,
                     NULL,
                     NULL,
                     hInstance,
                     NULL);
    }

    //
    HWND getHParent() const {
        return _hParent;
    };
    //
    HINSTANCE getHinst() const {
        if (!_hInst)
        {
            ::MessageBox(NULL, "_hInst == NULL", "class Window", MB_OK);
            throw int(1999);
        }
        return _hInst;
    };
    //
    void setCaption(char *str)
    {
        this->lpszCaption = str;
    }
    //
    HWND getHWnd() const {
        return _hSelf;
    }
    //
    LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
    {
        int count = 0;
        while((MessageEntry[count].pfn) != 0)
        {
            if(MessageEntry[count].nMessage == nMsg)
            {
                (this->*MessageEntry[count].pfn)();
                return 0;
            }
            count++;
        }
        return DefWindowProc(_hSelf, nMsg, wParam, lParam);
    }
private:
    void onPaint();
    void onClose();
    void onChar();
protected:
    HINSTANCE _hInst;
    HWND _hParent;
    HWND _hSelf;
    const char *lpszCaption;

    MESSAGE_MAP_DECLARE()
};


//
class UApp
{
public:
    virtual void initInstance(HINSTANCE hInstance)
    {
        pWnd = new UWindow();
        pWnd->create(hInstance);
    };
    virtual ~UApp(){delete pWnd;};
    virtual int run()
    {
        MSG  msg;
        while(GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return (msg.wParam);
    }
    UWindow *GetWnd()
    {
        return pWnd;
    }

protected:
    UWindow *pWnd;
};


extern UApp *theApp;

#endif // U_WINDOW_H
